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
	int				philo_nb;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				meals_nb;
	int				meals_finished;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	print_lock;
	struct timeval	start_time;
}	t_args;

typedef struct s_philo {
	int				name;
	int				pos;
	int				elapsed_start;
	int				elapsed_eat;
	int				eat_nb;
	int				wait_begin;
	int				wait;
	int				die;
	int				hfork;
	int				hfork_r;
	pthread_mutex_t	fork;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	die_lock;
	pthread_t		th;
	t_args			*args;
	struct timeval	end_time;
	struct timeval	start_eat_time;
	struct timeval	end_eat_time;
}	t_philo;

int		parsing(int argc, char **argv, t_args *args);
int		mutex_create(t_args *args, t_philo *philo);
int		philos_create(t_args *args, t_philo *philo);
int		alone_philo(t_args args, t_philo *philo);

void	routine(void *arg);
void	alone_routine(void *arg);

int		watcher(t_args *args, t_philo *philo);

int		odd_wait_test(t_philo *philo);
int		die_test(t_philo *philo);

long	ft_atoi(const char *str, int x);
int		elapsed(t_philo *philo, int x);
int		my_usleep(t_philo *philo, int i);
void	print_action(t_philo *philo, int x);

int		unlock_forks_die(t_philo *philo);
void	take_fork(t_philo *philo, int x);
int		get_fork(t_philo *philo);
void	put_forks(t_philo *philo);
void	meals_nb_ok(t_philo *philo);

#endif