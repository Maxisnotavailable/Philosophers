/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meals_finished_watcher(t_args *args, t_philo *philo)
{
	int	i;

	pthread_mutex_unlock(&args->meals_lock);
	i = -1;
	while (++i < args->philo_nb)
	{
		pthread_mutex_lock(&philo[i].die_lock);
		philo[i].die = 1;
		pthread_mutex_unlock(&philo[i].die_lock);
	}
	i = -1;
	while (++i < args->philo_nb)
		pthread_join(philo[i].th, NULL);
	free(philo);
}

int	philo_died_watcher(t_args *args, t_philo *philo, int i)
{
	pthread_mutex_unlock(&args->meals_lock);
	while (++i < args->philo_nb)
	{
		pthread_mutex_lock(&philo[i].die_lock);
		if (philo[i].die == 1)
		{
			pthread_mutex_unlock(&philo[i].die_lock);
			print_action(&philo[i], 0);
			i = -1;
			while (++i < args->philo_nb)
			{
				pthread_mutex_lock(&philo[i].die_lock);
				philo[i].die = 1;
				pthread_mutex_unlock(&philo[i].die_lock);
			}
			i = -1;
			while (++i < args->philo_nb)
				pthread_join(philo[i].th, NULL);
			free(philo);
			return (0);
		}
		pthread_mutex_unlock(&philo[i].die_lock);
	}
	return (1);
}

int	watcher(t_args *args, t_philo *philo)
{
	pthread_mutex_lock(&args->meals_lock);
	if (args->meals_finished >= args->philo_nb)
		return (meals_finished_watcher(args, philo), 0);
	else
	{
		if (philo_died_watcher(args, philo, -1) == 0)
			return (0);
	}
	usleep(1000);
	return (1);
}
