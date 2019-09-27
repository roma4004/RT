/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:31:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 19:54:56 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
//#include "main2.h"
//#include "color.h"
# define ERR_USAGE			0
# define ERR_MALLOC			1
# define ERR_SDL			2
# define ERR_ARGV			3
# define ERR_LEN			4
# define ERR_ROWS			5

# define ERR_WRONG_SYMBOL	9
# define ERR_PIPE			10
# define ERR_DOT			12
# define ERR_NBR			13
# define ERR_MINUS			14

# define ERR_LINE			15

# define MAX_LEN		200
# define MAX_ROWS		400
static t_list	*malloc_lst(t_list *read, char *str)
{
	if (!read)
	{
		if (!(read = (t_list *)malloc(sizeof(t_list))))
		{
			print_error(ERR_MALLOC, -1);
		}
		else
		{
			read->content = ft_strdup(str);
			read->content_size = ft_strlen(str);
			read->next = NULL;
			return (read);
		}
	}
	else
	{
		read->next = (t_list *)malloc(sizeof(t_list));
		if (!read->next)
		{
			print_error(ERR_MALLOC, -1);
		}
		else
		{
			read->next->content = ft_strdup(str);
			read->next->content_size = ft_strlen(str);
			read->next->next = NULL;
			return (read->next);
		}
	}
}

static int	check_dot(char *str, int i)
{
	if (i == 0)
		return (0);
	if (ft_isdigit(str[i-1]) && ft_isdigit(str[i+1]))
		return (1);
	return (0);
}


static void		check_line(char *line, int len, int row)
{
	int i;
	int pipe;

	i = 0;
	pipe = 0;
	while(i < len && line[i] != '#')
	{
		if (!ft_isdigit(line[i]) && !ft_isalpha(line[i])   /// char	*ft_strchr(const char *s, int c) or valid_sym func
			&& line[i] != ' ' && line[i] != '|'
			&& line[i] != '-' && line[i] != '.')//all valid symbol
			print_error(ERR_WRONG_SYMBOL, row);
		if (line[i] == '|')
			pipe++;
		if (line[i] == '.' && !(check_dot(line, i)))
			print_error(ERR_DOT, row);
		if(line[i] == '-' && !(ft_isdigit(line[i+1])))
			print_error(ERR_MINUS, row);
		i++;
	}
	if (pipe > PARSE_PARAM_CNT - 1)
		print_error(ERR_PIPE, row);
}

//t_env				*parse_map(char *file_name, t_env *env)
//{
//	int				status;
//	const int		fd = open(file_name, O_RDONLY);
//	int				rows;
//	char			*buf;
//	t_list			*lst;
//
//	if (!env || !file_name || (lst = NULL) || (rows = 0)
//		|| ((fd == -1 || errno == ITS_A_DIRECTORY) && (env->err_id = READ_ERR)))
//		return (NULL);
//	while ((status = get_next_line(fd, &buf)) == 1
//	&& (ft_lstappend(&lst, buf, ft_strlen(buf))) && ++rows)
//	{
//		ft_memdel((void *)&buf);
//		if (rows > MAX_MAP_SIDE && (env->err_id = MAP_SIZE_ERR))
//			break ;
//	}
//	if (status == -1 || lst == NULL || !(errno))
//		env->err_id = READ_ERR;
//	if (close(fd) || get_map_param(env, &env->map, lst) || env->err_id
//		|| convert_to_map(lst, &env->map) || ft_destroy_lst(lst)
//		|| find_player_map_repair(env, &env->cam.pos, env->map.tex_id,
//			(Uint32)DEF_EDGE_TEX < (Uint32)TEXTURES ? (Uint32)DEF_EDGE_TEX : 1)
//		|| print_map(&env->map, env->map.tex_id)
//		|| ((!(bool)env->cam.pos.x
//		|| !(bool)env->cam.pos.y) && (env->err_id = SPACE)))
//		return (NULL);
//	return (env);
//}
void	validation_file(t_env *env, int fd)
{
	int		status;
	char	*line;
	int		rows;
	t_list	*lst;

	rows = 0;
	lst = NULL;
	while ((status = get_next_line(fd, &line)) == 1
	&& ft_lstappend(&lst, line, ft_strlen(line)) && ++rows)
	{
		lst = malloc_lst(env->lst, line); //switch to my append_list and solve reverse
		if (lst->content_size > MAX_LEN)                //width limit
			print_error(ERR_LEN, rows);

printf("%s", lst->content);
printf("\n");

		check_line(lst->content, lst->content_size, rows);
		ft_strdel(&line);
	}
	if ((env->lst == NULL) || status == -1)
		print_error(ERR_ARGV, -1);
//	while (env->lst->prev != NULL)
//		env->lst = env->lst->prev;
}


