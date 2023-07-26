/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uclement <uclement@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:53:48 by uclement          #+#    #+#             */
/*   Updated: 2023/07/26 13:38:54 by uclement         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	cmd(char **argv, char **envp)
// {
// 	int		i;
// 	char	**mypath;
// 	char	**mycmdarg;
// 	char	*cmd;

// 	mycmdarg = ft_split(argv[1], ' ');
// 	i = 0;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH", 4) == 0)
// 			break ;
// 		i++;
// 	}
// 	mypath = ft_split(&envp[i][5], ':');
// 	i = 0;
// 	while (mypath[i])
// 	{
// 		mypath[i] = ft_strjoin(mypath[i], "/");
// 		cmd = ft_strjoin(mypath[i], mycmdarg[0]);
// 		if (execve(cmd, mycmdarg, envp) >= 0)
// 			exit(0);
// 		i++;
// 		free(cmd);
// 	}
// }

void p_process(char **argv, char **envp, int f2, int *fd)
{
	int		i;
	char	**mypath;
	char	**mycmdarg;
	char	*cmd;
	int status;

	waitpid(-1, &status, 0);
	dup2(f2, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	i = 0;
	i = 0;
	mycmdarg =	ft_split(argv[2], ' ');
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			break ;
		i++;
	}
	mypath = ft_split(&envp[i][5], ':');
	i = 0;
	close(f2);
	while (mypath[i])
	{
		mypath[i] = ft_strjoin(mypath[i], "/");
		cmd = ft_strjoin(mypath[i], mycmdarg[0]);
		if (execve(cmd, mycmdarg, envp) >= 0)
			exit(0);
		i++;
		free(cmd);
	}
}

void c_process(char **argv, char **envp, int f1, int *fd)
{
	int		i;
	char	**mypath;
	char	**mycmdarg;
	char	*cmd;

	dup2(f1, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	i = 0;
	i = 0;
	mycmdarg =	ft_split(argv[3], ' ');
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			break ;
		i++;
	}
	mypath = ft_split(&envp[i][5], ':');
	i = 0;
	close(f1);
	while (mypath[i])
	{
		mypath[i] = ft_strjoin(mypath[i], "/");
		cmd = ft_strjoin(mypath[i], mycmdarg[0]);
		if (execve(cmd, mycmdarg, envp) >= 0)
			exit(0);
		i++;
		free(cmd);
	}
}


void pipex(char **argv, char **envp, int f1, int f2) //ajout f1 et f2
{
	int pid;
	int	fd[2];
	
	pipe(fd);
	pid = fork();
	if (pid != 0 ) // parent
	{
		p_process(argv, envp, f2, fd);
	}
	else if (pid == 0 )
		c_process(argv, envp, f1, fd);
}

int	main(int argc, char **argv, char **envp)
{
	int f1;
	int f2;
	
	if (argc <= 1)
		return (0);
	
	f1 =  open(argv[1], O_RDONLY);
	f2 =  open(argv[4], O_RDWR, 0644);
	pipex(argv, envp, f1, f2);
	

}
