#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int	ft_strlen(char *str) {
	int i = -1;
	int	len = 0;

	while (str[++i]) {
		len++;
	}
	return len;
}

void	ft_puterr(char *str, char *path) {
		write(2, str, ft_strlen(str));
		if (path)
			write(2, path, ft_strlen(path));
		write(2, "\n", 1);
}

void    execute(char **av, int i, int tmpFD, char **env) {
	av[i] = NULL;
	if (tmpFD)
		dup2(tmpFD, STDIN_FILENO);
	close(tmpFD);
	if (execve(av[0], av, env) < 0)
		ft_puterr("error: cannot execute ", av[0]);
	exit(EXIT_FAILURE);
}

int main(int ac, char **av, char **env) {
    int     i = 0;
	int		pipeFD[2];
	int		tmpFD;

	if (ac == 1)
		return 1;
	tmpFD = 0;
	while (av[i]) {
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (av[0] && !strcmp(av[0], "cd")) {
			if (i != 2)
				ft_puterr("error: cd: bad arguments", NULL);
			else {
				if (chdir(av[1]) < 0)
					ft_puterr("error: cd: cannot change directory to ", av[1]);
			}
		}
		else if (i && (!av[i] || !strcmp(av[i], ";"))) {
			if (!fork()) {
				execute(av, i, tmpFD, env);
			}
			waitpid(-1, NULL, 0);
		}
		else if (i) {
			if (pipe(pipeFD) < 0) {
				ft_puterr("error: fatal", NULL);
				exit(EXIT_FAILURE);
			}
			if (!fork()) {
				dup2(pipeFD[1], STDOUT_FILENO);
				close(pipeFD[1]);
				close(pipeFD[0]);
				execute(av, i, tmpFD, env);
			}
			close(pipeFD[1]);
			if (tmpFD)
				close(tmpFD);
			tmpFD = pipeFD[0];
			waitpid(-1, NULL, 0);
		}
	}
	if (tmpFD)
		close(tmpFD);
    return 0;
}