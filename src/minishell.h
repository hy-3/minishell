#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"
# include <sys/uio.h>
# include <stddef.h>

# define BUFFER_SIZE 10

typedef struct s_list
{
	char			*str;
	struct s_list	*extra;
	struct s_list	*next;
}	t_list;

typedef struct s_env_param
{
	char	**first_envp;
	char	**current_envp;
	char	*pathenv;
}	t_env_param;

typedef struct s_cmd_param
{
	char	*exec_args[100]; //TODO: *exec_args[ARG_MAX];
	int		input_fd;
	int		output_fd;
	int		is_heredoc;
	int		p[100][2]; //TODO: p[ARG_MAX][2];
	int		pid;
	int		num_of_child;
	int		status_code; //TODO: implement
}	t_cmd_param;

typedef struct s_res_arrow
{
	int		start;
	t_list	*list;
}	t_res_arrow;

//	src/parse.c
t_list	*parse(char *original_str);
//	src/env.c
char	**copy_env(char **old_envp);
int		is_exist_in_env(char **envp, char *str);
int		is_valid_envname(char *str);
char	**create_new_env_with_str(char **old_envp, char *str);
char	**upd_to_new_env(t_env_param *env_p, t_cmd_param *cmd_p);
//	src/cmd/pipex.c
int		pipex(t_list *list, t_env_param *env_p);
//	src/cmd/cmds.c
void	exec_basedon_cmdtype(t_cmd_param *cmd_p, t_env_param *env_p, int num_node_ver, int num_node_hor, int i);
//	src/cmd/check_cmd.c
char	*get_value_of_pathenv(char **envp);
char	*is_cmd_exist_and_executable(char *path_env, char *cmd);
//	src/cmd/list.c
int		list_iter(t_list *list, t_env_param *env_p, int num_node_hor, int (*f)(t_list *list, t_env_param *env_p, int i, int num_node_hor));
int		count_next_node(t_list *list);
int		count_extra_node(t_list *list);
//	src/cmd/stderr.c
void	cust_perror(char *str, int status);
void	cust_write(char *str, int status);
//	src/cmd/split_to_str.c
char	**split_to_str(char const *s, char c);
int		count_num_of_str(char const *s, char c);
void	cust_free(char **res);
//	src/builtin/*.c
void	exec_cd(t_cmd_param *cmd_p, int num_node_ver);
void	exec_echo(t_cmd_param *cmd_p, int num_node_ver, int num_node_hor, int i);
void	exec_env(t_cmd_param *cmd_p, t_env_param *env_p, int num_node_ver);
void	exec_export(t_cmd_param *cmd_p, t_env_param *env_p, int num_node_ver);
void	exec_pwd();
void	exec_unset(t_cmd_param *cmd_p, t_env_param *env_p, int num_node_ver);
//	lib/get_next_line/get_next_line.c
char	*get_next_line(int fd);

#endif