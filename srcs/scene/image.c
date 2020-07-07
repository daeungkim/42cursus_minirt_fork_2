/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakim <dakim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 21:57:29 by lmartin           #+#    #+#             */
/*   Updated: 2020/07/07 16:25:01 by dakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
** Change pixel color by changing (char *data)
** (R) data[i]
** (G) data[i + 1]
** (B) data[i + 2]
*/

void	put_pixel(t_args *args, int y, int color)
{
	int	i;

	i = ((args->x + (args->scene->viewport->width / 2)) *
(args->bpp / 8)) + (y * args->size_line);
	pthread_mutex_lock(args->lock);
	args->data[i] = color;
	args->data[++i] = color >> 8;
	args->data[++i] = color >> 16;
	pthread_mutex_unlock(args->lock);
}

/*
** Function for each thread (1 thread == 1 column)
*/
#include <stdio.h>

void	*thread_function(void *arguments)
{
	t_vector		*obs;
	t_vector		*d;
	int				c;
	int				y;
	t_args			*args;

	args = (t_args *)arguments;
	if (!(t_camera *)args->scene->cameras)
		return (free_cpy_scene(args->scene) ? NULL : NULL);
		// 카메라 복사
	obs = ((t_camera *)args->scene->cameras->object)->origin;
	y = -(args->scene->viewport->height / 2) - 1;
	// y값도 내가 계산할 수 있음
	while (++y <= args->scene->viewport->height / 2)
	{
		d = new_vector(
args->x * (args->scene->viewplane->width / args->scene->viewport->width),
y * (args->scene->viewplane->height / args->scene->viewport->height), 1);
		//z를 1로 가정하였을 때 비율상의 값
		// 구할 수 있음
		if (args->x == 323 && y == -327)
		{
			printf("before %f %f %f\n", d->x, d->y, d->z);
		}
		rot(d, ((t_camera *)args->scene->cameras->object)->rotation);
		if (args->x == 323 && y == -327)
		{
			printf("after %f %f %f\n", d->x, d->y, d->z);
		}
		c = trace_ray(*d, args->scene);
		put_pixel(args, ((-(y - (args->scene->viewport->height / 2)))), c);
		free(d);
		((t_camera *)args->scene->cameras->object)->origin = obs;
		args->scene->depth = 3;
	}
	return (free_cpy_scene(args->scene) ? NULL : NULL);
}

/*
** Init a new t_args with all arguments for thread_function
*/

t_args	*new_s_args(t_mlx *my_mlx, int i, pthread_mutex_t *lock)
{
	t_args			*args;

	if (!(args = malloc(sizeof(t_args))))
		print_error_and_exit(-7);
	args->data = my_mlx->data;
	args->scene = cpy_scene(my_mlx->scene);
	args->x = -(my_mlx->scene->viewport->width / 2) + 1 + i;
	// 실제 x값을 얻기위한 처리
	// 이 값은 나도 구할 수 있음
	args->bpp = my_mlx->bpp;
	args->size_line = my_mlx->size_line;
	args->lock = lock;
	return (args);
}

/*
** Create a new image charging each column by a thread
*/

void	create_image(t_mlx *my_mlx)
{
	int				i;
	t_args			*args;
	pthread_t		*threads;
	pthread_mutex_t lock;

	if (!(threads = malloc(sizeof(pthread_t) * my_mlx->scene->viewport->width)))
		print_error_and_exit(-7);
	if (pthread_mutex_init(&lock, NULL) != 0)
		pthread_error(-9);
	i = -1;
	// i는 x를 의미함
	printf("viewplane = %f %f\n", my_mlx->scene->viewplane->width, my_mlx->scene->viewplane->height);
	printf("viewport = %f %f\n", my_mlx->scene->viewport->width, my_mlx->scene->viewport->height);
	while (++i < my_mlx->scene->viewport->width)
	{
		args = new_s_args(my_mlx, i, &lock);
		if (pthread_create(&threads[i], NULL, &thread_function, args))
			pthread_error(-10);
	}
	while (i--)
		if (pthread_join(threads[i], NULL))
			pthread_error(-11);
	pthread_mutex_destroy(&lock);
	i = my_mlx->scene->viewport->width;
	while (--i)
		wait(NULL);
}
