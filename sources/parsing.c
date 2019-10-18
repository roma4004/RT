/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:30:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
	if (res != 0.0)
	{
		while (--i > -1)
			res /= 10;
	}
	return (res * sign);
}

static void			get_value_from_line(t_env *env, t_list *lst, size_t type)
{
	const size_t	len = lst->content_size;
	const char		*str = lst->content;
	size_t			i;
	size_t			num_id;
	double			arr[VALUES_PER_OBJ];

	ft_bzero(&arr, sizeof(double) * VALUES_PER_OBJ);
	if (type == UINT64_MAX)
		return ;
	i = UINT64_MAX;
	num_id = 0;
	while (++i < len && str[i] != '#')
	{
		while (i < len && str[i] != '#'
		&& !ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')
			i++;
		if (i > 0 && str[i - 1] == '.' && num_id > 0)
			arr[num_id - 1] += after_dot(&str[i], arr[num_id - 1]);
		else
		{
			arr[num_id] = ft_atoi(&str[i]);
			num_id++;
		}
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

	if (env->flags.err_id || !init_obj_arr(env, lst))
		return (false);
	cur = lst;
	while (cur)
	{
		if ((ft_strlen(cur->content) > MAX_MAP_SIDE
		&& (env->flags.err_id = SCENE_SIZE_ERR)))
			return (false);
		nums = count_number(env, cur->content, cur->content_size);
		if (nums != 0 && nums != VALUES_PER_OBJ
		&& (env->flags.err_id = SCENE_ERR))
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
	&& (env->flags.err_id = READ_ERR))
	|| (buf = NULL))
		return (NULL);
	while ((status = get_next_line(fd, &buf)) == 1
	&& is_valid_line(env, buf, ft_strlen(buf))
	&& (ft_lstappend(&env->lst, buf, ft_strlen(buf) + 1)) && ++rows)
	{
		ft_memdel((void *)&buf);
		if (((rows > MAX_MAP_SIDE) && (env->flags.err_id = SCENE_SIZE_ERR)))
			break ;
	}
	if (buf)
		ft_memdel((void *)&buf);
	if (status == -1 || !env->lst || close(fd))
		env->flags.err_id = READ_ERR;
	if (!parse_lst(env, env->lst) )//|| ft_destroy_lst(env->lst))
		return (NULL);
	return (env);
}
