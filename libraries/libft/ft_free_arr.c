/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 20:14:10 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/03 16:31:07 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_free_arr(char **arr)
{
	size_t	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(&arr);
}
