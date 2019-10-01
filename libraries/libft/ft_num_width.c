/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_num_width.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 20:01:54 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 14:29:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_num_width(long long int num)
{
	size_t	len;

	len = 0;
	if (num < 0)
		len++;
	while (++len && (num /= 10))
		;
	return (len);
}
