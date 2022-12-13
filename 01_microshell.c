#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int	ft_size_cmds(char **str)
{
	int	i;

	i = 0;
	while (str[i] && strcmp(str[i], "|") != 0 && strcmp(str[i], ";") != 0)
		i++;
	return (i);
}

void	ft_fatal_exit()
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\0')
		i++;
	return (i);
}

int	ft_cd(char **cmds)
{
	int	rez;

	if (cmds[1] && cmds[2] == NULL)
	{
		rez = chdir(cmds[1]);
		if (rez == -1)
		{
			write(2, "error: cd: cannot change directory to ", 38);
			write(2, cmds[1], ft_strlen(cmds[1]));
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
	char	**cmds;
	int		len;
	int		flag_pipe;
	int		pipe_fd[2];
	int		j;
	pid_t	pid;
	int		save_fd_in;
	int		save_fd_out;

	save_fd_in = dup(0);
	save_fd_out = dup(1);
	flag_pipe = 0;
	i = 1;
	while (i < ac)
	{
		if (strcmp(av[i], ";") == 0 || strcmp(av[i], "|") == 0)
			i++;
		else
		{
			len = ft_size_cmds(&av[i]);
			cmds = (char **)malloc(sizeof(char *) * (len + 1));
			if (!cmds)
				ft_fatal_exit();
			cmds[len] = NULL;
			j = i;
			while (av[i] && strcmp(av[i], "|") != 0 && strcmp(av[i], ";") != 0)
			{
				cmds[i - j] = av[i];
				i++;
			}
			if (flag_pipe)
			{
				dup2(pipe_fd[0], 0);
				close(pipe_fd[0]);
			}
			if (av[i] && strcmp(av[i], "|") == 0)
			{
				if (pipe(pipe_fd) == -1)
					ft_fatal_exit();
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
						write(2, "error: cannot execute ", 22);
						write(2, cmds[0], ft_strlen(cmds[0]));
						write(2, "\n", 1);
						free(cmds);
						return (1);
					}
				}
				if (pid < 0)
					ft_fatal_exit();
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