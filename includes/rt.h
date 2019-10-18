/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:04:44 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# define WIN_NAME "RTv1 by dromanic (@Dentair)"
# define WIN_WIDTH 1000u
# define WIN_HEIGHT 1000u
# define VIEWPORT_SIZE 1.0
# define DISTANCE_TO_PLANE 1.0
# define VALUES_PER_OBJ 15
# define OBJ_TYPE_MAX 8
# define DEBUG 0
# define MAX_MAP_SIDE 10000

# include <fcntl.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdlib.h>
# include <errno.h>
# include <math.h>
# include <time.h>

# pragma GCC diagnostic ignored "-Wstrict-prototypes"
# pragma GCC diagnostic ignored "-Wpadded"
# pragma GCC diagnostic ignored "-Wdocumentation"
# pragma GCC diagnostic ignored "-Wundef"
# pragma GCC diagnostic ignored "-Wreserved-id-macro"
# include "SDL.h"
//# include "SDL_ttf.h"
# include "SDL_image.h"
//# include "SDL_mixer.h"
//# include "SDL_audio.h"
# pragma GCC diagnostic warning "-Wreserved-id-macro"
# pragma GCC diagnostic warning "-Wundef"
# pragma GCC diagnostic warning "-Wdocumentation"
# pragma GCC diagnostic warning "-Wpadded"
# pragma GCC diagnostic warning "-Wstrict-prototypes"

# include "libft.h"
# include "get_next_line.h"

typedef struct		s_vector3_int
{
	int				x;
	int				y;
	int				z;
	char			padding[4];
}					t_ivec3;

typedef struct		s_vector3_double
{
	double			x;
	double			y;
	double			z;
	double			padding;
}					t_dvec3;

typedef struct		s_vector_double
{
	double			x;
	double			y;
}					t_dvec;

typedef struct		s_canvas_parameter
{
	t_dvec			half;
	double			rate;
}					t_canvas_par;

typedef struct		s_ray
{
	double			t_min;
	double			t_max;
	t_dvec3			pos;
	t_dvec3			dir;
	t_dvec3			touch_point;
	double			reflect_coef;
	double			refract_coef;
	unsigned		dept_limit;
	char			padding[4];
	t_dvec3			normal;
}					t_ray;

typedef struct		s_camera
{
	double			t_min;
	double			t_max;
	double			epsilon;
	t_dvec3			pos;
	t_dvec3			pos_backup;
	double			move_speed;
	double			rotate_speed;
	t_dvec3			rotate_angle;
	t_dvec3			rotate_angle_backup;
	t_canvas_par	canvas;
	unsigned		reflective_dept;
	char			padding[4];
}					t_cam;

typedef struct		s_universal_object
{
	t_dvec3			pos;
	double			radius;
	t_dvec3			dir;
	double			height;
	t_dvec3			color;
	double			specular;
	void 			(*get_intersect)(const struct s_universal_object *,
										t_dvec3 *, t_ray *);
	void			(*get_normal)(t_ray *, const struct s_universal_object *,
									double, t_dvec3 *normal);
	double			reflective_coef;
	double			refractive_coef;
	double			transparency_coef;
	t_dvec3			pos_backup;
	double			radius_backup;
	t_dvec3			dir_backup;
	_Bool			is_selected;
//	t_obj			sliced_plane;
	char			padding[7];
	double			angle_cache; //todo: save here angle computs
}					t_uni;

typedef struct		s_light {
	t_dvec3			pos;
	double			intensity;
	size_t			type;
	t_dvec3			color;
}					t_lght;

typedef struct		s_vector3_comput_tmp
{
	t_dvec3		oc;
	t_dvec3		tc;
	double		dir;
	t_dvec3		dir_vec;
	double		m;
}					t_dvec3_comp;

typedef struct		s_light_calculating
{
	t_lght			*cur;
	t_dvec3			dir;
	double			defuse_val;
	double			t_max;
	double			specul_val;
//	t_dvec3			touch_point;
	t_dvec3			view;
//	t_ray			ray;
	double			vec_reflect_len;
	double			view_len;
//	t_dvec3			touch_point;
}					t_lght_comp;

typedef struct		s_flags
{
	t_ivec3			rotate;
	t_ivec3			move;
	Uint32			err_id;
	_Bool			is_rtv1_over;
	_Bool			is_in_select_mod;
	_Bool			is_reset;
	_Bool			is_sepia;
	_Bool			is_screenshot;
}					t_flags;

typedef struct		s_environment
{
	Uint32			buff[WIN_HEIGHT][WIN_WIDTH];
	t_flags			flags;
	t_cam			cam;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	t_uni			*uni_arr;
	t_uni			*selected_obj;
	size_t			uni_arr_len;
	t_lght			*light_arr;
	size_t			light_arr_len;
	t_dvec3			bg_color;
	t_list			*lst;

	size_t			threads;
	t_dvec3			origin_dir_x;
	t_dvec3			origin_dir_y;
	t_dvec3			origin_dir_z;
}					t_env;

typedef struct		s_pthread_data
{
	t_env			*env;
	size_t			id;
}					t_pth_dt;

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
	DISK = 7,


	CAM = 8, // the last one
};

enum				e_orient
{
	POS = 1,
	NEG = -1,
	NON = 0
};

t_env				*init_sdl2(t_env *env);
void				apply_sepia_filter(t_dvec3 *color);
void				send_selected_ray(t_env *env, t_ray *ray,
										t_uni **obj, double dist);

void				ft_clamp_in_range(double *dest, double value, double min, double max);
void				ft_clamp_in_range_vec(t_dvec3 *dest, double min,
											double max);
_Bool				is_x_move_up(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_y_move_up(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_z_move_up(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_x_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate);
_Bool				is_y_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate);
_Bool				is_z_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate);
_Bool				is_x_move_down(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_y_move_down(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_z_move_down(SDL_Keycode k, t_ivec3 *restrict move);
_Bool				is_x_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate);
_Bool				is_y_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate);
_Bool				is_z_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate);


t_env				*init_env(void);
_Bool				init_obj_arr(t_env *env, t_list *lst);

size_t				count_number(t_env *env, char *str, size_t len);
size_t				get_type(const char *str);
_Bool				is_valid_line(t_env *env, char *line, size_t len);
void				set_value(t_env *env, const double *v, size_t type);
t_env				*parse_scene(t_env *env, char *file_name);

t_dvec3				convert_to_viewport(double x, double y, double rate);
typedef void		(*intersect)(const t_uni *, t_dvec3 *, t_ray *);
intersect			intersect_catalog(size_t type);

typedef void		(*normal)(const t_uni *, t_dvec3 *, t_ray *);
void				(*normal_catalog(size_t type))
						(t_ray *, const t_uni *, double, t_dvec3 *);

_Bool				event_handler(t_env *env, t_cam *cam, t_flags *flags);
void				draw_scene(t_env *env, size_t threads);

void				get_light(t_env *env, t_lght_comp *l,
								t_uni *obj, t_dvec3 *col, t_ray *ray);

void				send_ray(t_env *env, t_ray *ray, t_dvec3 *cur_color);
//const t_uni			*is_shadow_ray(t_env *env, t_dvec3 *ray_pos,
//									t_dvec3 *direction, t_dvec limits);

const t_uni			*is_shadow_ray(t_env *env, t_dvec3 *touch_point,
									t_dvec3 *shadow_dir, double t_max, t_ray *ray);

void				discriminant_comput(t_dvec3 *tmp, t_dvec3 *touch);
void				vec3_length(double *destination,
								const t_dvec3 *restrict first);
double				double_clamp(double x);

void				double_mul_vec3_col(t_dvec3 *destination,
										double first,
										const t_dvec3 *restrict second);
void				vec3_add_vec3_col(t_dvec3 *destination,
										const t_dvec3 *restrict first,
										const t_dvec3 *restrict second);

void				get_intersect_sphere(const t_uni *sphere, t_dvec3 *touch,
											t_ray *ray);
void				get_intersect_plane(const t_uni *plane, t_dvec3 *touch,
										t_ray *ray);
void				get_intersect_cylinder(const t_uni *cylinder,
											t_dvec3 *touch, t_ray *ray);
void				get_intersect_cone(const t_uni *obj, t_dvec3 *touch,
										t_ray *ray);
void				get_intersect_disk(const t_uni *disk, t_dvec3 *touch,
										t_ray *ray);

void				get_normal_sphere(t_ray *ray, const t_uni *obj,
										double dist, t_dvec3 *normal);
void				get_normal_plane(t_ray *ray, const t_uni *obj,
									 double dist, t_dvec3 *normal);
void				get_normal_cylinder(t_ray *ray, const t_uni *obj,
										double dist, t_dvec3 *normal);
void				get_normal_cone(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal);

void				move_objects(t_env *env, t_dvec3 *move_dir,
									double move_speed);
void				move_camera(t_env *env, t_dvec3 *move_dir,
									double move_speed);


void				reset(t_env *env, t_cam *restrict cam, size_t obj_cnt);

void				rotate_vec(t_dvec3 *vec, t_dvec3 *rotate_angle);
void				rotate_x(t_dvec3 *destination,
								const t_dvec3 *restrict pt,
								double angle);
void				rotate_y(t_dvec3 *destination,
								const t_dvec3 *restrict pt,
								double angle);
void				rotate_z(t_dvec3 *destination,
								const t_dvec3 *restrict pt,
								double angle);
void				rotate_objects(t_env *env, t_dvec3 rot);
void				rotate_camera(t_env *env, t_dvec3 rot);

void				vec3_dot_vec3(double *destination,
									const t_dvec3 *first,
									const t_dvec3 *second);
double				vec3_to_double(t_dvec3 first);
void				vec3_normalize(t_dvec3 *destination,
									const t_dvec3 *restrict first);

void				vec3_add_vec3(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second);
void				vec3_sub_vec3(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second);
void				vec3_mul_vec3(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second);
void				vec3_div_vec3(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second);
void				vec3_crs_vec3(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second);

void				double_add_vec3(t_dvec3 *destination,
									double first,
									const t_dvec3 *restrict second);
void				double_sub_vec3(t_dvec3 *destination,
									double first,
									const t_dvec3 *restrict second);
void				double_mul_vec3(t_dvec3 *destination,
									double first,
									const t_dvec3 *restrict second);
void				double_div_vec3(t_dvec3 *destination,
									double first,
									const t_dvec3 *restrict second);

void				vec3_add_double(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									double second);
void				vec3_sub_double(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									double second);
void				vec3_mul_double(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									double second);
void				vec3_div_double(t_dvec3 *destination,
									const t_dvec3 *restrict first,
									double second);
void				quit_program(t_env *env);
void				calculate_oc_tc_dir(const t_ray *ray,
										const t_uni *obj,
										t_dvec3_comp *computs);
void				save_screenshot(t_env *env);

#endif
