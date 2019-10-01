/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 22:00:54 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/02 19:27:30 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	const unsigned char	*ch_src = (const unsigned char *)src;
	unsigned char		*ch_dst;
	size_t				i;
	unsigned char		ch_sym;

	ch_sym = (unsigned char)c;
	ch_dst = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		ch_dst[i] = ch_src[i];
		if (ch_dst[i] == ch_sym)
			return (__extension__(dest + i + 1));
		i++;
	}
	return (NULL);
}
