/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakim <dakim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 14:13:11 by lmartin           #+#    #+#             */
/*   Updated: 2020/07/08 13:32:25 by dakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "minirt.h"

int			setup_l_vectors_and_calculate(t_lstobjects *closest_object,
t_vector direction, float closest_t, t_scene *scene)
{
	int						final_color;
	t_lightning_vectors		*l_vectors;
	t_vector				*temp;

	final_color = 0;
	if (!(l_vectors = malloc(sizeof(t_lightning_vectors))))
		print_error_and_exit(-7);
	temp = multiply_vectors(closest_t, direction);
	l_vectors->point = add_vectors(*(
(t_camera *)scene->cameras->object)->origin, *(temp));
	free(temp);
	l_vectors->normal = new_vector(0, 0, 0);
	l_vectors->view = multiply_vectors(-1, direction);
	final_color = compute_lightning(l_vectors, scene->lights,
scene, closest_object);
	if (scene->depth > 0 && closest_object->reflective > 0)
		final_color = color_with_reflect(closest_object, scene,
l_vectors, final_color);
	free_l_vectors(l_vectors);
	return (final_color);
}

float		closest_intersection(t_vector origin, t_vector direction,
t_scene *scene, t_lstobjects **closest_object)
{
	float					t_temp;
	float					closest_t;
	t_lstobjects			*objects;

	*closest_object = NULL;
	closest_t = -1;
	objects = scene->objects;
	while (objects)
	{
		t_temp = intersect_global(origin, direction, objects);
		// origin = cam origin driection = 로테이션이 수행된 값
		if (t_temp > scene->t_min && (t_temp < scene->t_max ||
scene->t_max == -1) && (t_temp < closest_t || closest_t == -1))
		{
			// 첫번쨰 루프 : t_min < t_temp < t_max 인경우
			// 두번째 이후 루프 t_min < t_temp이고 t_temp < closest_t인 경우
			closest_t = t_temp;
			*closest_object = objects;
		}
		scene->t_max = -1;
		objects = objects->next;
	}
	return (closest_t);
}

int			trace_ray(t_vector direction, t_scene *scene)
{
	float					closest_t;
	t_lstobjects			*closest_object;
	int						color;

	/*
	t_vector origin = *((t_camera*)scene->cameras->object)->origin;
	printf("origin = %f %f %f\n", origin.x, origin.y, origin.z);
	printf("direction = %f %f %f\n", direction.x, direction.y, direction.z);
	*/
	closest_t = closest_intersection(*(((t_camera *)scene->cameras->object)
->origin), direction, scene, &closest_object);
	if (g_x == 58 && g_y == 106)
	{
		t_vector origin = *((t_camera*)scene->cameras->object)->origin;
		printf("origin = %f %f %f\n", origin.x, origin.y, origin.z);
	}
	// 가장 가까운 물체를 찾는 함수
	// 인자로 origin direction을 받음
	// origin = 카메라의 시점
	// direction = 로테이트를 이용해서 얻은 값
	if (!closest_object)
		return (scene->background_color);
	color = setup_l_vectors_and_calculate(closest_object, direction,
closest_t, scene);
	return (color);
}
