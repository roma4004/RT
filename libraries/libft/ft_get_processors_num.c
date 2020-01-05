/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_processors_num.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 12:51:00 by dromanic          #+#    #+#             */
/*   Updated: 2020/01/05 17:54:48 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

#ifdef linux

# include <sys/sysinfo.h>

#elif defined(__unix__) || defined(__APPLE__)

# include <sys/param.h>
# include <sys/sysctl.h>

#elif defined(_WIN32) || defined(WIN32)

# include <winbase.h>

#endif

#ifdef linux

int		ft_get_processors_num(void)
{
	return (sysconf(_SC_NPROCESSORS_ONLN));
}

#elif defined(__unix__) || defined(__APPLE__)

int		ft_get_processors_num(void)
{
	int		nm[2];
	int		count;
	size_t	len;

	len = 4;
	nm[0] = CTL_HW;
	nm[1] = HW_AVAILCPU;
	sysctl(nm, 2, &count, &len, NULL, 0);
	if (count < 1)
	{
		nm[1] = HW_NCPU;
		sysctl(nm, 2, &count, &len, NULL, 0);
		if (count < 1)
			count = 1;
	}
	return (count);
}

#elif defined(_WIN32) || defined(WIN32)

int		ft_get_processors_num(void)
{
	SYSTEM_INFO		sysinfo;

	GetSystemInfo(&sysinfo);
	return (sysinfo.dwNumberOfProcessors);
}

#endif
