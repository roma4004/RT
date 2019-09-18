/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:41:05 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/18 09:49:02 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
//rtv_macro:
//# define FOV M_PI / 2.
# define FOV 60
# define WIN_WIDTH 1024u
# define WIN_HEIGHT 1024u
# define BACKGROUND_COLOR 0xffffff
# define VIEWPORT_SIZE 1.
# define DISTANCE_TO_PLANE 1.
//tmp defines
#define SPHERE_CNT 4
#define LIGHTS_CNT 3

//rtv_macro end


# define WIN_NAME "wolf3d by dromanic (@Dentair)"
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

/*todo:
**	red dot aim
**	visual editor
**	inventory [i]
**	pause [p]
**	quick belt (fast access)
**	menu and splash screen
**/

typedef struct		s_vector_signed_int32
{
	__int32_t		x;
	__int32_t		y;
}					t_sivec;

typedef struct		s_vector3_signed_int32
{
	__int32_t		x;
	__int32_t		y;
	__int32_t		z;
}					t_sivec3;

typedef struct		s_vector3_float
{
	float			x;
	float			y;
	float			z;
}					t_fvec3;

typedef struct		s_vector_float
{
	float			x;
	float			y;
}					t_fvec;

typedef struct		s_vector_double
{
	double			x;
	double			y;
}					t_dvec;

typedef struct		s_line_float
{
	t_fvec			sta;
	t_fvec			end;
}					t_fline;

typedef struct	s_uint32_point
{
	Uint32		x;
	Uint32		y;
}				t_ui32pt;

typedef struct	s_sint32_point
{
	Sint32		x;
	Sint32		y;
}				t_sint32_pt;

typedef struct	s_double_point
{
	double		x;
	double		y;
}				t_db_pt;

typedef struct	s_double_point3
{
	double		x;
	double		y;
	double		z;
}				t_db_3pt;

typedef struct material
{
	t_fvec3	diffuse_color;
	float	specular_exponent;
	float	albedo;
}				t_mat;

typedef struct sphere
{
	t_fvec3	center;
	float	radius;
//	t_fvec3	color;
	t_mat	mat;
//	int		specular;

}				t_sphr;


typedef struct light {
	unsigned	type;
	t_fvec3		pos;
	float		intensity;
	t_fvec3		color;
}				t_lght;






typedef struct	s_sprite
{
	SDL_Surface	*image;
	SDL_Rect	rect;
	Uint32		width;
	Uint32		height;
}				t_sprite;

typedef struct	s_line
{
	Uint32			tex_num;
	Uint32			*img;
	Uint32			x;
	Uint32			y;
	Uint32			tex_y;
	Uint32			end_y;
	Uint32			color;
	Uint32			scale;
	Uint32			height;
	double			half;
	double			normal;
	double			weight;
	double			current_dist;
	t_db_pt			pos;
	t_db_pt			beg;
	t_ui32pt		texture;
	bool			side;
}				t_line;

typedef struct	s_ray
{
	double			wall_x;
	double			x;
	t_db_pt			dist;
	t_db_pt			step;
	t_db_pt			dir;
	t_ui32pt		pos;
	int perem:1;
}				t_ray;

typedef struct	s_frame_per_second
{
	u_char		value;
	double		frame_time;
	Uint32		frame_limit_second;
	Uint32		cur_tick;
	Uint32		pre_tick;
}				t_fps;

typedef struct	s_text
{
	int				width;
	int				height;
	char			str[3];
	SDL_Color		color;
	SDL_Surface		*sur_str;
	SDL_Texture		*tex_str;
	SDL_Rect		rect_txt;
	SDL_Rect		rect_val;
	TTF_Font		*font;
}				t_txt;

typedef struct	s_camera
{
	double			x;
	double			zoom;
	double			move_speed;
	double			min_wall_dist;
	double			rotate_speed;
	t_ui32pt		center;
	t_sint32_pt		step;
	t_fvec3			pos;
	t_fvec3			dir;


	float			t_min;
	float			t_max;
	t_db_pt			plane;
	t_fvec3			rotate_angle;
}				t_cam;

typedef struct	s_map
{
	Uint32			**tex_id;
	Uint32			height;
	Uint32			width;
	t_ui32pt		center;
}				t_map;

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

//	bool			is_compass_texture;
	unsigned char	mode;
}				t_flags;

typedef struct	s_environment
{
	int				state_arr_length;
	int				threads;
	Uint8			bytes_per_pixel;
	Uint8			bits_per_pixel;
	Uint32			err_id;
	Uint32			buff[WIN_HEIGHT][WIN_WIDTH];
	Uint32			gen_tex[TEXTURES][TEX_WIDTH * TEX_HEIGHT];
	const Uint8		*state;
	t_flags			flags;
	t_cam			cam;
	t_txt			txt;
	t_fps			fps;
	t_map			map;
	SDL_Surface		**img_tex;
//	SDL_Event		event;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*screen;
	SDL_Surface		*surface;
	Mix_Music		*music;

	t_sphr		*sphere_arr;
	t_lght		*light_arr;
	t_fvec		canvas_half;
	t_fvec3		bg_color;
	double		epsilon;
}				t_env;

typedef struct	s_pthread_data
{
	t_env	*env;
	int		offset;
}				t_pth_dt;

enum			e_colors
{
	RED = 0xFF0000,
	GREEN = 0x00FF00,
	BLUE = 0x0000FF,
	WHITE = 0xFFFFFF,
	YELLOW = 0xFFFF00
};

enum			e_errors
{
	MAP_ERR = 404,
	READ_ERR = 405,
	MAP_SIZE_ERR = 406,
	INVALID_RESOURCE = 407,
	SPACE = 408,
	ITS_A_DIRECTORY = 21
};

enum			e_texture_mode
{
	COLOR_TEX = 0,
	GENERATED_TEX = 1,
	IMAGE_TEX = 2
};

enum			e_light_type
{
	AMBIENT = 0,
	POINT = 1,
	DIRECTIONAL = 2
};

t_env			*init_env(void);
t_env			*parse_map(char *file_name, t_env *env);
void			generate_texture(t_env *env);
//void			raycasting(t_env *env, Uint32 **map);
void			*multi_raycasting(void *thread_data);
//Uint32			chose_color(Uint32 switch_num, bool side);
void			event_handler(t_env *env, t_cam *cam, t_flags *flags);
int				render_interface(t_env *env, t_fps *fps, t_txt *cam);
//void			show_errors(t_env *env);
//void			quit_program(t_env *env);

t_fvec3 get_light(t_env *env, t_fvec3 point, t_fvec3 normal, t_fvec3 view, t_sphr *obj);

float			vec3_length(t_fvec3 vec);
void				rerender_scene(t_env *env);

t_fvec3				send_ray(t_env *env);
t_sphr				*intersect_obj(t_env *env, float *dist);
t_sphr				*is_shadow_ray(t_env *env, t_fvec3 origin, t_fvec3 direction, t_fvec limits);

t_fvec3				convert_to_viewport(float x, float y);

//float				vec3_length(t_fvec3 vec);

void				rotate_cam(t_env *env);

float				vec3_dot_vec3(t_fvec3 first, t_fvec3 second);
void				vec3_to_negative(t_fvec3 *restrict destination);
float				vec3_mag(const t_fvec3 *restrict first);
float				vec3_to_float(t_fvec3 first);
void				vec3_normalize(t_fvec3 *restrict destination,
									t_fvec3 *restrict first);
void				vec3_normalize_ptr(t_fvec3 *restrict first);
t_fvec3				vec3_normalize_cpy(t_fvec3 first);

void				vec3_add_vec32(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const t_fvec3 *restrict second);
void				vec3_sub_vec32(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const t_fvec3 *restrict second);
void				vec3_mul_vec32(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const t_fvec3 *restrict second);
void				vec3_div_vec32(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const t_fvec3 *restrict second);
void 				vec3_cross_vec32(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const t_fvec3 *restrict second);
t_fvec3				vec3_add_vec3(t_fvec3 first, t_fvec3 second);
t_fvec3				vec3_sub_vec3(t_fvec3 first, t_fvec3 second);
t_fvec3				vec3_mul_vec3(t_fvec3 first, t_fvec3 second);
t_fvec3				vec3_div_vec3(t_fvec3 first, t_fvec3 second);
t_fvec3				vec3_cross_vec3(t_fvec3 first, t_fvec3 second);

void				float_add_vec32(t_fvec3 *restrict destination,
							const float first,
							const t_fvec3 *restrict second);
void				float_sub_vec32(t_fvec3 *restrict destination,
							const float first,
							const t_fvec3 *restrict second);
void				float_mul_vec32(t_fvec3 *restrict destination,
							const float first,
							const t_fvec3 *restrict second);
void				float_div_vec32(t_fvec3 *restrict destination,
							const float first,
							const t_fvec3 *restrict second);
t_fvec3				float_add_vec3(float first, t_fvec3 second);
t_fvec3				float_sub_vec3(float first, t_fvec3 second);
t_fvec3				float_mul_vec3(float first, t_fvec3 second);
t_fvec3				float_div_vec3(float first, t_fvec3 second);

void				vec3_add_float2(t_fvec3 *restrict destination,
							 const t_fvec3 *restrict first,
							 const float second);
void				vec3_sub_float2(t_fvec3 *restrict destination,
							 const t_fvec3 *restrict first,
							 const float second);
void				vec3_mul_float2(t_fvec3 *restrict destination,
							 const t_fvec3 *restrict first,
							 const float second);
void				vec3_div_float2(t_fvec3 *restrict destination,
							 const t_fvec3 *restrict first,
							 const float second);
t_fvec3				vec3_add_float(t_fvec3 first, float second);
t_fvec3				vec3_sub_float(t_fvec3 first, float second);
t_fvec3				vec3_mul_float(t_fvec3 first, float second);
t_fvec3				vec3_div_float(t_fvec3 first, float second);











#endif

//#ifndef DOOM_H
//# define DOOM_H
//
//# include <SDL_ttf.h>
//# include <SDL_image.h>
//# include <SDL_mixer.h>
//# include <unistd.h>
//# include <stdlib.h>
//# include <sys/types.h>
//# include <sys/uio.h>
//# include <sys/stat.h>
//# include <fcntl.h>
//# include <time.h>
//# include <math.h>
//# include <string.h>
//# include <errno.h>
//# include <stdarg.h>
//# include "../libraries/libft/libft.h"
//
//# define BUFF_SIZE			2000
//# define WIDTH				1200
//# define HEIGHT				800
//# define SKY_BOX_WIDTH		4300.0
//# define SKY_BOX_HEIGHT		2150.0
//# define CENTR_X			(WIDTH / 2)
//# define CENTR_Y			(HEIGHT / 2)
//
//# define MEDIA_PATH			"media"
//# define SKYBOX_PATH		MEDIA_PATH"/skybox"
//# define MAPS_PATH			MEDIA_PATH"/maps"
//# define SOUNDS_PATH		MEDIA_PATH"/sounds"
//# define TEXTURES_PATH		MEDIA_PATH"/textures"
//# define FONTS_PATH			MEDIA_PATH"/fonts"
//# define SPRITES_PATH		MEDIA_PATH"/sprites"
//# define FONT_SP			FONTS_PATH"/Special.ttf"
//# define FONT_DM			FONTS_PATH"/DooM.ttf"
//# define WEAPONS_PATH		SPRITES_PATH"/weapons"
//# define ITEMS_PATH			SPRITES_PATH"/items"
//# define ENEMY_PATH			SPRITES_PATH"/enemy"
//
//# define TEX_SEC			"texture_sector "
//# define END_SEC			"end  "
//# define DOR_SEC			"door_sector "
//# define LIF_SEC			"lift "
//# define VER_SEC			"vertex "
//# define SEC_SEC			"sector "
//# define PLA_SEC			"player "
//
//# define MSG_END_F			"MISSION FAILED"
//# define MSG_END_A			"MISSION ACCOMPLISHED"
//# define MSG_ITEMS			"ITEMS :  "
//# define MSG_KILLS			"KILLS :  "
//# define MSG_BRIEF			"Get to the extraction point alive"
//
//
//#endif
