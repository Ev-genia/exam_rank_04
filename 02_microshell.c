#include "microshell.h"

int	ft_size_cms(char **str)
{
	int	i;

	i = 0;
	while (str[i] && strcmp(str[i], ";") != 0 && strcmp(str[i], "|") != 0)
		i++;
	return (i);
}

void	ft_fatal_exit(void)
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != ';' && s[i] != '|')
		i++;
	return (i);
}

int	ft_cd(char **arg)
{
	int	rez;

	if (arg[1] && arg[2] == NULL)
	{
		rez = chdir(arg[1]);
		if (rez == -1)
		{
			write(2, "error: cd:cannot change directory to \n", 38);
			write(2, arg[1], ft_strlen(arg[1]));
			write(2, "\n", 1);
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
	char	**cmd;
	int		j;
	int		flag_pipe;
	int		fd_pipe[2];
	pid_t	pid;
	int		save_fd_in;
	int		save_fd_out;

	save_fd_in = dup(0);
	save_fd_out = dup(1);
	i = 1;
	while (i < ac)
	{

		if (strcmp(av[i], "|") == 0 || strcmp(av[i], ";") == 0)
			i++;
		else
		{

			len = ft_size_cms(&av[i]);

			cmd = (char **) malloc(sizeof(char *) * (len + 1));
			if (!cmd)
				ft_fatal_exit();
			cmd[len] = NULL;
			j = i;
			while (av[i] && strcmp(av[i], ";") != 0 && strcmp(av[i], "|") != 0)
			{
				cmd[i - j] = av[i];
				i++;
			}
			if (flag_pipe)
			{
				dup2(fd_pipe[0], 0);
				close(fd_pipe[0]);
			}
			if (av[i] && strcmp(av[i], "|") == 0)
			{
				if (pipe(fd_pipe) == -1)
					ft_fatal_exit();
				dup2(fd_pipe[1], 1);
				close(fd_pipe[1]);
			}
			if (strcmp(cmd[0], "cd") == 0)
				ft_cd(cmd);
			else
			{
				pid = fork();
				if (pid == -1)
					ft_fatal_exit();
				if (pid == 0)
				{
					if (execve(cmd[0], cmd, env) == -1)
					{
						write(2, "error: cannot execute ", 22);
						write(2, cmd[0], ft_strlen(cmd[0]));
						free(cmd);
						return (1);
					}
				}
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
			free(cmd);
		}
	}
	return (0);
}