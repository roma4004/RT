/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 22:22:01 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 18:52:13 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	ssize_t		sign;
	size_t		i;
	size_t		result;

	i = 0;
	ft_skip_white_spases(str, &i);
	sign = 1;
	result = 0;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
		result = result * 10 + (size_t)str[i++] - '0';
	if ((result > 9223372036854775807 && sign == 1)
	|| (ft_digits_len(str) >= 19 && sign == 1))
		return (-1);
	if (result > 9223372036854775807 && sign == -1)
		return (0);
	return ((int)((ssize_t)result * sign));
}
