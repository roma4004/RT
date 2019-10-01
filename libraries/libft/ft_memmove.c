/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 21:28:02 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 14:03:55 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char		*ch_src = (const unsigned char *)src;
	unsigned char			*ch_dst;
	size_t					i;

	ch_dst = (unsigned char *)dst;
	i = ~0ul;
	if (src < dst)
		while (len--)
			ch_dst[len] = ch_src[len];
	else
		while (++i < len)
			ch_dst[i] = ch_src[i];
	return (dst);
}
