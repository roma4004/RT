/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_white_spases.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/19 14:52:19 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/03 22:53:52 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void	ft_skip_white_spases(const char *str, size_t *i)
{
	while (str[*i] == '\t' || str[*i] == '\n' || str[*i] == '\r'
		|| str[*i] == ' ' || str[*i] == '\v' || str[*i] == '\f')
		(*i)++;
}
