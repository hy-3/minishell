/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 17:34:44 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/19 17:01:44 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Convert string to integer.
 * 
 * @param str(const char *): Source string.
 * @return (int): Converted integer.
 */
int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	size_t	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' \
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while ('0' <= str[i] && str[i] <= '9' && str[i] != '\0')
	{
		if (res > LONG_MAX / 10 && sign < 0)
			return (0);
		if (res > LONG_MAX / 10 && sign > 0)
			return (-1);
		res = res * 10 + str[i++] - '0';
	}
	return ((int)(res * sign));
}
