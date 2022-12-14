/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:05:05 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/19 17:53:13 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_str(char *original_str, t_list *list, t_env *env, t_parse *parse)
{
	t_fill	fill;

	fill.i = parse->i;
	fill.start = parse->start;
	fill.count = 0;
	fill.list = list;
	fill.current_quote = 0;
	fill.end_pos_of_dollar_var = 0;
	fill.tmp_str = NULL;
	while ((fill.i - fill.start) > 0)
	{
		if (is_include_cmd(original_str, &fill) == 0)
			break ;
		if (original_str[fill.start] == '>' || original_str[fill.start] == '<')
			fill_al(original_str, &fill);
		else
			fill_str(original_str, &fill, parse, env);
		free(fill.tmp_str);
		if (parse->first_node == NULL)
			return ;
		fill.count++;
	}
	parse->i++;
	parse->start = parse->i;
}

void	create_node_with_str(char *original_str, t_env *env, t_parse *parse)
{
	t_list	*list;

	while (original_str[parse->i] != '\0')
	{
		check_quote_condition(parse, original_str[parse->i]);
		if (original_str[parse->i] == '|' && parse->is_quote_closed == 1)
		{
			check_pipe_condition(original_str, parse, env);
			if (parse->pipe_condition == 1)
				break ;
			else if (parse->pipe_condition == 2)
				return ;
			list = create_next_node(list, parse->count);
			if (parse->count++ == 0)
				parse->first_node = list;
			handle_str(original_str, list, env, parse);
		}
		else
			parse->i++;
	}
	list = create_next_node(list, parse->count);
	if (parse->count == 0)
		parse->first_node = list;
	handle_str(original_str, list, env, parse);
}

t_list	*parse(char *original_str, t_env *env)
{
	t_parse	parse;

	parse.first_node = NULL;
	parse.i = 0;
	parse.start = 0;
	parse.count = 0;
	parse.num_of_single_quote = 0;
	parse.num_of_double_quote = 0;
	parse.is_quote_closed = 1;
	parse.pipe_condition = 0;
	if (original_str == NULL || original_str[0] == '\0')
		return (NULL);
	create_node_with_str(original_str, env, &parse);
	if (is_nullstr_in_list(parse.first_node, env) == 1)
		return (NULL);
	return (parse.first_node);
}
