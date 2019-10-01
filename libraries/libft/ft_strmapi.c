/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 20:55:19 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/02 20:24:01 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*alloc;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	if ((alloc = ft_strnew(ft_strlen(s))))
	{
		i = ~0u;
		while (s[++i])
			alloc[i] = f(i, s[i]);
	}
	return (alloc);
}
