/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 12:20:20 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/25 12:28:41 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	after_dot(const char *str, size_t i)
{
	double	result;

	result = 0;
	while (str[i] != '\0')
		i++;
	i--;
	while (ft_isdigit(str[i]))
	{
		result = result * 0.1 + (str[i] - '0');
		i--;
	}
	return (result * 0.1);
}

double			ft_atod(const char *str)
{
	size_t	i;
	int		sign;
	double	result;

	i = 0;
	sign = 1;
	result = 0;
	ft_skip_white_spases(str, &i);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+'
	|| str[i] == '-')
		i++;
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.'
	&& ft_isdigit(str[i + 1]))
		result += after_dot(str, i);
	return (result * sign);
}
