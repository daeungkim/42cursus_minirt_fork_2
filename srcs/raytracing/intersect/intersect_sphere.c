/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sphere.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakim <dakim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 13:02:59 by lmartin           #+#    #+#             */
/*   Updated: 2020/07/07 14:56:34 by dakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include <stdio.h>
#include <stdbool.h>


float		intersect_sphere(t_vector origin,
t_vector direction, t_sphere *object)
{
	float		discriminant;
	float		k[2];
	float		t[2];

	if (!object->prev_origin || !is_equal(object->prev_origin, &origin))
	{
		if (object->difference)
			free(object->difference);
		if (object->prev_origin)
			free(object->prev_origin);
		object->difference = subtract_vectors(origin, *object->center);
		object->calcul_c = product_vectors(*object->difference,
*object->difference) - (object->radius * object->radius);
		object->prev_origin = cpy_vector(&origin);
	}
	k[0] = product_vectors(direction, direction);
	k[1] = 2 * product_vectors(*object->difference, direction);
	discriminant = k[1] * k[1] - 4 * k[0] * object->calcul_c;
	if (discriminant < 0)
		return (0);
	t[0] = (-k[1] + sqrt(discriminant)) / (2 * k[0]);
	t[1] = (-k[1] - sqrt(discriminant)) / (2 * k[0]);
	if (t[0] < t[1])
		return (t[0]);
	return (t[1]);
}
