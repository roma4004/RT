/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:31:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 15:35:11 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int		is_dot_in_double(char *str, int i)
{
	if (str
		&& i != 0
		&& ft_isdigit(str[i + 1])
		&& ft_isdigit(str[i - 1]))
		return (1);
	return (0);
}

bool		is_valid_line(t_env *env, char *line, int len)
{
	int		i;

	if (!line)
		return (false);
	i = -1;
	while (++i < len && line[i] != '#')
	{
		if
		(	!((line[i] == '+' && ft_isdigit(line[i + 1]))
			|| (line[i] == '-' && ft_isdigit(line[i + 1]))
			|| (line[i] == '.' && is_dot_in_double(line, i))
			|| ft_isalpha(line[i])
			|| ft_isdigit(line[i])
			|| ft_strchr(" \t|", line[i]))

//				||
//
		)
		{

			env->err_id = SCENE_ERR;
			return (false);
		}
	}
	return (true);
}

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

size_t		get_type(const char *str)
{
	size_t		type;

	if (str[0] == '#' || str[0] == ' ')
		return (UINT64_MAX);
	type = UINT64_MAX;
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
	else if(cmp_from_begin("ambient", str) || cmp_from_begin("AMBIENT", str))
		type = AMBIENT;
	else if(cmp_from_begin("point", str) || cmp_from_begin("POINT", str))
		type = POINT;
	else if(cmp_from_begin("directional", str)
		|| cmp_from_begin("DIRECTIONAL", str))
		type = DIRECTIONAL;
	return (type);
}

void		cnt_obj_type(t_env *env, t_list *lst)
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

int		count_number(t_env *env, char *str, size_t len)
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
