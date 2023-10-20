/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_test_begin(t_philo *philo)
{
	if (elapsed(philo, 0) > philo->args->eat_time * philo->pos)
	{
		if (my_usleep(philo, 0) == 1)
			return (-1);
		return (1);
	}
	return (0);
}

int	wait_test(t_philo *philo, int wait)
{
	if (elapsed(philo, 0) > ((philo->args->eat_time * philo->pos) + \
		(philo->args->eat_time * philo->args->philo_nb * wait)))
	{
		if (my_usleep(philo, 0) == 1)
			return (-1);
		wait++;
		return (wait);
	}
	return (wait);
}

int	odd_wait_test(t_philo *philo)
{
	if (philo->args->eat_time >= philo->args->sleep_time)
	{
		if (philo->wait_begin == 0)
		{
			philo->wait_begin = wait_test_begin(philo);
			if (philo->wait_begin == -1)
				return (1);
		}
		else
		{
			philo->wait = wait_test(philo, philo->wait);
			if (philo->wait == -1)
				return (1);
		}
	}
	else if (philo->args->eat_time < philo->args->sleep_time && \
		philo->name == philo->args->philo_nb && philo->wait_begin == 0)
	{
		if (my_usleep(philo, 0) == 1)
			return (1);
		philo->wait_begin++;
	}
	return (0);
}

int	die_test(t_philo *philo)
{
	if (philo->args->philo_nb == 1)
		return (0);
	pthread_mutex_lock(&philo->die_lock);
	if (philo->die == 1)
		return (unlock_forks_die(philo), 1);
	pthread_mutex_unlock(&philo->die_lock);
	philo->elapsed_eat = elapsed(philo, 1);
	philo->elapsed_start = elapsed(philo, 0);
	if (philo->elapsed_eat > philo->args->die_time)
	{
		if (philo->hfork == 1)
			pthread_mutex_unlock(&philo->fork);
		if (philo->hfork_r == 1)
			pthread_mutex_unlock(philo->fork_r);
		pthread_mutex_lock(&philo->die_lock);
		philo->die = 1;
		pthread_mutex_unlock(&philo->die_lock);
		return (1);
	}
	return (0);
}
