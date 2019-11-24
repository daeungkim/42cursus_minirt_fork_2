/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 03:51:43 by lmartin           #+#    #+#             */
/*   Updated: 2019/11/24 02:15:57 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "camera.h"
#include "vector.h"

s_camera		*new_camera(s_vector *pos, s_vector *rota, int fov)
{
	s_camera	*camera;

	if (!(camera = malloc(sizeof(s_camera))))
		print_error_and_exit(-7);
	camera->rotation = rota;
	camera->origin = pos;
	camera->fov = fov;
	return (camera);
}

s_camera		*cpy_camera(s_camera *cam)
{
	s_vector	*pos;
	s_vector	*rota;

	pos = new_vector(cam->origin->x, cam->origin->y, cam->origin->z);
	rota = new_vector(cam->rotation->x, cam->rotation->y, cam->rotation->z);
	return (new_camera(pos, rota, cam->fov));
}
