/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 21:58:00 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/03 17:47:14 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char		*ch_src = (const unsigned char *)src;
	size_t					i;
	unsigned char			*ch_dst;

	ch_dst = (unsigned char *)dest;
	i = ~0ul;
	while (++i < n)
		ch_dst[i] = ch_src[i];
	return (dest);
}
