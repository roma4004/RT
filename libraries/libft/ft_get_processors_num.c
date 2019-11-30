/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_processors_num.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 12:51:00 by dromanic          #+#    #+#             */
/*   Updated: 2018/11/03 14:45:47 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <winbase.h>
#include "libft.h"

/*
** on macOS detecting number of processors on machine:
** <sys/param.h>, <sys/sysctl.h> needed for int func get_processors_num();
*/

/*
** on linux detecting number of processors on machine:
** #include <sys/sysinfo.h>
** and using:
**	{
**		return (sysconf(_SC_NPROCESSORS_ONLN));
**	}
*/

/*
** on win32 detecting number of processors on machine:
** #include <winbase.h>
**	{
**		SYSTEM_INFO		sysinfo;
**
**		GetSystemInfo(&sysinfo);
**		return (sysinfo.dwNumberOfProcessors);
**	}
*/

int		ft_get_processors_num(void)
{
	SYSTEM_INFO		sysinfo;

	GetSystemInfo(&sysinfo);
	return (sysinfo.dwNumberOfProcessors);
}
