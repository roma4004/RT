/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 18:00:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 12:59:17 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>

# include "libft.h"

# define BUFF_SIZE 8192
# define MAX_FD 10000
# define MAX_LINE 10000

int		get_next_line(const int fd, char **line);

#endif
