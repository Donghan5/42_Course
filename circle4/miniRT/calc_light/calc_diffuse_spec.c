/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_diffuse_spec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:31:21 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 17:56:52 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	multiply_colors(t_color a, t_color b, double factor)
{
	t_color	result;

	result.r = (int)(a.r * b.r * factor / 255.0);
	result.g = (int)(a.g * b.g * factor / 255.0);
	result.b = (int)(a.b * b.b * factor / 255.0);
	return (result);
}

t_color	add_white_component(t_color base, double factor)
{
	t_color	result;

	result.r = base.r + (int)(255.0 * factor);
	result.g = base.g + (int)(255.0 * factor);
	result.b = base.b + (int)(255.0 * factor);
	return (result);
}

static t_color	calc_diffuse(t_light *light,
	t_hit_material closest, double shadow)
{
	t_color	diffuse_color;
	t_vec3	light_dir;
	double	diffuse;

	light_dir = vec3_normalize(vec3_sub(light->coordinates,
				closest.hit_point));
	diffuse = fmax(0.0, vec3_dot(closest.normal, light_dir))
		* closest.diffuse_strength
		* light->l_brightness * shadow;
	diffuse_color = multiply_colors(closest.base_color, light->color, diffuse);
	return (diffuse_color);
}

static t_color	calc_specular(t_light *light, t_hit_material closest,
	t_ray ray, double shadow)
{		
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	t_vec3	light_dir;
	t_color	spec_color;
	double	spec;

	light_dir = vec3_normalize(vec3_sub(light->coordinates,
				closest.hit_point));
	view_dir = vec3_normalize(vec3_sub(ray.origin, closest.hit_point));
	reflect_dir = vec3_reflect(vec3_mul(light_dir, -1.0), closest.normal);
	spec = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), closest.shininess)
		* closest.spec_intensity * shadow;
	spec_color = add_white_component(
			(t_color){0, 0, 0},
			spec * light->l_brightness
			);
	return (spec_color);
}

void	calc_diffuse_spec(t_hit_material closest, t_info *info, t_ray ray,
	t_color (*total_diff_spec)[2])
{
	t_light	*light;
	double	shadow;
	int		i;

	i = 0;
	while (i < info->scene.light_n)
	{
		light = info->scene.light[i];
		shadow = check_shadow(info, vec3_add(closest.hit_point,
					vec3_mul(closest.normal, EPSILON)), light->coordinates);
		(*total_diff_spec)[0] = t_color_add((*total_diff_spec)[0],
				calc_diffuse(light, closest, shadow));
		(*total_diff_spec)[1] = t_color_add((*total_diff_spec)[1],
				calc_specular(light, closest, ray, shadow));
		i++;
	}
}
