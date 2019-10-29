/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 17:05:18 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# define WIN_NAME "RT"
# define VIEWPORT_SIZE 1.0
# define DISTANCE_TO_PLANE 1.0
# define VALUES_PER_OBJ 14
# define OBJ_TYPE_MAX 9
# define DEBUG 0
# define MAX_MAP_SIDE 10000
# define REFLECTIVE_MAX_DEPT 5

# include <fcntl.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdlib.h>
# include <errno.h>
# include <math.h>
# include <time.h>
# include "parson.h"

# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"

# include "libft.h"
# include "get_next_line.h"

//---------------------------------TODO:json defines--------------------
//# define FGRS_ANGLE		"angle"
# define POSITION		"position"
# define COLOR			"color"
# define SPCULAR		"specular"
# define DIRECTION		"direction"
# define RADIUS			"radius"
# define HEIGHT			"height"
# define RFLCTV			"reflective"
# define RFRCTV			"refractive"
# define INTENSITY		"intensity"

typedef struct		s_calculate_array
{
	JSON_Object		*object;
	size_t			i;
	size_t			i_js;
	size_t			js_len;
	size_t			type;
}					t_calc_arr;

//---------------------------------json defines--------------------

typedef struct		s_vector3_int
{
	int				x;
	int				y;
	int				z;
	int				padding;
}					t_ivec3;

typedef struct		s_vector_int
{
	int				x;
	int				y;
}					t_ivec;

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

typedef struct		s_light {
	t_dvec3		pos;
	double		intensity;
	size_t		type;
	t_dvec3		col;
}					t_lght;

typedef struct		s_light_calculating
{
	t_lght		*cur;
	t_dvec3		dir;
	t_dvec3		defuse_intens;
	t_dvec3		specul_intens;
	double		obj_specular;
	t_dvec3		obj_color;
	t_dvec3		view;
	double		dist;
}					t_lght_comp;

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
	t_lght_comp		light_comput;
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
	t_dvec3		pos;
	double		radius;
	t_dvec3		dir;
	double		height;
	t_dvec3		color;
	double		specular;
	void		(*get_intersect)
				(t_dvec3 *touch,
					const struct s_universal_object *obj,
					const t_ray *ray);
	void		(*get_normal)
				(t_ray *ray,
					const struct s_universal_object *obj,
					double dist);
	double		reflective_coef;
	double		refractive_coef;//todo
	_Bool		is_selected;
	t_dvec3		pos_backup;
	double		radius_backup;
	t_dvec3		dir_backup;
	double		cone_angle_cache;
	size_t		texture_id;
}					t_uni;

typedef struct		s_touch
{
	t_uni		*obj;
	double		far;
	double		near;
}					t_touch;

typedef struct		s_vector3_comput_tmp
{
	t_dvec3			oc;
	t_dvec3			tc;
	double			dir;
	t_dvec3			dir_vec;
	double			m;
}					t_dvec3_comp;

typedef struct		s_flags
{
	t_ivec3			rotate;
	t_ivec3			move;
	Uint32			err_id;
	_Bool			is_rtv1_over;
	_Bool			is_select_mod;
	_Bool			is_camera_mod;
	_Bool			is_reset;
	_Bool			is_sepia;
	_Bool			is_grayscale;
	_Bool			is_screenshot;
	_Bool			is_menu;
	_Bool			is_info;
}					t_flags;

typedef struct		s_environment
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	t_uni			*uni_arr;
	t_uni			*neg_arr;
	t_uni			*selected_obj;
	t_lght			*light_arr;
	Uint32			*buff;
	TTF_Font		*font;
	SDL_Surface		**tex_arr;
	size_t			tex_arr_len;
	Uint32			buff_width;
	Uint32			buff_height;
	t_dvec3			bg_color;
	size_t			threads;
	t_cam			cam;
	size_t			uni_arr_len;
	size_t			light_arr_len;
	size_t			neg_arr_len;
	t_flags			flags;
}					t_env;

typedef struct		s_pthread_data
{
	t_env		*env;
	size_t		id;
}					t_pth_dt;

enum				e_errors
{
	ERR_SCENE = 404,
	ERR_READ = 405,
	ERR_SIZE = 406,
	INVALID_RESOURCE = 407,
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
	PARABOLOID = 8,
	SPHERENEG = 9,
	SCRN = 10,
	CAM = 11
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
void				convert_to_viewport(t_dvec3 *destination, const t_env *env,
						double x, double y);
void				draw_scene(t_env *env, size_t threads);

/*
**					effects.c
*/
void				apply_effects(t_flags *flags, t_dvec3 *color);
void				save_screenshot(t_env *env);

/*
**					init.c
*/
t_env				*init_sdl2(t_env *env);
t_env				*init_env(void);

/*
**					interface.c
*/
char				*draw_text(t_env *env, t_ivec pos, char *text);
void				show_inteface(t_env *env);

/*
**					interface_utils.c
*/
void				show_info(t_env *env, t_ivec *pos, t_ivec offset);

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
**					key_mouse.c
*/
_Bool				mouse_move(t_env *env, SDL_Event *event);
_Bool				mouse_rotate(t_env *env, SDL_Event *event);
_Bool				mouse_rotate_z(t_env *env, SDL_Event *event);

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
void				get_light(t_dvec3 *col, t_lght_comp *l, const t_env *env,
						const t_ray *ray);

/*
**					main.c
*/
double				double_clamp(double x);
void				screen_update(t_env *env);
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
void				get_intersect_cone(t_dvec3 *touch, const t_uni *cone,
						const t_ray *ray);
void				get_intersect_disk(t_dvec3 *touch, const t_uni *disk,
						const t_ray *ray);
void				get_intersect_paraboloid(t_dvec3 *touch,
						const t_uni *paraboloid, const t_ray *ray);

/*
**					obj_normal_base.c
*/
void				set_normal_sphere(t_ray *ray, const t_uni *sphere,
						double dist);
void				set_normal_plane(t_ray *ray, const t_uni *plane,
						double dist);
void				set_normal_cylinder(t_ray *ray, const t_uni *cylinder,
						double dist);
void				set_normal_cone(t_ray *ray, const t_uni *cone,
						double dist);
void				set_normal_paraboloid(t_ray *ray, const t_uni *paraboloid,
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
**					parse_utils.c
*/
_Bool				init_obj_arr(t_env *env, t_list *lst);
void				set_obj_value(t_env *env, double *v, size_t type);

/*
**					parsing_utils_backup.c
*/
void				set_backup_val(t_uni *obj);

/*
**					parsing_utils_setters.c
*/
void				calc_bot_cap(t_uni *cap, t_uni *his_parent);
void				calc_top_cap(t_uni *cap, t_uni *his_parent);
void				add_caps(t_uni *uni_arr, size_t *id_uni, size_t type);
/*
**					parsing_validate_scene.c
*/
_Bool				is_valid_line(t_env *env, char **line, size_t len);
size_t				get_type(const char *str);
size_t				count_number(t_env *env, char *str, size_t len);

/*
**					ray_traces.c
*/
t_uni				*intersect_obj(double *dist, const t_env *env, t_ray *ray);
const t_uni			*is_shadow_ray(const t_env *env, const t_ray *ray,
						const t_dvec3 *shadow_dir, double t_max);
void				send_selected_ray(t_uni **obj, const t_env *env,
						const t_ray *ray, double dist);
void				send_ray(t_dvec3 *cur_color, const t_env *env,
						t_ray *ray);

/*
**					ray_reflect_n_refract.c
*/
void				send_refract_ray(t_dvec3 *cur_color, t_lght_comp *l,
						const t_env *env, const t_ray *ray);
void				send_reflect_ray(t_dvec3 *cur_color, t_lght_comp *l,
						const t_env *env, const t_ray *ray);

/*
**					key_utils.c
*/
void				swith_handle(t_env *env, t_flags *flags, size_t obj_cnt);
void				count_selected_obj(size_t *dest, t_uni *uni_arr,
						size_t uni_arr_len);
_Bool				select_mod(t_env *env, const SDL_Event *event,
						const t_cam *cam);

/*
**					col.c
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
_Bool				rotate_vec(t_dvec3 *vec, const t_dvec3 *rotate_angle);
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






///dodelat
//int			set_img_cord_to_sphere(t_v3d p, t_sphere *sp);
Uint32			set_color_img(SDL_Surface *img, int x, int y);
void			init_img_tex(t_env *env, SDL_Surface **img_tex);
void			texturing_or_color(t_lght_comp *l, const t_env *env,
	const t_ray *ray, t_uni *obj);
/*
**					json_parson.c
*/
_Bool				json_parson(t_env *env, char *file_name,
								Uint32 *err_id);

/*
**					json_get_value.c
*/
_Bool				get_type_obj(JSON_Object *obj, size_t *type);
_Bool				get_type_light(JSON_Object *obj, size_t *type);
_Bool				get_double_val(double *dst, char *key_word,
							JSON_Object *obj);
_Bool				get_uint_val(Uint32 *dst, char *key_word,
							JSON_Object *obj);
_Bool				get_vector_val(t_dvec3 *dst, char *key_word,
							JSON_Object *obj);

/*
**					json_get_obj.c
*/
_Bool				parse_obj(JSON_Object *jsn_obj, t_uni *obj,
							size_t type, size_t *i);
_Bool				parse_light(JSON_Object *jsn_obj, t_lght *lght,
							size_t type);
_Bool				parse_params(t_env *env, JSON_Array	*arr);
_Bool				count_n_malloc(t_calc_arr *var, t_uni **obj_arr,
									size_t *arr_len, JSON_Array *arr);

#endif
