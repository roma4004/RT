/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 19:50:19 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
//rtv_macro:
//# define FOV M_PI / 2.
# define FOV 60
# define WIN_WIDTH 1000u
# define WIN_HEIGHT 1000u
# define BACKGROUND_COLOR 0xffffff
# define VIEWPORT_SIZE 1.0
# define DISTANCE_TO_PLANE 1.0
//tmp defines

#define PARSE_PARAM_CNT 6

#define SPHERE_CNT 4
#define LIGHTS_CNT 3
#define PLANE_CNT 1
#define CYLINDER_CNT 1
#define CONE_CNT 1
#define UNI_OJB_CNT (SPHERE_CNT + PLANE_CNT + CYLINDER_CNT + CONE_CNT)


//rtv_macro end


# define WIN_NAME "RTv1 by dromanic (@Dentair)"
# define DEFAULT_MENU_COLOR 0x0f9100FF
# define DEF_FONT "resources/fonts/ARIAL.TTF"
# define DEF_FONT_SIZE 24
# define FRAME_LIMIT 60
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define TEXTURES 8
# define DEF_EDGE_TEX 2
# define SHOW_FPS 1
# define DEBUG 1
# define MAX_MAP_SIDE 10000

# include <fcntl.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdlib.h>
# include <errno.h>
# include <math.h>

# pragma GCC diagnostic ignored "-Wstrict-prototypes"
# pragma GCC diagnostic ignored "-Wpadded"
# pragma GCC diagnostic ignored "-Wdocumentation"
# pragma GCC diagnostic ignored "-Wundef"
# pragma GCC diagnostic ignored "-Wreserved-id-macro"
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "SDL_mixer.h"
# include "SDL_audio.h"
# pragma GCC diagnostic warning "-Wreserved-id-macro"
# pragma GCC diagnostic warning "-Wundef"
# pragma GCC diagnostic warning "-Wdocumentation"
# pragma GCC diagnostic warning "-Wpadded"
# pragma GCC diagnostic warning "-Wstrict-prototypes"

# include "libft.h"
# include "get_next_line.h"


# define MAX_LEN		200
# define MAX_ROWS		400

enum			e_figures
{
	F_NON = 10,
	F_SPHERE = 11,
	F_PLANE = 12,
	F_CONE = 13,
	F_CYLINDER = 14,
};

enum			e_numbers
{
	N_SPHR_NBR = 8,
	N_PLN_NBR = 8,
	N_CONE_NBR = 8,
	N_CLDR_NBR = 8,
};


/*todo:
**	red dot aim
**	visual editor
**	inventory [i]
**	pause [p]
**	quick belt (fast access)
**	menu and splash screen
**/

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

typedef struct	universal_object
{
	unsigned		type;
	t_dvec3			pos;
	double			radius;
	t_dvec3			dir;
	t_dvec3			diffuse_color;
	double			specular;
//	void			(*intersect)(t_dvec3 *, struct universal_object *,
//								t_dvec3, t_dvec3 *);
}				t_uni;

typedef struct	cone
{
	unsigned	type;
	t_dvec3		pos;
	double		angle;
	t_dvec3		dir;
	t_dvec3		diffuse_color;
	double		specular;
}				t_cone;

typedef struct	light {
	unsigned	type;
	t_dvec3		pos;
	double		intensity;
	t_dvec3		color;
}				t_lght;


typedef struct			s_lght
{

	int					type;
	double				intensity;
	t_dvec3				dir;


	struct s_lght		*next;
}						t_llght;

typedef struct		s_light_calculating
{
	t_lght			*cur;
	t_dvec3			dir;
	double			defuse_val;
	double			t_max;
	double			specul_val;

	t_dvec3		touch_point;
	t_dvec3		obj_normal;
	t_dvec3		view;
}					t_lght_comp;

typedef struct	s_canvas_parameter
{
	t_dvec			half;
	double			rate;

}				t_canvas_par;

typedef struct	s_camera
{
	double			move_speed;
	double			rotate_speed;

	t_dvec3			pos;
	t_dvec3			dir;

	double			t_min;
	double			t_max;
	t_dvec3			rotate_angle;
	t_canvas_par	canvas;
}				t_cam;

typedef struct	s_flags
{
	bool			is_rtv1_over;

	bool			is_move_x_less;
	bool			is_move_x_more;

	bool			is_move_y_more;
	bool			is_move_y_less;

	bool			is_move_z_more;
	bool			is_move_z_less;

	bool			is_rotate_x_more;
	bool			is_rotate_x_less;

	bool			is_rotate_y_more;
	bool			is_rotate_y_less;

	bool			is_rotate_z_more;
	bool			is_rotate_z_less;
}				t_flags;

//typedef struct			s_read
//{
//	char				*line;
//	int					len;
//	struct s_read		*next;
//}						t_read;

typedef struct			s_clr
{
	int 				r;
	int 				g;
	int 				b;
	int 				a;
}						t_clr;

typedef struct			s_fgrs
{
	int 				type;
	t_dvec3				center;
	double				radius;
	t_clr 				color;
	int 				specul;

	struct s_fgrs		*next;
	//	struct s_fgrs		*prev;
}						t_fgrs;

typedef struct	s_environment
{
	Uint32			err_id;
	Uint32			buff[WIN_HEIGHT][WIN_WIDTH];
	t_flags			flags;
	t_cam			cam;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	SDL_Surface		*surface;

	t_uni			*uni_lst;
	size_t			*uni_arr_len;
	t_lght			*light_arr;
	size_t			*light_arr_len;

	t_dvec3			bg_color;
	double			epsilon;
	t_list			*lst;
	t_fgrs			*figures;
}				t_env;

enum			e_errors
{
	MAP_ERR = 404,
	READ_ERR = 405,
	MAP_SIZE_ERR = 406,
	INVALID_RESOURCE = 407,
	SPACE = 408,
	ITS_A_DIRECTORY = 21,

	ERR_USAGE			= 0,
	ERR_MALLOC			= 1,
	ERR_SDL				= 2,
	ERR_ARGV			= 3,
	ERR_LEN				= 4,
	ERR_ROWS			= 5,
	ERR_WRONG_SYMBOL	= 9,
	ERR_PIPE			= 10,
	ERR_DOT				= 12,
	ERR_NBR				= 13,
	ERR_MINUS			= 14,
	ERR_LINE			= 15
};

enum			e_light_type
{
	AMBIENT = 0,
	POINT = 1,
	DIRECTIONAL = 2,
	SPHERE = 3,
	PLANE = 4,
	CYLINDER = 5,
	CONE = 6,
};

void					validation_file(t_env *env, int fd);
void					read_data(t_env *env);
void					print_error(int error, int row);


t_env				*init_env(void);
bool				event_handler(t_cam *cam, t_flags *flags);
//void				show_errors(t_env *env);
//void				quit_program(t_env *env);
t_uni				*intersect_obj(t_env *env, t_cam *cam, double *dist);
t_dvec3				get_light(t_env *env, t_lght_comp *l, t_uni *obj);
t_dvec3				discriminant_comput(t_dvec3 *tmp);
double				vec3_length(t_dvec3 vec);
void				rerender_scene(t_env *env);

void				send_ray(t_env *env, t_cam *cam, t_dvec3 *color);


void				intersect_sphere(t_dvec3 *ray_pos, t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_plane(t_dvec3 *ray_pos, t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_cylinder(t_dvec3 *ray_pos, t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);
void				intersect_cone(t_dvec3 *ray_pos, t_uni *obj,
									t_dvec3 ray_dir, t_dvec3 *plane_toch);

t_uni				*intersect_obj(t_env *env, t_cam *cam, double *dist);
t_uni				*is_shadow_ray(t_uni *uni_arr, t_dvec3 *ray_pos,
									t_dvec3 direction, t_dvec limits, t_uni *obj);

//t_dvec3				convert_to_viewport(double x, double y);

void rotate_cam(t_dvec3 *dir, t_dvec3 *rotate_angle);

double				vec3_dot_vec3(t_dvec3 first, t_dvec3 second);
void				vec3_to_negative(t_dvec3 *restrict destination);
//double				vec3_magnitude(t_dvec3 first);
double				vec3_to_double(t_dvec3 first);
t_dvec3				vec3_normalize(t_dvec3 first);
void				vec3_normalize_ptr(t_dvec3 *restrict first);
t_dvec3				vec3_normalize_cpy(t_dvec3 first);

void				vec3_add_vec32(t_dvec3 *restrict destination,
							const t_dvec3 *restrict first,
							const t_dvec3 *restrict second);
void				vec3_sub_vec32(t_dvec3 *restrict destination,
							const t_dvec3 *restrict first,
							const t_dvec3 *restrict second);
void				vec3_mul_vec32(t_dvec3 *restrict destination,
							const t_dvec3 *restrict first,
							const t_dvec3 *restrict second);
void				vec3_div_vec32(t_dvec3 *restrict destination,
							const t_dvec3 *restrict first,
							const t_dvec3 *restrict second);
void 				vec3_cross_vec32(t_dvec3 *restrict destination,
							const t_dvec3 *restrict first,
							const t_dvec3 *restrict second);
t_dvec3				vec3_add_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_sub_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_mul_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_div_vec3(t_dvec3 first, t_dvec3 second);
t_dvec3				vec3_cross_vec3(t_dvec3 first, t_dvec3 second);

void				double_add_vec32(t_dvec3 *restrict destination,
							const double first,
							const t_dvec3 *restrict second);
void				double_sub_vec32(t_dvec3 *restrict destination,
							const double first,
							const t_dvec3 *restrict second);
void				double_mul_vec32(t_dvec3 *restrict destination,
							const double first,
							const t_dvec3 *restrict second);
void				double_div_vec32(t_dvec3 *restrict destination,
							const double first,
							const t_dvec3 *restrict second);
t_dvec3				double_add_vec3(double first, t_dvec3 second);
t_dvec3				double_sub_vec3(double first, t_dvec3 second);
t_dvec3				double_mul_vec3(double first, t_dvec3 second);
t_dvec3				double_div_vec3(double first, t_dvec3 second);

void				vec3_add_double2(t_dvec3 *restrict destination,
							 const t_dvec3 *restrict first,
							 const double second);
void				vec3_sub_double2(t_dvec3 *restrict destination,
							 const t_dvec3 *restrict first,
							 const double second);
void				vec3_mul_double2(t_dvec3 *restrict destination,
							 const t_dvec3 *restrict first,
							 const double second);
void				vec3_div_double2(t_dvec3 *restrict destination,
							 const t_dvec3 *restrict first,
							 const double second);
t_dvec3				vec3_add_double(t_dvec3 first, double second);
t_dvec3				vec3_sub_double(t_dvec3 first, double second);
t_dvec3				vec3_mul_double(t_dvec3 first, double second);
t_dvec3				vec3_div_double(t_dvec3 first, double second);

#endif
