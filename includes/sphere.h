/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 07:09:23 by lmartin           #+#    #+#             */
/*   Updated: 2019/11/02 06:30:20 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SPHERE_H
# define SPHERE_H

# include <stdlib.h>
# include "vector.h"

typedef struct 	t_sphere
{
	float		radius;
	int			color;
	float		reflection;
	s_vector	*center;
}				s_sphere;

s_sphere		*new_sphere(float radius, int color,
float reflection, s_vector *center);
s_sphere		*new_default_sphere(float radius, int color);
void 			set_reflection(s_sphere *sphere, float reflection);

#endif
