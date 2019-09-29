/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:30:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 19:51:52 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static double		after_dot(const char *str, double nbr)
{
	int		sign;
	int		i;
	double	res;

	sign = 1;
	if (nbr < 0)
		sign = -1;
	i = 0;
	while (str[i] && str[i] != '#' && ft_isdigit(str[i]))
		i++;
	res = ft_atoi(str);
	if (res != 0)
	{
		while (--i > -1)
			res /= 10;
	}
	return (res * sign);
}

static void			get_value_from_line(t_env *env, t_list *lst, int type)
{
	const size_t	len = lst->content_size;
	const char		*str = lst->content;
	size_t			i;
	size_t			j;
	double			arr[VALUES_PER_OBJ];

	i = UINT64_MAX;
	j = 0;
	while (++i < len && str[i] != '#')
	{
		while (i < len && str[i] != '#'
		&& !ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')
			i++;
		if (i > 0 && str[i - 1] == '.' && j > 0)
			arr[j - 1] += after_dot(&str[i], arr[j - 1]);
		else
			arr[j++] = ft_atoi(&str[i]);
		if (str[i] == '+' || str[i] == '-')
			i++;
		while (ft_isdigit(str[i]))
			i++;
	}
	set_value(env, arr, type);
}

static bool			parse_lst(t_env *env, t_list *lst)
{
	t_list		*cur;
	size_t		nums;

	if (env->err_id || !init_obj_arr(env, lst))
		return (false);
	cur = lst;
	while (cur)
	{
		if ((ft_strlen(cur->content) > MAX_MAP_SIDE
		&& (env->err_id = SCENE_SIZE_ERR)))
			return (false);
		nums = count_number(env, cur->content, cur->content_size);
		if (nums != 0 && nums != VALUES_PER_OBJ
		&& (env->err_id = SCENE_ERR))
			return (false);
		if (nums != 0)
			get_value_from_line(env, cur, get_type(cur->content));
		cur = cur->next;
	}
	return (true);
}

t_env				*parse_scene(t_env *env, char *file_name)
{
	const int		fd = open(file_name, O_RDONLY);
	int				status;
	int				rows;
	static char		*buf;

	if (!env || !file_name || (env->lst = NULL)
	|| (rows = 0) || ((fd == -1 || errno == ITS_A_DIRECTORY)
	&& (env->err_id = READ_ERR))
	|| (buf = NULL))
		return (NULL);
	while ((status = get_next_line(fd, &buf)) == 1
	&& is_valid_line(env, buf, ft_strlen(buf))
	&& (ft_lstappend(&env->lst, buf, ft_strlen(buf) + 1)) && ++rows)
	{
		ft_memdel((void *)&buf);
		if (((rows > MAX_MAP_SIDE) && (env->err_id = SCENE_SIZE_ERR)))
			break ;
	}
	if (buf)
		ft_memdel((void *)&buf);
	if (status == -1 || !env->lst || close(fd))
		env->err_id = READ_ERR;
	if (!parse_lst(env, env->lst) || ft_destroy_lst(env->lst))
		return (NULL);
	return (env);
}
