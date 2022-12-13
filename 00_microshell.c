/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_microshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlarra <mlarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 10:50:28 by mlarra            #+#    #+#             */
/*   Updated: 2022/10/03 14:58:52 by mlarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int	ft_len_line(char **str)
{
	int	i;

	i = 0;
	while (str[i] && strcmp(str[i], ";") != 0 && strcmp(str[i], "|") != 0)
		i++;
	return (i);
}

void	ft_exit_fatal()
{
	write(2, "fatal exit\n", 11);
	exit(1);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\0')
		i++;
	return (i);
}

int	ft_cd(char **args)
{
	int	rez;

	if (args[1] && args[2] == NULL)
	{
		rez = chdir(args[1]);
		if (rez == -1)
		{
			write(2, "error: cd: cannot change directory to ", 38);
			write(2, args[1], ft_strlen(args[1]));
			write(2, "\n", 1);
			return (1);
		}
	}
	else
		write(2, "error: cd: bad arguments\n", 25);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	int		len;
	char	**cmds;
	int		j;
	int		flag_pipe;
	int		pipe_fd[2];
	pid_t	pid;
	int		save_fd_in;
	int		save_fd_out;

	save_fd_in = dup(0);
	save_fd_out = dup(1);
	i = 1;
	flag_pipe = 0;
	while (i < ac)
	{
		if (strcmp(av[i], ";") == 0 || strcmp(av[i], "|") == 0)
			i++;
		else
		{
			len = ft_len_line(&av[i]);
			cmds = (char **) malloc(sizeof(char *) * (len + 1));
			if (!cmds)
				ft_exit_fatal();
			cmds[len] = NULL;
			j = i;
			while (av[i] && strcmp(av[i], ";") != 0 && strcmp(av[i], "|") != 0)
			{
				cmds[i - j] = av[i];
				i++;
			}
			if (flag_pipe == 1)
			{
				dup2(pipe_fd[0], 0);
				close(pipe_fd[0]);
			}
			if (av[i] && strcmp(av[i], "|") == 0)
			{
				if (pipe(pipe_fd) == -1)
					ft_exit_fatal();
				dup2(pipe_fd[1], 1);
				close(pipe_fd[1]);
			}
			if (strcmp(cmds[0], "cd") == 0)
				ft_cd(cmds);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					if (execve(cmds[0], cmds, env) == -1)
					{
						write (2, "error: cannot execute ", 22);
						write (2, cmds[0], ft_strlen(cmds[0]));
						write (2, "\n", 1);
						free (cmds);
						return (1);
					}
				}
				if (pid < 0)
					ft_exit_fatal();
				waitpid(pid, &j, 0);
			}
			if (flag_pipe)
			{
				dup2(save_fd_in, 0);
				flag_pipe = 0;
			}
			if (av[i] && strcmp(av[i], "|") == 0)
			{
				dup2(save_fd_out, 1);
				flag_pipe = 1;
			}
			free(cmds);
		}
	}
	return (0);
}