/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:06:23 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/16 15:02:05 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	Cone formula:
	||(R(t) - C) - ((R(t) - C)·A)A|| = r(h)
	Where:
	- R(t) is any point on cone surface
	- C is cone center point
	- A is normalized axis vector
	- r(h) is radius at height h
	- h is height from cone center
	r(h) = h * tan(theta) where theta is cone angle
	
	Ray formula:
	R(t) = O + tD
	Where:
	- O is ray origin
	- D is ray direction
	- t is distance along ray
	
	Final formula:
	(((O - C) + tD) - (((O - C) + tD)·A)A)·(((O - C) + tD)
		- (((O - C) + tD)·A)A) = (((O - C) + tD)·A)² * tan²(theta)
	at² + bt + c = 0
	Where:
	a = D·D - (1 + tan²(theta))(D·A)²
	b = 2(D·(O-C) - (1 + tan²(theta))(D·A)(O-C)·A)
	c = (O-C)·(O-C) - (1 + tan²(theta))((O-C)·A)²

	(solved through quadratic equation)
	t1 = (-b - √discriminant) / (2a)
	t2 = (-b + √discriminant) / (2a)
*/
static double	validate_t(t_intersection *inters, t_ray ray, double t)
{
	t_vec3	hit_point;
	t_vec3	hp_c;
	double	height;

	if (t < EPSILON)
		return (-1.0);
	hit_point = vec3_add(ray.origin, vec3_mul(ray.direction, t));
	hp_c = vec3_sub(hit_point, inters->center);
	height = vec3_dot(hp_c, inters->axis);
	if (fabs(height) > inters->half_height)
		return (-1.0);
	return (t);
}

static void	check_cap(t_intersection *inters, t_ray ray, t_bool check_top)
{
	t_vec3	cap_center;
	t_vec3	to_cap;
	double	t_cap;

	if (fabs(inters->denom) <= EPSILON)
		return ;
	if (check_top)
		cap_center = vec3_add(inters->center,
				vec3_mul(inters->axis, inters->half_height));
	else
		cap_center = vec3_add(inters->center,
				vec3_mul(inters->axis, -inters->half_height));
	to_cap = vec3_sub(cap_center, ray.origin);
	t_cap = vec3_dot(to_cap, inters->axis) / inters->denom;
	if ((check_top && (t_cap > EPSILON
				&& (inters->t_cap < 0 || t_cap < inters->t_cap)))
		|| (!check_top && t_cap > EPSILON))
	{
		inters->p = vec3_add(ray.origin, vec3_mul(ray.direction, t_cap));
		inters->v = vec3_sub(inters->p, cap_center);
		inters->d2 = vec3_dot(inters->v, inters->v)
			- pow(vec3_dot(inters->v, inters->axis), 2);
		if (inters->d2 <= inters->radius * inters->radius)
			inters->t_cap = t_cap;
	}
}

static t_bool	solve_quadratic(t_intersection *inters, t_ray ray)
{
	t_vec3	oc;
	double	tan_theta;
	double	sqrt_disc;
	double	dot_v_a;
	double	dot_p_a;

	oc = vec3_sub(ray.origin, inters->center);
	tan_theta = inters->radius / inters->half_height;
	dot_v_a = vec3_dot(ray.direction, inters->axis);
	dot_p_a = vec3_dot(oc, inters->axis);
	inters->a = vec3_dot(ray.direction, ray.direction)
		- (1.0 + tan_theta * tan_theta) * dot_v_a * dot_v_a;
	inters->b = 2.0 * (vec3_dot(ray.direction, oc)
			- (1.0 + tan_theta * tan_theta) * dot_v_a * dot_p_a);
	inters->c = vec3_dot(oc, oc) - (1.0 + tan_theta * tan_theta)
		* dot_p_a * dot_p_a;
	inters->discriminant = inters->b * inters->b - 4 * inters->a * inters->c;
	if (inters->discriminant < 0)
		return (0);
	sqrt_disc = sqrt(inters->discriminant);
	inters->t1 = (-inters->b - sqrt_disc) / (2.0 * inters->a);
	inters->t2 = (-inters->b + sqrt_disc) / (2.0 * inters->a);
	return (1);
}

double	intersect_cone(t_ray ray, t_cone *co)
{
	t_intersection	inters;
	double			t_con;

	inters.axis = vec3_normalize(co->axis_vector);
	inters.center = co->coordinates;
	inters.radius = co->co_diameter / 2.0;
	inters.half_height = co->co_height / 2.0;
	inters.t_cap = -1.0;
	inters.denom = vec3_dot(ray.direction, inters.axis);
	check_cap(&inters, ray, 0);
	check_cap(&inters, ray, 1);
	if (!solve_quadratic(&inters, ray))
		return (inters.t_cap);
	t_con = validate_t(&inters, ray, inters.t1);
	if (t_con <= EPSILON)
		t_con = validate_t(&inters, ray, inters.t2);
	if (t_con > EPSILON && inters.t_cap > EPSILON)
		return (fmin(t_con, inters.t_cap));
	else if (t_con > EPSILON)
		return (t_con);
	return (inters.t_cap);
}
