/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 19:36:33 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# define WIN_NAME "RT by dromanic (@Dentair)"
# define WIN_WIDTH 1000u
# define WIN_HEIGHT 1000u
# define VIEWPORT_SIZE 1.0
# define DISTANCE_TO_PLANE 1.0
# define VALUES_PER_OBJ 14
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
# include "SDL_image.h"

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
	int				padding;
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
	t_dvec			half;
	double			rate;
	unsigned		reflective_dept;
	char			padding[4];
	t_dvec3			origin_dir_x;
	t_dvec3			origin_dir_y;
	t_dvec3			origin_dir_z;
}					t_cam;

typedef struct		s_universal_object
{
	t_dvec3			pos;
	double			radius;
	t_dvec3			dir;
	double			height;
	t_dvec3			color;
	double			specular;
	void			(*get_intersect)
					(t_dvec3 *touch,
						const struct s_universal_object *obj,
						const t_ray *ray);
	void			(*get_normal)
					(t_ray *ray,
						const struct s_universal_object *obj,
						double dist);
	double			reflective_coef;
	double			refractive_coef;
	t_dvec3			pos_backup;
	double			radius_backup;
	t_dvec3			dir_backup;
	_Bool			is_selected;
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
	t_dvec3			oc;
	t_dvec3			tc;
	double			dir;
	t_dvec3			dir_vec;
	double			m;
}					t_dvec3_comp;

typedef struct		s_light_calculating
{
	t_lght			*cur;
	t_dvec3			dir;
	double			defuse_val;
	double			specul_val;
	t_dvec3			view;
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
	_Bool			is_grayscale;
	_Bool			is_screenshot;
	char			padding[2];
}					t_flags;

typedef struct		s_environment
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	t_uni			*uni_arr;
	t_uni			*selected_obj;
	t_lght			*light_arr;
	Uint32			buff[WIN_HEIGHT][WIN_WIDTH];
	t_dvec3			bg_color;
	size_t			threads;
	t_cam			cam;
	size_t			uni_arr_len;
	size_t			light_arr_len;
	t_flags			flags;
}					t_env;

typedef struct		s_pthread_data
{
	t_env			*env;
	size_t			id;
}					t_pth_dt;

enum				e_errors
{
	ERR_SCENE = 404,
	ERR_READ = 405,
	ERR_SIZE = 406,
	ERR_DIRECTORY = 21,
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

/*
**					draw.c
*/
t_dvec3				convert_to_viewport(double x, double y, double rate);
void				draw_scene(t_env *env, size_t threads);

/*
**					effects.c
*/
void				apply_effects(t_flags *flags, t_dvec3 *color);
void				save_screenshot(t_env *env);

/*
**					init.c
*/
t_env				*init_env(void);
t_env				*init_sdl2(t_env *env);

/*
**					parse_utils.c
*/
_Bool				init_obj_arr(t_env *env, t_list *lst);
void				set_obj_value(t_env *env, const double *v, size_t type);

/*
**					key_down_cam_move.c
*/
_Bool				is_x_move_down(t_ivec3 *move, SDL_Keycode k);
_Bool				is_y_move_down(t_ivec3 *move, SDL_Keycode k);
_Bool				is_z_move_down(t_ivec3 *move, SDL_Keycode k);

/*
**					key_down_cam_move.c
*/
_Bool				is_x_rotate_down(t_ivec3 *rotate, SDL_Keycode k);
_Bool				is_y_rotate_down(t_ivec3 *rotate, SDL_Keycode k);
_Bool				is_z_rotate_down(t_ivec3 *rotate, SDL_Keycode k);
void				rotate_camera(t_env *env, t_dvec3 rot);

/*
**					key_mouse.c
*/
void				select_caps_cylinder_cone(t_env *env);

/*
**					key_mouse.c
*/
void				move_objects(t_env *env, const t_dvec3 *move_dir,
						double move_speed);
void				move_camera(t_env *env, const t_dvec3 *move_dir,
						double move_speed);

/*
**					key_reset.c
*/
void				reset(t_env *env, t_cam *restrict cam, size_t obj_cnt);

/*
**					key_up_cam_move.c
*/
_Bool				is_x_move_up(t_ivec3 *move, SDL_Keycode k);
_Bool				is_y_move_up(t_ivec3 *move, SDL_Keycode k);
_Bool				is_z_move_up(t_ivec3 *move, SDL_Keycode k);

/*
**					key_up_cam_move.c
*/
_Bool				is_x_rotate_up(t_ivec3 *rotate, SDL_Keycode k);
_Bool				is_y_rotate_up(t_ivec3 *rotate, SDL_Keycode k);
_Bool				is_z_rotate_up(t_ivec3 *rotate, SDL_Keycode k);

/*
**					keys.c
*/
_Bool				event_handler(t_env *env, t_cam *cam, t_flags *flags);

/*
**					ligths.c
*/
void				get_light(t_env *env, t_lght_comp *l,
						const t_uni *obj, t_dvec3 *col, t_ray *ray);

/*
**					main.c
*/
double				double_clamp(double x);
void				quit_program(t_env *env);

/*
**					obj_intersection_base.c
*/
void				get_intersect_sphere(t_dvec3 *touch, const t_uni *sphere,
						const t_ray *ray);
void				get_intersect_plane(t_dvec3 *touch, const t_uni *plane,
						const t_ray *ray);
void				get_intersect_cylinder(t_dvec3 *touch,
						const t_uni *cylinder, const t_ray *ray);
void				get_intersect_cone(t_dvec3 *touch, const t_uni *obj,
						const t_ray *ray);
void				get_intersect_disk(t_dvec3 *touch, const t_uni *disk,
						const t_ray *ray);

/*
**					obj_normal_base.c
*/
void				set_normal_sphere(t_ray *ray, const t_uni *plane,
						double dist);
void				set_normal_plane(t_ray *ray, const t_uni *plane,
						double dist);
void				set_normal_cylinder(t_ray *ray, const t_uni *cylinder,
						double dist);
void				set_normal_cone(t_ray *ray, const t_uni *cone,
						double dist);

/*
**					obj_utils.c
*/
void				discriminant_comput(t_dvec3 *touch, const t_dvec3 *tmp);
void				(*g_intersect_catalog(size_t type))
						(t_dvec3 *touch, const t_uni *obj, const t_ray *ray);
void				(*g_normal_catalog(size_t type))
						(t_ray *ray, const t_uni *obj, double dist);
void				crop_cyl_n_cone(t_dvec3 *touch, double dir,
						double oc_dot_dir, double height);
void				calculate_oc_tc_dir(t_dvec3_comp *computs, const t_uni *obj,
						const t_ray *ray);

/*
**					parsing.c
*/
t_env				*parse_scene(t_env *env, char *file_name);

/*
**					parsing_validate_scene.c
*/
_Bool				is_valid_line(t_env *env, char **line, size_t len);
size_t				get_type(const char *str);
size_t				count_number(t_env *env, char *str, size_t len);

/*
**					ray_traces.c
*/
const t_uni			*is_shadow_ray(t_env *env, const t_ray *ray,
						const t_dvec3 *shadow_dir, double t_max);
void				send_selected_ray(t_env *env, t_ray *ray,
						t_uni **obj, double dist);
void				send_ray(t_env *env, t_ray *ray, t_dvec3 *cur_color);

/*
**					utils.c
*/
void				set_backup_val(t_uni *obj);

/*
**					key_utils.c
*/
void				swith_handle(t_env *env, t_flags *flags, size_t obj_cnt);
void				count_selected_obj(size_t *dest, t_uni *uni_arr,
						size_t uni_arr_len);

/*
**					color.c
*/
t_dvec3				vec3_clamp_col_cpy(t_dvec3 first);
void				double_mul_vec3_col(t_dvec3 *destination, double first,
						const t_dvec3 *restrict second);
void				vec3_add_vec3_col(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second);
void				ft_clamp_in_range(double *dest, double value,
						double min, double max);
void				ft_clamp_in_range_vec(t_dvec3 *dest,
						double min, double max);

/*
**					vec3_utils.c
*/
void				vec3_length(double *destination,
						const t_dvec3 *restrict first);
void				vec3_dot_vec3(double *destination,
						const t_dvec3 *first,
						const t_dvec3 *second);
void				vec3_normalize(t_dvec3 *destination,
						const t_dvec3 *restrict first);

/*
**					vec3_rotate.c
*/
void				rotate_x(t_dvec3 *destination, const t_dvec3 *restrict pt,
						double angle);
void				rotate_y(t_dvec3 *destination, const t_dvec3 *restrict pt,
						double angle);
void				rotate_z(t_dvec3 *destination, const t_dvec3 *restrict pt,
						double angle);
void				rotate_vec(t_dvec3 *vec, const t_dvec3 *rotate_angle);
void				rotate_objects(t_env *env, t_dvec3 rot);

/*
**					vec3_with_double.c
*/
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

/*
**					vec3_with_vec3.c
*/
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

/*
**					double_with_vect3.c
*/
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

#endif
