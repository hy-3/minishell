/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:04:35 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/28 18:57:52 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"
# include <sys/uio.h>
# include <stddef.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define BUFFER_SIZE 10

int	g_condition;

typedef struct s_list
{
	char			*str;
	struct s_list	*extra;
	struct s_list	*next;
}	t_list;

typedef struct s_env
{
	char	**current_envp;
	char	*pathenv;
	int		num_of_next_node;
	int		num_of_child;
	int		status_code;
	int		p[100][2];
	char	*history_file;
}	t_env;

typedef struct s_cmd
{
	char	*exec_args[100];
	int		pid;
	int		input_fd;
	int		output_fd;
	int		is_heredoc;
	int		heredoc_p[2];
	int		num_of_args;
	int		is_error;
}	t_cmd;

typedef struct s_parse
{
	t_list	*first_node;
	int		i;
	int		start;
	int		count;
	int		num_of_single_quote;
	int		num_of_double_quote;
	int		is_quote_closed;
	int		pipe_condition;
}	t_parse;

typedef struct s_fill
{
	int		i;
	int		start;
	int		count;
	char	*tmp_str;
	char	current_quote;
	int		end_pos_of_dollar_var;
	t_list	*list;
}	t_fill;

typedef struct s_dollar
{
	int		start;
	int		tmp_start;
	int		end;
	char	*str_dollar_part;
	int		size_dollar_part;
	int		is_free_needed;
}	t_dollar;

// src/parse
//	- parse.c
t_list	*parse(char *original_str, t_env *env);
//	- check.c
void	check_pipe_condition(char *original_str, t_parse *parse, t_env *env);
void	check_quote_condition(t_parse *parse, char c);
int		is_include_cmd(char *original_str, t_fill *fill);
//	- fill_str.c
void	fill_str(char *original_str, t_fill *fill, t_parse *parse, t_env *env);
void	fill_al(char *original_str, t_fill *fill);
//	- dollar.c
char	*convert_str_from_dollar(t_fill *fill, t_env *env);
char	*cust_getenv(char *key, t_env *env);
//	- dollar_util.c
void	upd_dollar_start_and_end(t_fill *fill, t_dollar *dollar);
int		is_dollar_exist(char *tmp_str);
int		find_question_position(char *str);
int		calc_length(t_dollar *dollar, t_fill *fill);

// src/signal
//	- signal.c
void	handle_signals(void);
void	rl_replace_line(char *s, int a);
void	change_terminal_setting(void);

// src/env
//	- env_upd.c
char	**copy_env(char **old_envp);
char	**create_newenv_upd(char **old_envp, char *str, char *pos_equal);
char	**create_newenv_add(char **old_envp, char *str);
//	- env_util.c
char	*get_value_of_pathenv(char **envp);
int		calc_envp_size(char **envp);
int		count_till_equal(char *str);
int		is_exist_in_env(char **envp, char *str);
int		is_valid_envname(char *str);
//	- shlvl.c
void	increment_shlvl(t_env *env);

// src/cmd
//	- pipex.c
void	pipex(t_list *list, t_env *env);
//	- check_cmd.c
char	*is_cmd_exist_and_executable(char *path_env, char *cmd);
//	- external_cmd.c
void	exec_external_cmd(t_cmd *cmd, t_env *env, int i);
//	- close_pipe.c
void	close_pipes(t_env *env, t_cmd *cmd, int i);
void	close_pipes_when_ctrlc(t_env *env, t_cmd *cmd, int i);
//	- wait.c
void	handle_wait(t_env *env, t_cmd *cmd);

// src/cmd/builtin
//	- *.c
void	exec_cd(t_cmd *cmd, t_env *env, int i);
void	exec_echo(t_cmd *cmd, t_env *env, int i);
void	exec_env(t_cmd *cmd, t_env *env, int i);
void	exec_exit(t_cmd *cmd, t_env *env, int i);
void	exec_export(t_cmd *cmd, t_env *env, int i);
char	**handle_export(char **old_envp, char *str, t_env *env);
void	exec_pwd(t_cmd *cmd, t_env *env, int i);
void	exec_unset(t_cmd *cmd, t_env *env, int i);
void	exec_history(t_cmd *cmd, t_env *env, int i);
void	save_history(char *str, t_env *env);

// redirection
//	- check_arrows.c
t_list	*check_arrows(t_list *list, t_cmd *cmd, t_env *env);
//	- check_arrows_util.c
void	handle_err1(char *str, t_env *env, t_cmd *cmd, int status_code);
void	handle_err2(char *str, t_env *env, t_cmd *cmd, int status_code);
int		is_special_char(char first_char);
t_list	*arrow_special_case(t_list *list, t_cmd *cmd, t_env *env);
int		write_heredoc(t_list *list, t_cmd *cmd, char *str);
//	- bultin_fd.c
int		get_output_fd(t_cmd *cmd, t_env *env, int i);
//	- external_cmd_fd.c
void	organize_fd(t_cmd *cmd, t_env *env, int i);

// src/util
//	- list.c
int		count_next_node(t_list *list);
t_list	*create_next_node(t_list *list, int count);
t_list	*create_extra_node(t_list *list);
int		is_nullstr_in_list(t_list *list, t_env *env);
void	free_list(t_list *list);
//	- string.c
char	**split_to_str(char const *s, char c);
int		count_num_of_str(char const *s, char c);
void	cust_free(char **res);
int		is_specialchar(char c);
//	- stderr.c
void	cust_perror(char *str, int status);
void	cust_write(char *str, int status);

// lib/get_next_line
//	- get_next_line.c
char	*get_next_line(int fd);

#endif
