/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 10:22:06 by molla             #+#    #+#             */
/*   Updated: 2023/04/28 17:01:52 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_args {
	int 			philo_nb;
	int 			die_time;
	int 			eat_time;
	int 			sleep_time;
	int 			meals_nb;
	int				meals_finished;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	print_lock;
	struct timeval	start_time;
}   t_args;

typedef struct s_philo {
	int        		name;
	int				pos;
	int		 		elapsed_start;
	int				elapsed_eat;
	int				eat_nb;
	int				wait_begin;
	int				wait;
	int				die;
	int				hfork;
	int				hfork_r;
	pthread_mutex_t	fork;
	pthread_mutex_t	*fork_r;
	//pthread_mutex_t	meals_lock;
	pthread_mutex_t	die_lock;
	pthread_t		th;
	t_args    		*args;
	struct timeval	end_time;
	struct timeval	start_eat_time;
	struct timeval	end_eat_time;
}   t_philo;

#endif