/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 19:01:25 by lmartin           #+#    #+#             */
/*   Updated: 2019/11/24 02:19:01 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "miniRT.h"

s_cylinder	*new_cylinder(s_vector *vectors[2], float diameter, float height, s_vector *color)
{
	s_cylinder	*new;
	s_vector	*temp;
	float		total;

	if (!(new = malloc(sizeof(s_cylinder))))
		print_error_and_exit(-7);
	new->center = vectors[0];
	new->orientation = vectors[1];
	total = fabs(new->orientation->x) + fabs(new->orientation->y) + fabs(new->orientation->z);
	temp = new_vector(height/2 * (new->orientation->x / total), height/2 * (new->orientation->y / total), height/2 * (new->orientation->z / total));
	new->point1 = add_vectors(*new->center, *temp);
	free(temp);
	temp = new_vector(-height/2 * (new->orientation->x / total), -height/2 * (new->orientation->y / total), -height/2 * (new->orientation->z / total));
	new->point2 = add_vectors(*new->center, *temp);
	free(temp);
	new->diameter = diameter;
	new->height = distance_points(*new->point1, *new->point2);
	new->color = color;
	new->shiny = -1;
	new->difference = NULL;
	new->calcul_a = 1.0 / product_vectors(*new->orientation, *new->orientation);
	new->calcul_c = 0;
	new->calcul_d = 0;
	new->prev_origin = NULL;
	return (new);
}

void		free_cylinder(s_cylinder *cylinder)
{
	free(cylinder->center);
	free(cylinder->orientation);
	free(cylinder->point1);
	free(cylinder->point2);
	free(cylinder->color);
	if (cylinder->difference)
		free(cylinder->difference);
	if (cylinder->prev_origin)
		free(cylinder->prev_origin);
	free(cylinder);
}

s_cylinder	*cpy_cylinder(s_cylinder *cylinder)
{
	s_cylinder *new;

	if (!(new = malloc(sizeof(s_cylinder))))
		print_error_and_exit(-7);
	new->center = cpy_vector(cylinder->center);
	new->orientation = cpy_vector(cylinder->orientation);
	new->point1 = cpy_vector(cylinder->point1);
	new->point2 = cpy_vector(cylinder->point2);
	new->color = cpy_vector(cylinder->color);
	new->diameter = cylinder->diameter;
	new->height = cylinder->height;
	new->shiny = cylinder->shiny;
	new->difference = NULL;
	new->calcul_a = cylinder->calcul_a;
	new->calcul_c = 0;
	new->calcul_d = 0;
	new->prev_origin = NULL;
	return (new);
}
