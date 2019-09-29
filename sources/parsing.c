/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:30:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 15:27:45 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "main.h"

static double		after_dot(const char *str, double nbr)
{
	int		sign;
	int		i;
	double	res;

	sign = 1;
	if(nbr < 0)
		sign = -1;
	i = 0;
	while(str[i] && str[i] != '#' && ft_isdigit(str[i]))
		i++;
	res = ft_atoi(str);
	if (res != 0)
	{
		while (--i > -1)
			res /= 10;
	}
	return(res * sign);
}

static void			get_value_from_line(t_env *env, t_list *lst, int type)
{//check all algo
	int				i;
	int				j;
	const size_t	len = lst->content_size;
	const char		*str = lst->content;
	double			arr[VALUES_PER_OBJ];

	i = -1;
	j = 0;
	while (++i < len && str[i] != '#')
	{
		while (i < len && str[i] != '#'
		&& !ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')  //check valid
			i++;
		if (i > 0 && str[i - 1] == '.' && (j > 0))
			arr[j-1] += after_dot(&str[i], arr[j-1]);
		else
		{
			arr[j] = ft_atoi(&str[i]);
			j++;
		}
		if (str[i] == '+' || str[i] == '-')
			i++;
		while(ft_isdigit(str[i]))
			i++;
	}
	set_value(env, arr, type);
}

static bool		parse_lst(t_env *env, t_list *lst)
{
	t_list		*cur;
	size_t		nums;

	if (env->err_id || !init_obj_arr(env, lst))
		return (false);
	cur = lst;
	while (cur)
	{
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

t_env			*parse_scene(t_env *env, char *file_name)
{
	const int		fd = open(file_name, O_RDONLY);
	int				status;
	int				rows;
	char			*buf;
	t_list			*lst;

	if (!env || !file_name || (lst = NULL) || (rows = 0)
	|| ((fd == -1 || errno == ITS_A_DIRECTORY) && (env->err_id = READ_ERR)))
		return (NULL);
	while ((status = get_next_line(fd, &buf)) == 1
	&& is_valid_line(env, buf, ft_strlen(buf))
	&& (ft_lstappend(&lst, buf, ft_strlen(buf) + 1)) && ++rows)
	{
		ft_memdel((void *)&buf);
		if ((rows > MAX_MAP_SIDE && (env->err_id = MAP_SIZE_ERR)))
			break ;
	}
	if (buf)
		ft_memdel((void *)&buf);
	if (status == -1 || !lst || close(fd))
		env->err_id = READ_ERR;
	if (!parse_lst(env, lst) || ft_destroy_lst(lst))
		return (NULL);
	return (env);
}
