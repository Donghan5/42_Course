/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:48:42 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/16 15:02:04 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	Cylinder formula:
	||(R(t) - C) - ((R(t) - C)·A)A|| = r
	Where:
	- R(t) is any point on cylinder surface
	- C is cylinder center point
	- A is normalized axis vector
	- r is cylinder radius
	
	Ray formula:
	R(t) = O + tD
	Where:
	- O is ray origin
	- D is ray direction
	- t is distance along ray
	
	Final formula:
	(((O - C) + tD) - (((O - C) + tD)·A)A)·(((O - C) + tD)
		- (((O - C) + tD)·A)A) = r²
	at² + bt + c = 0
	Where:
	a = D·D - (D·A)²
	b = 2(D·(O-C) - (D·A)(O-C)·A)
	c = (O-C)·(O-C) - ((O-C)·A)² - r²
	
	(solved through quadratic equation)
	t = (-b - √discriminant) / (2a)
*/
static t_bool	solve_quadratic(t_intersection *inters, t_ray ray)
{
	t_vec3	oc;

	oc = vec3_sub(ray.origin, inters->center);
	inters->a = vec3_dot(ray.direction, ray.direction)
		- pow(vec3_dot(ray.direction, inters->axis), 2);
	inters->b = 2.0 * (vec3_dot(ray.direction, oc)
			- vec3_dot(ray.direction, inters->axis)
			* vec3_dot(oc, inters->axis));
	inters->c = vec3_dot(oc, oc) - pow(vec3_dot(oc, inters->axis), 2)
		- inters->radius * inters->radius;
	inters->discriminant = inters->b * inters->b - 4 * inters->a * inters->c;
	if (inters->discriminant < 0)
		return (0);
	inters->t1 = (-inters->b - sqrt(inters->discriminant)) / (2.0 * inters->a);
	if (inters->t1 < EPSILON)
		return (0);
	return (1);
}

static void	check_cap(t_intersection *inters, t_ray ray)
{
	double	t;
	t_vec3	p_center;

	if (fabs(inters->denom) <= EPSILON)
		return ;
	t = -vec3_dot(vec3_sub(ray.origin, inters->center), inters->axis)
		/ inters->denom;
	if (t > EPSILON)
	{
		inters->p = vec3_add(ray.origin, vec3_mul(ray.direction, t));
		inters->v = vec3_sub(inters->p, inters->center);
		if (vec3_dot(inters->v, inters->v) <= inters->radius * inters->radius)
			inters->t_cap = t;
	}
	p_center = vec3_add(inters->center, vec3_mul(inters->axis, inters->height));
	t = vec3_dot(vec3_sub(p_center, ray.origin), inters->axis) / inters->denom;
	if (t > EPSILON && (inters->t_cap < 0 || t < inters->t_cap))
	{
		inters->p = vec3_add(ray.origin, vec3_mul(ray.direction, t));
		inters->v = vec3_sub(inters->p, p_center);
		if (vec3_dot(inters->v, inters->v) <= inters->radius * inters->radius)
			inters->t_cap = t;
	}
}

double	intersect_cylinder(t_ray ray, t_cylinder *cyl)
{
	t_intersection	inters;
	t_vec3			hit_point;

	inters.axis = vec3_normalize(cyl->axis_vector);
	inters.center = cyl->coordinates;
	inters.radius = cyl->cy_diameter / 2.0;
	inters.t_cap = -1.0;
	inters.denom = vec3_dot(ray.direction, inters.axis);
	inters.height = cyl->cy_height;
	check_cap(&inters, ray);
	if (!solve_quadratic(&inters, ray))
		return (inters.t_cap);
	hit_point = vec3_add(ray.origin, vec3_mul(ray.direction, inters.t1));
	inters.height = vec3_dot(vec3_sub(hit_point, inters.center),
			inters.axis);
	if (inters.height < 0 || inters.height > cyl->cy_height)
		return (inters.t_cap);
	if (inters.t_cap < 0 || inters.t1 < inters.t_cap)
		return (inters.t1);
	return (inters.t_cap);
}
