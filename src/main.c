/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uclement <uclement@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:53:48 by uclement          #+#    #+#             */
/*   Updated: 2023/07/27 14:23:59 by uclement         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void free_lst(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}


void p_process(t_arg arg, int f2, int *fd)
{
	int		i;
	int		status;
	char	**mycmdarg;
	char	*cmd;

	waitpid(-1, &status, 0);
	dup2(f2, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	mycmdarg =	ft_split(arg.av[3], ' ');
	i = 0;
	close(fd[0]);
	close(f2);
	while (arg.path[i])
	{
		arg.path[i] = ft_strjoin(arg.path[i], "/");
		cmd = ft_strjoin(arg.path[i], mycmdarg[0]);
		if (access(cmd, X_OK) == 0)
			break ;
		i++;
		free(cmd);
	}
	execve(cmd, mycmdarg, arg.ev);
		// perror("errorp");
}

void c_process (t_arg arg, int f1, int *fd)
{
	int		i;
	char	**mycmdarg;
	char	*cmd;

	dup2(f1, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	mycmdarg =	ft_split(arg.av[2], ' ');
	i = 0;
	close(fd[1]);
	close(f1);
	while (arg.path[i])
	{
		arg.path[i] = ft_strjoin(arg.path[i], "/");
		cmd = ft_strjoin(arg.path[i], mycmdarg[0]);
		if (access(cmd, X_OK) == 0)
			break ;
		i++;
		free(cmd);
	}
	execve(cmd, mycmdarg, arg.ev);
}

void pipex(t_arg arg, int f1, int f2)
{
	int	fd[2];
	int	pid;
	
	if (pipe(fd) == -1)
		perror("Error pipe");
	pid = fork();
	if (pid == 0 )
	{
		close(f2);
		c_process(arg, f1, fd);
	}
	if (pid != 0 )
	{
		close(f1);
		p_process(arg, f2, fd);
	}
	close(fd[0]);
	close(fd[1]);
	close(f1);
	close(f2);
	free_lst(arg.path);
}

char **pathfinder(t_arg arg)
{
	int i;

	i = 0;
	while (arg.ev[i])
		{
			if (ft_strncmp(arg.ev[i], "PATH", 4) == 0)
				break ;
			i++;
		}
	return(ft_split(&arg.ev[i][5], ':'));
}

int	main(int argc, char **argv, char **envp)
{
	int		f1;
	int		f2;
	t_arg	arg;

	arg.av = argv;
	arg.ev = envp;
	arg.path = pathfinder(arg);
	if (argc != 5)
		return (1);
	if ((f1 = open(argv[1], O_RDONLY)) == -1)
		perror("error open");
	if ((f2 =  open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644)) == -1)
		perror("error open");
	pipex(arg, f1, f2);
}
