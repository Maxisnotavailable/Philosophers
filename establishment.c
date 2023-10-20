/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   establishment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parsing(int argc, char **argv, t_args *args)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("Error\nUsage: philo <philo_nb> <die_time> <eat_time> \
			<sleep_time> [<meals_nb>]\n"), 1);
	if (ft_atoi(argv[1], 1) == 2147483648 || (argc == 6 && \
	(ft_atoi(argv[5], 1) == 2147483648)))
		return (printf("Error\nInvalid argument(s)\n"), 1);
	i = 1;
	while (++i < 5)
	{
		if (ft_atoi(argv[i], 0) == 2147483648)
			return (printf("Error\nInvalid argument(s)\n"), 1);
	}
	args->philo_nb = ft_atoi(argv[1], 1);
	args->die_time = ft_atoi(argv[2], 0);
	args->eat_time = ft_atoi(argv[3], 0);
	args->sleep_time = ft_atoi(argv[4], 0);
	if (argc == 6)
		args->meals_nb = ft_atoi(argv[5], 1);
	return (0);
}

int	mutex_create(t_args *args, t_philo *philo)
{
	int	i;

	i = -1;
	gettimeofday(&args->start_time, NULL);
	while (++i < args->philo_nb)
	{
		if (pthread_mutex_init(&(philo[i].fork), NULL) != 0)
			return (1);
		philo[(i + args->philo_nb - 1) % args->philo_nb].fork_r = \
			&philo[i].fork;
		if (pthread_mutex_init(&(philo[i].die_lock), NULL) != 0)
			return (1);
	}
	if (pthread_mutex_init(&(args->meals_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(args->print_lock), NULL) != 0)
		return (1);
	return (0);
}

int	philos_create(t_args *args, t_philo *philo)
{
	int	i;

	i = -1;
	args->meals_finished = 0;
	while (++i < args->philo_nb)
	{
		philo[i].name = i + 1;
		philo[i].pos = args->philo_nb - i;
		philo[i].eat_nb = 0;
		philo[i].die = 0;
		philo[i].hfork = 0;
		philo[i].hfork_r = 0;
		philo[i].args = args;
		if (pthread_create(&(philo[i].th), NULL, (void *)routine, &philo[i]))
			return (1);
	}
	return (0);
}

int	alone_philo(t_args args, t_philo *philo)
{
	int	ret;

	ret = pthread_mutex_init(&(philo[0].die_lock), NULL);
	if (ret != 0)
		return (1);
	philo[0].args = &args;
	ret = pthread_create(&(philo[0].th), NULL, \
		(void *)alone_routine, &philo[0]);
	if (ret != 0)
		return (1);
	while (1)
	{
		pthread_mutex_lock(&philo[0].die_lock);
		if (philo->die == 1)
		{
			pthread_mutex_unlock(&philo[0].die_lock);
			pthread_join(philo[0].th, NULL);
			free(philo);
			return (1);
		}
		pthread_mutex_unlock(&philo[0].die_lock);
	}
	return (1);
}
