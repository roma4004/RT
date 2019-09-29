/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:30:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 11:25:41 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "main.h"

static int	cmp_from_begin(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i]
	&& (s1[i] == s2[i]))
		i++;
	if (i == ft_strlen(s1))
		return (1);
	return (0);
}

static int		count_number(t_env *env, char *str, size_t len)
{
	int		i;
	int		digits;
	int		dots;

	i = -1;
	digits = 0;
	while(++i < len && str[i] != '#')
	{
		if (ft_isdigit(str[i]) && !(dots = 0) && ++digits)
		{
			while(i < len && str[i] != '#'
			&& (ft_isdigit(str[i]) || str[i] == '.'))
			{
				if (str[i] == '.')
					dots++;
				i++;
			}
			if (dots > 1)
				env->err_id = SCENE_ERR;
		}
	}
	return(digits);
}

static size_t		get_type(const char *str)
{
	size_t		type;

	if (str[0] == '#')
		return (UINT64_MAX);
	type = 0;
	if (cmp_from_begin("sphere", str) || cmp_from_begin("SPHERE", str))
		type = SPHERE;
	else if(cmp_from_begin("plane", str) || cmp_from_begin("PLANE", str))
		type = PLANE;
	else if(cmp_from_begin("cone", str) || cmp_from_begin("CONE", str))
		type = CONE;
	else if(cmp_from_begin("cylinder", str) || cmp_from_begin("CYLINDER", str))
		type = CYLINDER;
	else if(cmp_from_begin("cam", str) || cmp_from_begin("CAM", str))
		type = CAM;
	return (type);
}

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

static void			lst_write_value(t_env *env, const double *v, int type)
{
	static size_t	id_uni = 0;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == 7)
	{
		env->cam.pos = (t_dvec3){v[0], v[1], v[2]};
		env->cam.rotate_angle = (t_dvec3){v[4], v[5], v[6]};
	}
	else if (type < 3)
		env->light_arr[id_lgh++] =
			(t_lght){type, (t_dvec3){v[0], v[1], v[2]}, v[3]};
	else
		env->uni_arr[id_uni++] =
			(t_uni){type, (t_dvec3){v[0], v[1], v[2]}, v[3],
				(t_dvec3){v[4], v[5], v[6]}, (t_dvec3){v[7], v[8], v[9]}, v[10]
				};
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
		{ arr[j] = ft_atoi(&str[i]);j++; }
		if(str[i] == '+' || str[i] == '-')
			i++;
		while(ft_isdigit(str[i]))
			i++;
	}for(int k = 0; k < VALUES_PER_OBJ; k++) printf("%.2f\t\t", arr[k]); printf("\n");
	lst_write_value(env, &arr[0], type);
}

void				cnt_obj_type(t_env *env, t_list *lst)
{
	size_t		*lights = &env->light_arr_len;
	size_t		*other = &env->uni_arr_len;
	t_list		*cur;
	size_t		type;

	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type < 3)
				(*lights)++;
			else if (type < OBJ_TYPE_MAX)
				(*other)++;
		}
		cur = cur->next;
	}
}

bool		init_obj_arr(t_env *env, t_list *lst)
{
	cnt_obj_type(env, lst);
	if ((env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * env->uni_arr_len))
//	&& (env->light_arr = (t_lght *)malloc(sizeof(t_lght) * env->light_arr_len))
	)
		return (true);
	else
	{
		if (env->uni_arr)
			ft_memdel((void **)&env->uni_arr);
		if (env->light_arr)
			ft_memdel((void **)&env->light_arr);
	}
	return (false);
}

void		parse_lst(t_env *env, t_list *lst)
{
	t_list		*cur;
	size_t		nums;

	if (!init_obj_arr(env, lst))
		return ;
	cur = lst;
	while (cur)
	{
		nums = count_number(env, cur->content, cur->content_size);
		if (nums != 0 && nums != VALUES_PER_OBJ
		&& (env->err_id = SCENE_ERR))
			return ;
		if (nums != 0)
			get_value_from_line(env, cur, get_type(cur->content));
		cur = cur->next;
	}
}

static int		is_dot_in_double(char *str, int i)
{
	if (str
		&& i != 0
		&& str[i] == '.'
		&& ft_isdigit(str[i + 1])
		&& ft_isdigit(str[i - 1]))
		return (1);
	return (0);
}

static bool		is_valid_line(t_env *env, char *line, int len)
{
	int		i;

	if (!line)
		return (false);
	i = -1;
	while (++i < len && line[i] != '#')
	{
		if (!(ft_strchr(" \t|", line[i])
			|| ft_isalpha(line[i])
			|| ft_isdigit(line[i])
			|| is_dot_in_double(line, i)
			|| ((line[i] == '-' || line[i] == '+') && ft_isdigit(line[i + 1]))))
		{
			env->err_id = SCENE_ERR;
			return (false);
		}
	}
	return (true);
}
#include <stdio.h>
void		print_list(t_list *lst)
{
	t_list		*cur;

	cur = lst;
	while (cur)
	{
		printf("==%s\n",cur->content);
		cur = cur->next;
	}
}

t_env			*parse_scene(t_env *env, char *file_name)
{
	int				status;
	const int		fd = open(file_name, O_RDONLY);
	int				rows;
	char			*buf;
	t_list			*lst;

	if (!env || !file_name || (lst = NULL) || (rows = 0)
		|| ((fd == -1 || errno == ITS_A_DIRECTORY) && (env->err_id = READ_ERR)))
		return (NULL);
	while ((status = get_next_line(fd, &buf)) == 1
	&& is_valid_line(env, buf, ft_strlen(buf)
	&& (ft_lstappend(&lst, buf, ft_strlen(buf) + 1)) && ++rows))
	{
		printf("%s", buf);
		printf("\n");
		ft_memdel((void *)&buf);
		if ((rows > MAX_MAP_SIDE && (env->err_id = MAP_SIZE_ERR)))
			break ;
	}	//print_list(lst);
	//cl_stat = 0;
	if (status == -1
	|| !lst
	|| (status = errno != 2)
	|| !(status = close(fd)))
		env->err_id = READ_ERR;

	parse_lst(env, lst);
//	if (get_map_param(env, &env->map, lst) || env->err_id
//		|| convert_to_map(lst, &env->map) || ft_destroy_lst(lst)
//		|| find_player_map_repair(env, &env->cam.pos, env->map.tex_id,
//			(Uint32)DEF_EDGE_TEX < (Uint32)TEXTURES ? (Uint32)DEF_EDGE_TEX : 1)
//		|| print_map(&env->map, env->map.tex_id)
//		|| ((!(bool)env->cam.pos.x
//			|| !(bool)env->cam.pos.y) && (env->err_id = SPACE)))
//		return (NULL);
	return (env);
}
