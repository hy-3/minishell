/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:02:49 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/28 15:20:41 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	write_each_line(int fd, char *id_str, char *str)
{
	write(fd, "  ", 2);
	write(fd, id_str, ft_strlen(id_str));
	write(fd, "  ", 2);
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
}

void	handle_error(t_env *env)
{
	printf("history: too many arguments\n");
	env->status_code = 1;
}

void	save_history(char *str, t_env *env)
{
	int		fd;
	int		id;
	char	*id_str;
	char	*each_cmd;

	if (ft_strncmp(str, "", 2) == 0)
		return ;
	fd = open(env->history_file, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
		cust_write("file open error 222\n", 1);
	add_history(str);
	id = 1;
	while (1)
	{
		each_cmd = get_next_line(fd);
		if (each_cmd == NULL)
			break ;
		id++;
		free(each_cmd);
	}
	id_str = ft_itoa(id);
	write_each_line(fd, id_str, str);
	free(id_str);
	if (close(fd) == -1)
		cust_perror("Error(close: .history)", 1);
}

void	exec_history(t_cmd *cmd, t_env *env, int i)
{
	int		input_fd;
	int		output_fd;
	char	*each_cmd;

	if (cmd->num_of_args == 1)
	{
		input_fd = open(env->history_file, O_RDONLY);
		if (input_fd == -1)
			cust_write("file open error\n", 1);
		output_fd = get_output_fd(cmd, env, i);
		while (1)
		{
			each_cmd = get_next_line(input_fd);
			if (each_cmd == NULL)
				break ;
			write(output_fd, each_cmd, ft_strlen(each_cmd));
			free(each_cmd);
		}
		if (close(input_fd) == -1)
			cust_perror("Error(close: input_fd)", 1);
	}
	else
		handle_error(env);
}
