/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 20:35:39 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 14:22:29 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	len;

	i = ~0ul;
	if (*needle == '\0')
		return ((char *)haystack);
	len = ft_strlen(needle);
	while (haystack[++i])
		if (haystack[i] == *needle && ft_memcmp(haystack + i, needle, len) == 0)
			return ((char *)(haystack + i));
	return (NULL);
}
