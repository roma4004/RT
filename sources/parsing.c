/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:30:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 19:52:54 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "main.h"

static t_fgrs	*malloc_fgrs(t_fgrs *fgr)
{
	t_fgrs *tmp;

	if (!fgr)
	{
		if (!(fgr = (t_fgrs *)malloc(sizeof(t_fgrs))))
		{
			print_error(ERR_MALLOC, -1);
		}
		else
		{
			fgr->type = F_NON;
			fgr->center = (t_dvec3) {0, 0, 0};
			fgr->radius = 0;
			fgr->color = (t_clr) {255, 255, 255, 255};
			fgr->specul = 0;
			fgr->next = NULL;
			return (fgr);
		}
	}
	else
	{
		tmp = (t_fgrs *)malloc(sizeof(t_fgrs));
		if (!tmp)
		{
			print_error(ERR_MALLOC, -1);
		}
		else
		{
			tmp->type = F_NON;
			tmp->center = (t_dvec3) {0, 0, 0};
			tmp->radius = 0;
			tmp->color = (t_clr) {255, 255, 255, 255};
			tmp->specul = 0;
			tmp->next = fgr;
			return (tmp);
		}
	}
}

static int	cmp_begin(const char *s1, const char *s2)
{
	int	len;
	int	i;

	len = ft_strlen(s1);
	i = 0;
	while (s1[i] != '\0' && (s1[i] == s2[i]))
		i++;
	if (i == len)
		return (1);
	return (0);
}

static int		count_number(char *str, int len)
{
	int i;
	int nb;
	int dot;

	i = 0;
	nb = 0;
	while(i < len && str[i] != '#')
	{
		if (ft_isdigit(str[i]))
		{
			nb++;
			dot = 0;
			while(i < len && str[i] != '#'
				&& (ft_isdigit(str[i]) || str[i] == '.'))
			{
				if (str[i] == '.')
					dot++;
				i++;
			}
		}
		i++;
	}
	return(nb);
}

static 	int check_type_count_nbrs(char *str, int len, t_list *read, int *nb)
{
	int		let_c;
	int		type;

	let_c = 0;
	type = 0;
	while(let_c < len && str[let_c] != '#')
		let_c++;
	if (let_c == 0)
		return (0);

	// Чекаємо який тип фігури
	if (cmp_begin("sphere", str) == 1) //str_n_str
		type = SPHERE;
	else if(cmp_begin("plane", str) == 1)
		type = PLANE;
	else if(cmp_begin("cone", str) == 1)
		type = CONE;
	else if(cmp_begin("cylinder", str) == 1)
		type = CYLINDER;


	*nb = count_number(str, len);

	//	Чекаємо чи співпадає коректна кількість чисел для кожного типу фігур
//	if (*nb != N_SPHR_NBR)
//		print_error(ERR_NBR, read->row);
	//	else if (type == PLANE && *nb != N_PLN_NBR)
	//		print_error(ERR_NBR, lst->row);
	//	else if (type == CONE && *nb != N_CONE_NBR)
	//		print_error(ERR_NBR, lst->row);
	//	else if (type == CYLINDER && *nb != N_CLDR_NBR)
	//		print_error(ERR_NBR, lst->row);

	return (type);
}

static double after_dot(char *str, double nbr)
{
	int sign;
	double res = 0;

	sign = 1;
	if(nbr < 0)
		sign = -1;

	int i = 0;
	while(str[i] != '\0' && str[i] != '#' && ft_isdigit(str[i]))
		i++;
	res = ft_atoi(str);
	if (res != 0)
	{
		while (--i > -1)
			res /= 10;
	}
	return(res * sign);
}

static void check_write_value(double *arr, int nb, t_fgrs *fgr, int type)
{
	(void)nb;
	t_clr	color;
	t_dvec3	pos;

	if (type == SPHERE)
	{
		pos = (t_dvec3) {arr[0], arr[1], arr[2]};
		color = (t_clr) {arr[4], arr[5], arr[6], 255};
		*fgr = (t_fgrs) {SPHERE, pos, arr[3], color, arr[7], fgr->next};
	}
	//	else if (type == CONE)
	//	{
	//
	//	}
	//	else if (type == PLANE)
	//	{
	//
	//	}
	//	else if (type == CYLINDER)
	//	{
	//
	//	}
}

static void get_value_from_line(t_list	*read, t_fgrs *fgr, int type, int nb)
{
	int		i;
	int		j;
	int		len;
	char	*str;
	double	arr[nb];


	i = -1;
	j = 0;
	len = read->content_size;
	str = read->content;

	while (++i < len && str[i] != '#')
	{
		while (i < len  && str[i] != '#'
			&& !ft_isdigit(str[i]) && str[i] != '-')
			i++;
		if (i > 0 && str[i - 1] == '.' && (j > 0))
			arr[j-1] += after_dot(&str[i], arr[j-1]);
		else
		{
			arr[j] = ft_atoi(&str[i]);
			j++;
		}
		if(str[i] == '-')
			i++;
		while(ft_isdigit(str[i]))
			i++;
	}
	for(int k = 0; k < nb; k++)
	{
		printf("%.2f\t\t", arr[k]);
	}
	printf("\n");
	check_write_value(&arr[0], nb, fgr, type);
}

void read_data(t_env *env)
{

	t_list	*read;
	int	n=0;
	int	nb;
	int	type;

	read = env->lst;
	while (read)
	{
		type = 0;
		nb = 0;
		type = check_type_count_nbrs(read->content, read->content_size, read, &nb);
		if (type != 0)
		{
			env->figures = malloc_fgrs(env->figures);     //append list
			get_value_from_line(read, env->figures, type, nb);
			n++;
		}
		read = read->next;
	}
	printf("%d - cells have been mallocated for figures list\n\n", n);
}