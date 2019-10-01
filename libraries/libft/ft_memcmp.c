/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 21:58:59 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 14:29:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *memptr1, const void *memptr2, size_t num)
{
	const unsigned char		*str1 = (const unsigned char *)memptr1;
	const unsigned char		*str2 = (const unsigned char *)memptr2;
	size_t					i;

	if (memptr1 == memptr2 || num == 0)
		return (0);
	i = 0;
	while (num--)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		if (num)
			i++;
	}
	return (0);
}
