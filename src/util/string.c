/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:06:01 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/19 17:50:47 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_specialchar(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\0')
		return (1);
	if (c == '>')
		return (1);
	if (c == '<')
		return (1);
	return (0);
}

int	count_num_of_str(char const *s, char c)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	if (s == NULL)
		return (res);
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			res++;
			while (s[i++] != c && s[i] != '\0')
				continue ;
		}
		else
			i++;
	}
	return (res);
}

void	cust_free(char **res)
{
	int	i;

	i = 0;
	while (res[i] != NULL)
		free(res[i++]);
}

void	fill_str_in_array(char const *s, char c, char **res)
{
	int	i;
	int	k;
	int	l;
	int	start;

	i = 0;
	k = -1;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] != c && s[i] != '\0')
				i++;
			res[++k] = (char *) malloc((i - start + 1) * sizeof(char));
			if (res[k] == NULL)
				return (cust_free(res));
			l = 0;
			while ((i - start) > 0)
				res[k][l++] = s[start++];
			res[k][l] = '\0';
		}
		else
			i++;
	}
}

char	**split_to_str(char const *s, char c)
{
	int		num_of_strings;
	char	**res;

	if (s == NULL)
		return (NULL);
	num_of_strings = count_num_of_str(s, c);
	res = (char **) malloc((num_of_strings + 1) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	fill_str_in_array(s, c, res);
	res[num_of_strings] = NULL;
	return (res);
}
