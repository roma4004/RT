/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 12:41:00 by dromanic          #+#    #+#             */
/*   Updated: 2019/01/01 19:07:11 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_pow(double num, unsigned exp)
{
	double	result;

	result = 1.0;
	while (exp)
	{
		if (exp & 1u)
			result *= num;
		exp >>= 1u;
		num *= num;
	}
	return (result);
}
