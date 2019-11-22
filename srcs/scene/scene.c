/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmartin <lmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 06:43:09 by lmartin           #+#    #+#             */
/*   Updated: 2019/11/22 04:42:01 by lmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "camera.h"
#include "lstobjects.h"

s_scene		*new_empty_scene(int color)
{
	s_scene	*scene;

	scene = malloc(sizeof(s_scene));
	scene->viewport = NULL;
	scene->viewplane = NULL;
	scene->cameras = NULL;
	scene->objects = NULL;
	scene->lights = NULL;
	scene->total_intensity = 0;
	scene->background_color = color;
	scene->t_min = 0.001;
	scene->t_max = -1;
	scene->depth = 3;
	return (scene);
}

s_scene		*new_scene(s_lstobjects* cameras, s_lstobjects *objects,
s_lstobjects *lights, int color)
{
	s_scene	*scene;

	scene = malloc(sizeof(s_scene));
	scene->viewport = NULL;
	scene->viewplane = NULL;

	scene->cameras = cameras;
	scene->objects = objects;
	scene->lights = lights;
	scene->background_color = color;
	scene->total_intensity = 0;
	scene->t_min = 0.001;
	scene->t_max = -1;
	scene->depth = 3;
	return (scene);
}

s_scene		*cpy_scene(s_scene *scene)
{
	s_scene *new;
	s_lstobjects *cameras;
	s_lstobjects *objects;
	float	t_min;
	float	t_max;
	int		depth;
	int		i;

	t_min = scene->t_min;
	t_max = scene->t_max;
	depth = scene->depth;
	new = malloc(sizeof(s_scene));
	new->viewport = scene->viewport;
	new->viewplane = scene->viewplane;
	cameras = scene->cameras;
	new->cameras = NULL;
	i = 0;
	while (i < scene->nb_camera)
	{
		add_back(&(new->cameras), TYPE_CAMERA, cpy_camera(((s_camera *)cameras->object)));
		cameras = cameras->next;
		i++;
	}
	objects = scene->objects;
	new->objects = NULL;
	while (objects)
	{
		if (objects->type == TYPE_SPHERE)
			add_back(&(new->objects), objects->type, cpy_sphere(objects->object));
		if (objects->type == TYPE_PLAN)
			add_back(&(new->objects), objects->type, cpy_plan(objects->object));
		if (objects->type == TYPE_SQUARE)
			add_back(&(new->objects), objects->type, cpy_square(objects->object));
		if (objects->type == TYPE_TRIANGLE)
			add_back(&(new->objects), objects->type, cpy_triangle(objects->object));
		if (objects->type == TYPE_CYLINDER)
			add_back(&(new->objects), objects->type, cpy_cylinder(objects->object));
		objects = objects->next;
	}
	new->lights = scene->lights;
	new->total_intensity = scene->total_intensity;
	new->background_color = scene->background_color;
	new->t_min = t_min;
	new->t_max = t_max;
	new->depth = depth;
	return (new);
}

int		free_cpy_scene(s_scene *scene)
{
	int i;
	s_lstobjects *cameras;
	s_lstobjects *objects;

	i = 0;
	while (i < scene->nb_camera)
	{
		cameras = scene->cameras->next;
		free(scene->cameras);
		scene->cameras = cameras;
	}
	objects = scene->objects;
	while (objects)
	{
		objects = objects->next;
		if (scene->objects->type == TYPE_SPHERE)
			free_sphere((s_sphere *)scene->objects->object);
		if (scene->objects->type == TYPE_PLAN)
			free_plan((s_plan *)scene->objects->object);
		if (scene->objects->type == TYPE_SQUARE)
			free_square((s_square *)scene->objects->object);
		if (scene->objects->type == TYPE_TRIANGLE)
			free_triangle((s_triangle *)scene->objects->object);
		if (scene->objects->type == TYPE_CYLINDER)
			free_cylinder((s_cylinder *)scene->objects->object);
		free(scene->objects);
		scene->objects = objects;
	}
	free(scene);
	return (0);
}
