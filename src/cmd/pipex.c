#include "../minishell.h"

int	wexitstatus(int status)
{
	return (((status) & 0xff00) >> 8);
}

void	cust_waitpid(int num_of_executed_cmd)
{
	while (num_of_executed_cmd-- > 0)
	{
		if (waitpid(-1, NULL, 0) == -1)
			cust_perror("Error(cust_waitpid)", 1);
	}
}

void	exec_cmd(t_cmd_param *cmd_p, t_env_param *env_p, int i)
{
	if (ft_strlen(cmd_p->exec_args[0]) == 4 && ft_strncmp(cmd_p->exec_args[0], "exit", 4) == 0) //TODO: consider ft_strncmp
		exit(0); //TODO: check which status code I should return.
	if (ft_strlen(cmd_p->exec_args[0]) == 4 && ft_strncmp(cmd_p->exec_args[0], "echo", 4) == 0)
		exec_echo(cmd_p, env_p, i);
	else if (ft_strlen(cmd_p->exec_args[0]) == 2 && ft_strncmp(cmd_p->exec_args[0], "cd", 2) == 0)
		exec_cd(cmd_p, env_p, i);
	else if (ft_strlen(cmd_p->exec_args[0]) == 3 && ft_strncmp(cmd_p->exec_args[0], "pwd", 3) == 0)
		exec_pwd(cmd_p, env_p, i);
	else if (ft_strlen(cmd_p->exec_args[0]) == 6 && ft_strncmp(cmd_p->exec_args[0], "export", 6) == 0)
		exec_export(cmd_p, env_p, i);
	else if (ft_strlen(cmd_p->exec_args[0]) == 5 && ft_strncmp(cmd_p->exec_args[0], "unset", 5) == 0)
		exec_unset(cmd_p, env_p, i);
	else if (ft_strlen(cmd_p->exec_args[0]) == 3 && ft_strncmp(cmd_p->exec_args[0], "env", 3) == 0)
		exec_env(cmd_p, env_p, i);
	else
		exec_external_cmd(cmd_p, env_p, i);
}

void	config_execargs(t_list *list, t_cmd_param *cmd_p, t_env_param *env_p, int i)
{
	int	k;

	k = 0;
	while (list != NULL)
	{
		list = check_arrows(list, cmd_p, env_p, i);
		if (list == NULL)
			break;
		cmd_p->exec_args[k++] = list->str;
		list = list->extra;
	}
	cmd_p->exec_args[k] = NULL;
	cmd_p->num_of_args = k;
}

void	pipex(t_list *list, t_env_param *env_p)
{
	t_cmd_param	*cmd_p;
	char		*pathenv;
	int			i;

	cmd_p = (t_cmd_param *) malloc(sizeof(t_cmd_param));
	i = 0;
	while (list != NULL)
	{
		if (pipe(env_p->p[i]) < 0)
			cust_perror("Error(pipex)", 1);;
		cmd_p->input_fd = 0;
		cmd_p->output_fd = 1;
		cmd_p->is_heredoc = 0;
		cmd_p->pid = 0;
		config_execargs(list, cmd_p, env_p, i);
		if (cmd_p->num_of_args != 0)
			exec_cmd(cmd_p, env_p, i);
		list = list->next;
		i++;
	}
	if (cmd_p->pid != 0)
	{
		env_p->num_of_child--;
		if (waitpid(cmd_p->pid, &(env_p->status_code), 0) == -1)
			cust_perror("ERROR(last_cmd: waitpid)", 1);
	}
	env_p->status_code = wexitstatus(env_p->status_code);
	cust_waitpid(env_p->num_of_child);
}
