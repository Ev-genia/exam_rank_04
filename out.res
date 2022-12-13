/bin/ls

00_microshell.c
01_microshell.c
02_microshell.c
02_microshell.h
a.out
leaks.res
microshell
microshell.c
microshell.dSYM
out.res
test.sh
/bin/cat microshell.c
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include<stdio.h>

int	ft_size_cmd(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] && strcmp(arr[i], ";") == 0 && strcmp(arr[i], "|") == 0)
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
	while (s[i] && s[i] != '\0')
		i++;
	return (i);
}

int	ft_cd(char **arr)
{
	if (arr[1] && arr[2] == NULL)
	{
		if (chdir(arr[1]) == -1)
		{
			write(2, "error: cd: cannot change directory to ", 38);
			write(2, arr[1], ft_strlen(arr[1]));
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
		if (av[i] && strcmp(av[i], ";") == 0 && strcmp(av[i], "|") == 0)
			i++;
		else
		{
			len = ft_size_cmd(&av[i]);
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
				dup2(fd_pipe[1], 1);
				close(fd_pipe[1]);
			}
			if (av[i] && strcmp(cmd[0], "cd") == 0)
				ft_cd(cmd);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					if (execve(cmd[0], cmd, env) == -1)
					{
						write(2, "error: cannot execute ", 22);
						write(2, cmd[0], ft_strlen(cmd[0]));
						write(2, "\n", 1);
						free(cmd);
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
				if (pipe(fd_pipe) == -1)
					ft_fatal_exit();
				dup2(save_fd_out, 1);
				flag_pipe = 1;
			}
			free(cmd);
		}
	}
	return (0);
}
/bin/ls microshell.c
microshell.c

/bin/ls salut

;

; ;

; ; /bin/echo OK

; ; /bin/echo OK ;

; ; /bin/echo OK ; ;

; ; /bin/echo OK ; ; ; /bin/echo OK

/bin/ls | /usr/bin/grep microshell

/bin/ls | /usr/bin/grep microshell | /usr/bin/grep micro

/bin/ls | /usr/bin/grep microshell | /usr/bin/grep micro | /usr/bin/grep shell | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro

/bin/ls | /usr/bin/grep microshell | /usr/bin/grep micro | /usr/bin/grep shell | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep micro | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell | /usr/bin/grep shell

/bin/ls ewqew | /usr/bin/grep micro | /bin/cat -n ; /bin/echo dernier ; /bin/echo

/bin/ls | /usr/bin/grep micro | /bin/cat -n ; /bin/echo dernier ; /bin/echo ftest ;

/bin/echo ftest ; /bin/echo ftewerwerwerst ; /bin/echo werwerwer ; /bin/echo qweqweqweqew ; /bin/echo qwewqeqrtregrfyukui ;
ftest

/bin/ls ftest ; /bin/ls ; /bin/ls werwer ; /bin/ls microshell.c ; /bin/ls subject.fr.txt ;

/bin/ls | /usr/bin/grep micro ; /bin/ls | /usr/bin/grep micro ; /bin/ls | /usr/bin/grep micro ; /bin/ls | /usr/bin/grep micro ;

/bin/cat subject.fr.txt | /usr/bin/grep a | /usr/bin/grep b ; /bin/cat subject.fr.txt ;

/bin/cat subject.fr.txt | /usr/bin/grep a | /usr/bin/grep w ; /bin/cat subject.fr.txt ;

/bin/cat subject.fr.txt | /usr/bin/grep a | /usr/bin/grep w ; /bin/cat subject.fr.txt

/bin/cat subject.fr.txt ; /bin/cat subject.fr.txt | /usr/bin/grep a | /usr/bin/grep b | /usr/bin/grep z ; /bin/cat subject.fr.txt

; /bin/cat subject.fr.txt ; /bin/cat subject.fr.txt | /usr/bin/grep a | /usr/bin/grep b | /usr/bin/grep z ; /bin/cat subject.fr.txt

blah | /bin/echo OK

blah | /bin/echo OK ;

