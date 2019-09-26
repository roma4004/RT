/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/26 18:20:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static	t_dvec3		discriminant_comput(t_dvec3 *tmp)
{
	double				discriminant;
	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;
	if (discriminant < 0.0)
		return ((t_dvec3){MAXFLOAT, MAXFLOAT, 0.0});
	return ((t_dvec3){(-tmp->y + sqrtf(discriminant)) / (2.0 * tmp->x),
						(-tmp->y - sqrtf(discriminant)) / (2.0 * tmp->x), 0.0});
}

static void		intersect_sphere(t_dvec3 ray_pos, t_uni *obj,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	const t_dvec3		center = obj->pos;
	const double		radius = obj->radius;
	t_dvec3				oc;
	t_dvec3				tmp;

	oc = vec3_sub_vec3(ray_pos, center);

	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(ray_dir, ray_dir),
		.y = vec3_to_double(double_mul_vec3(2, vec3_mul_vec3(oc, ray_dir))),
		.z = vec3_dot_vec3(oc, oc) - radius * radius};
	*plane_toch = discriminant_comput(&tmp);
}

static void			intersect_plane(t_dvec3 ray_pos, t_uni *plane,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	t_dvec3		plane_pos;		//plane center		// (plane.pos)
	t_dvec3		normal;			//plane normal		// (plane.ray_dir)
//	t_dvec3		ray_pos;		//ray pos			// (cam.pos)
//	t_dvec3		ray_dir;		//ray directional	// (cam.ray_dir)
	double		denom;

	plane_pos = plane->pos;
	normal    = plane->dir;
//	vec3_normalize(&normal, &normal);
	denom = vec3_dot_vec3(normal, ray_dir);
	if (denom > 1e-6 || denom < 1e-6)
	{
		*plane_toch =
			(t_dvec3){vec3_dot_vec3(vec3_sub_vec3(plane_pos, ray_pos), normal)
			/ denom, MAXFLOAT, 0};
	}
}

void			intersect_cylinder(t_dvec3 point, t_uni *obj, t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	t_dvec3 x = vec3_sub_vec3(point, obj->pos);
	t_dvec3 tmp;
//	D - ray direction.
//	V is a unit length vector that determines cylinder's axis
//	x equals O-C.
//	O is the ray origin
//	C is a center point of a shape that we hit

//	a   = D|D - (D|V)^2
//	b/2 = D|x - (D|V)*(x|V)
//	c   = x|x - (x|V)^2 - r*r
	t_dvec3 v = obj->dir;

	vec3_normalize(&v, &v);
	tmp = (t_dvec3){.x = vec3_dot_vec3(ray_dir, ray_dir)
				 		- pow(vec3_dot_vec3(ray_dir, v), 2),
					.y = 2 * (vec3_dot_vec3(ray_dir, x)
							- vec3_dot_vec3(ray_dir, v)
							* vec3_dot_vec3(x, v)),
					.z = vec3_dot_vec3(x, x)
						- pow(vec3_dot_vec3(x, v), 2)
						- obj->radius * obj->radius};
	*plane_toch = (discriminant_comput(&tmp));
}


void			intersect_cone(t_dvec3 point, t_uni *obj, t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	const double	k = obj->radius * M_PI / 360.0;
	t_dvec3 x = vec3_sub_vec3(point, obj->pos);
	t_dvec3 tmp;
//	D - ray direction.
//	V is a unit length vector that determines cylinder's axis
//	x equals O-C.
//	O is the ray origin
//	C is a center point of a shape that we hit

//	a   = D|D - (1+k*k)*(D|V)^2
//	b/2 = D|x - (1+k*k)*(D|V)*(x|V)
//	c   = x|x - (1+k*k)*(x|V)^2

t_dvec3 D = ray_dir;
t_dvec3 V = obj->dir;


	vec3_normalize(&obj->dir, &obj->dir);
	tmp = (t_dvec3){.x =      vec3_dot_vec3(D,D) - pow((1 + k * k) * vec3_dot_vec3(D,V), 2),
					.y = 2 * (vec3_dot_vec3(D,x) -     (1 + k * k) * vec3_dot_vec3(D,V) * vec3_dot_vec3(x,V)),
					.z =      vec3_dot_vec3(x,x) - pow((1 + k * k) * vec3_dot_vec3(x,V), 2)};
	*plane_toch = (discriminant_comput(&tmp));
}

t_uni				*intersect_obj(t_env *env, double *dist)//dist need move to ray struct
{
	size_t		i;
	t_uni		*cur_obj;
	t_dvec3		touch;

	cur_obj = NULL;
	i = -1;
	while (++i < SPHERE_CNT + PLANE_CNT + CYLINDER_CNT + CONE_CNT)
	{
		if (env->uni_arr[i].type == SPHERE)
			intersect_sphere(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		else if (env->uni_arr[i].type == PLANE)
			intersect_plane(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		else if (env->uni_arr[i].type == CYLINDER)
			intersect_cylinder(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		else if (env->uni_arr[i].type == CONE)
			intersect_cone(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		if (touch.x < *dist //compute nearly obj
		&& env->cam.t_min < touch.x
		&& touch.x < env->cam.t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist
		&& env->cam.t_min < touch.y
		&& touch.y < env->cam.t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

t_uni		*is_shadow_ray(t_uni *uni_arr, t_dvec3 origin,
							t_dvec3 direction, t_dvec limits)
{
	size_t		i;
	t_dvec3		touch;

	i = -1;
	while (++i < SPHERE_CNT + PLANE_CNT + CYLINDER_CNT + CONE_CNT)
	{
		if (uni_arr[i].type == SPHERE)
			intersect_sphere(origin, &uni_arr[i], direction, &touch);
		else if (uni_arr[i].type == PLANE)
			intersect_plane(origin, &uni_arr[i], direction,  &touch);
		else if (uni_arr[i].type == CYLINDER)
			intersect_cylinder(origin, &uni_arr[i], direction,  &touch);
		else if (uni_arr[i].type == CONE)
			intersect_cone(origin, &uni_arr[i], direction,  &touch);
		if ((touch.x < MAXFLOAT
			&& limits.x < touch.x
			&& touch.x < limits.y)
		|| (touch.y < MAXFLOAT
			&& limits.x < touch.y
			&& touch.y < limits.y))
			return (&uni_arr[i]);
	}
	return (NULL);
}

t_dvec3				get_normal(t_env *env, t_uni *obj, t_dvec3 point, double dist)
{
	t_dvec3		obj_normal;

	if (obj->type == SPHERE)
	{
		obj_normal = vec3_sub_vec3(point, obj->pos);
		vec3_normalize(&obj_normal, &obj_normal);
		return (obj_normal);
	}
	if (obj->type == PLANE)
	{
		obj_normal = obj->dir;
		vec3_normalize(&obj_normal, &obj_normal);
		return (obj_normal);
	}
	if (obj->type == CYLINDER)
	{
		t_dvec3 v = obj->dir;													vec3_normalize(&v, &v); //need norm dir after parsing
		t_dvec3 x = vec3_sub_vec3(env->cam.pos, obj->pos);
		obj_normal = (vec3_sub_vec3(vec3_sub_vec3(point, obj->pos),
			vec3_mul_double(v,
				vec3_dot_vec3(v, v) * dist
					+ vec3_dot_vec3(x, v))));
		vec3_normalize(&obj_normal, &obj_normal);
		return (obj_normal);
	}
	if (obj->type == CONE)
	{


		////todo: nrm( P-C - (1+k*k)*V*m )
		t_dvec3 v = obj->dir;													vec3_normalize(&v, &v); //need norm dir after parsing
		t_dvec3 x = vec3_sub_vec3(env->cam.pos, obj->pos);
		obj_normal = (vec3_sub_vec3(vec3_sub_vec3(point, obj->pos),
			vec3_mul_double(v,
				vec3_dot_vec3(v, v) * dist
					+ vec3_dot_vec3(x, v))));
		vec3_normalize(&obj_normal, &obj_normal);
		return (obj_normal);
	}
	return ((t_dvec3){0});
}

t_dvec3				send_ray(t_env *env)
{
	double			dist;
	t_uni			*obj;
	t_lght_comp		l;

	dist = MAXFLOAT;
	ft_bzero(&l, sizeof(t_lght_comp));
	if (!(obj = intersect_obj(env, &dist)))
		return (env->bg_color);
	l.touch_point =
		vec3_add_vec3(env->cam.pos, double_mul_vec3(dist, env->cam.dir)); //ray not cam
	l.view = double_mul_vec3(-1, env->cam.dir);
	l.obj_normal = get_normal(env, obj, l.touch_point, dist);
	return (get_light(env, &l, obj));
}