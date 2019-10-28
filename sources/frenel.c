/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frenel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 17:55:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/23 18:03:41 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_dvec3			ReflectRay(t_dvec3 *R, t_dvec3 *N)
{
	double		cos_i;
	t_dvec3		normal_x2;
	t_dvec3		tmp;

	vec3_dot_vec3(&cos_i, N, R);
	double_mul_vec3(&normal_x2, 2, N);
	vec3_mul_double(&tmp, &normal_x2, cos_i);
	vec3_sub_vec3(&tmp, &tmp, R);
	return (tmp);
}

t_dvec3			compute_refract_dir(double eta, double cosi,
	const t_dvec3 *dir, const t_dvec3 *ref_normal)
{
	double		k;
	t_dvec3		tmp;
	t_dvec3		tmp2;

	k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0.0)
		return ((t_dvec3){0.0, 0.0, 0.0, 0.0});
	else
	{
		double_mul_vec3(&tmp, eta, dir);
		double_mul_vec3(&tmp2, (eta * cosi - sqrt(k)), ref_normal);
		vec3_add_vec3(&tmp, &tmp, &tmp2);
		return (tmp);
	}
}

void			compute_refract_coef(t_dvec3 *tmp, double cos_i, double *kr)
{
	double		cos_t;
	double		Rs;
	double		Rp;
	//	tmp = (t_dvec3){.x = etai, .y = etat, .z = sint};

	cos_t = sqrt(fmax(0.f, 1 - tmp->z * tmp->z));
	cos_i = fabs(cos_i);
	Rs = ((tmp->y * cos_i) - (tmp->x * cos_t))
		/ ((tmp->y * cos_i) + (tmp->x * cos_t));
	Rp = ((tmp->x * cos_i) - (tmp->y * cos_t))
		/ ((tmp->x * cos_i) + (tmp->y * cos_t));
	*kr = (Rs * Rs + Rp * Rp) / 2;
}

t_dvec3			reflect(const t_dvec3 *I, const t_dvec3 *N)
{
	double			I_dot_N;
	t_dvec3			tmp;
	t_dvec3			res;

	vec3_dot_vec3(&I_dot_N, I, N);
	double_mul_vec3(&tmp, 2 * I_dot_N, N);
	vec3_sub_vec3(&res, I, &tmp);
	return (res);
}

void			fresnel(const t_dvec3 *dir, const t_dvec3 *normal,
					const double *obj_refract_coef, double *kr)
{
	double		cos_i;
	double		sin_t;
	double		eta_i;
	double		eta_t;
	t_dvec3		tmp;

	eta_i = 1;
	eta_t = *obj_refract_coef;
	vec3_dot_vec3(&cos_i, dir, normal);
	ft_clamp_in_range(&cos_i, cos_i, -1, 1);
	if (cos_i > 0)
		ft_swap_double(&eta_i, &eta_t);
	sin_t = eta_i / eta_t * sqrt(fmax(0.f, 1 - cos_i * cos_i));				// Compute sini using Snell's law
	if (sin_t >= 1)																// Total internal reflection
		*kr = 1;
	else
	{
		tmp = (t_dvec3){.x = eta_i, .y = eta_t, .z = sin_t};
		compute_refract_coef(&tmp, cos_i, kr);
	}// As a consequence of the conservation of energy, transmittance is given by: kt = 1 - kr;
///	*obj = intersect_obj(env, ray, &dist);
}