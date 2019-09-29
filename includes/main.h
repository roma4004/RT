/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 20:59:29 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# define WIN_NAME "RTv1 by dromanic (@Dentair)"
# define WIN_WIDTH 1000u
# define WIN_HEIGHT 1000u
# define VIEWPORT_SIZE 1.0
# define DISTANCE_TO_PLANE 1.0
# define VALUES_PER_OBJ 11
# define OBJ_TYPE_MAX 7
# define DEBUG 0
# define MAX_MAP_SIDE 10000

# include <fcntl.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdlib.h>
# include <errno.h>
# include <math.h>

# include "SDL.h"
# include "libft.h"
# include "get_next_line.h"

typedef struct		s_vector3_double
{
	double		x;
	double		y;
	double		z;
}					t_dvec3;

typedef struct		s_vector_double
{
	double		x;
	double		y;
}					t_dvec;

typedef struct		s_universal_object
{
	unsigned	type;
	t_dvec3		pos;
	double		radius;
	t_dvec3		dir;
	t_dvec3		diffuse_color;
	double		specular;
}					t_uni;

typedef struct		s_cone
{
	unsigned	type;
	t_dvec3		pos;
	double		angle;
	t_dvec3		dir;
	t_dvec3		diffuse_color;
	double		specular;
}					t_cone;

typedef struct		s_light {
	unsigned	type;
	t_dvec3		pos;
	double		intensity;
}					t_lght;

typedef struct		s_light_calculating
{
	t_lght		*cur;
	t_dvec3		dir;
	double		defuse_val;
	double		t_max;
	double		specul_val;
	t_dvec3		touch_point;
	t_dvec3		obj_normal;
	t_dvec3		view;
}					t_lght_comp;

typedef struct		s_canvas_parameter
{
	t_dvec			half;
	double			rate;

}					t_canvas_par;

typedef struct		s_camera
{
	double			move_speed;
	double			rotate_speed;
	double			t_min;
	double			t_max;
	t_dvec3			dir;
	t_dvec3			pos;
	t_dvec3			rotate_angle;
	t_canvas_par	canvas;
}					t_cam;

typedef struct		s_flags
{
	bool	is_rtv1_over;

	bool	is_move_x_less;
	bool	is_move_x_more;

	bool	is_move_y_more;
	bool	is_move_y_less;

	bool	is_move_z_more;
	bool	is_move_z_less;

	bool	is_rotate_x_more;
	bool	is_rotate_x_less;

	bool	is_rotate_y_more;
	bool	is_rotate_y_less;

	bool	is_rotate_z_more;
	bool	is_rotate_z_less;
}					t_flags;

typedef struct		s_environment
{
	Uint32			err_id;
	Uint32			buff[WIN_HEIGHT][WIN_WIDTH];
	t_flags			flags;
	t_cam			cam;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	SDL_Surface		*surface;
	t_uni			*uni_arr;
	size_t			uni_arr_len;
	t_lght			*light_arr;
	size_t			light_arr_len;
	t_dvec3			bg_color;
	double			epsilon;
	t_list			*lst;
}					t_env;

enum				e_errors
{
	SCENE_ERR = 404,
	READ_ERR = 405,
	SCENE_SIZE_ERR = 406,
	ITS_A_DIRECTORY = 21,
};

enum				e_light_type
{
	AMBIENT = 0,
	POINT = 1,
	DIRECTIONAL = 2,
	SPHERE = 3,
	PLANE = 4,
	CYLINDER = 5,
	CONE = 6,
	CAM = 7,
};

t_env				*init_env(void);
bool				init_obj_arr(t_env *env, t_list *lst);

int					count_number(t_env *env, char *str, size_t len);
size_t				get_type(const char *str);
bool				is_valid_line(t_env *env, char *line, int len);
void				set_value(t_env *env, const double *v, size_t type);
t_env				*parse_scene(t_env *env, char *file_name);

bool				event_handler(t_cam *cam, t_flags *flags);
void				rerender_scene(t_env *env);

void				get_light(t_env *env, t_lght_comp *l,
								t_uni *obj, t_dvec3 *col);

void				send_ray(t_env *env, t_cam *cam, t_dvec3 *color);
t_uni				*intersect_obj(t_env *env, t_cam *cam, double *dist);
const t_uni			*is_shadow_ray(t_env *env, t_dvec3 *ray_pos,
									t_dvec3 direction, t_dvec limits);

void				discriminant_comput(t_dvec3 *tmp, t_dvec3 *plane_toch);
double				vec3_length(t_dvec3 vec);
uint8_t				double_clamp(double x);
t_dvec3				double_mul_vec3_col(double first, t_dvec3 second);
t_dvec3				vec3_add_vec3_col(t_dvec3 first, t_dvec3 second);

void				intersect_sphere(t_dvec3 *ray_pos, const t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_plane(t_dvec3 *ray_pos, const t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_cylinder(t_dvec3 *ray_pos, const t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_cone(t_dvec3 *ray_pos, const t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);

void				rotate_cam(t_dvec3 *dir, t_dvec3 *rotate_angle);

double				vec3_dot_vec3(t_dvec3 first, t_dvec3 second);
double				vec3_to_double(t_dvec3 first);
t_dvec3				vec3_normalize(t_dvec3 first);

t_dvec3				vec3_add_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_sub_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_mul_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_div_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_cross_vec3(t_dvec3 first, t_dvec3 second);

t_dvec3				double_add_vec3(double first, t_dvec3 second);
t_dvec3				double_sub_vec3(double first, t_dvec3 second);
t_dvec3				double_mul_vec3(double first, t_dvec3 second);
t_dvec3				double_div_vec3(double first, t_dvec3 second);

t_dvec3				vec3_add_double(t_dvec3 first, double second);
t_dvec3				vec3_sub_double(t_dvec3 first, double second);
t_dvec3				vec3_mul_double(t_dvec3 first, double second);
t_dvec3				vec3_div_double(t_dvec3 first, double second);
void				quit_program(t_env *env);

#endif
