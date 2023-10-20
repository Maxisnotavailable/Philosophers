/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	unlock_forks_die(t_philo *philo)
{
	pthread_mutex_unlock(&philo->die_lock);
	if (philo->hfork == 1)
		pthread_mutex_unlock(&philo->fork);
	if (philo->hfork_r == 1)
		pthread_mutex_unlock(philo->fork_r);
	return (1);
}

void	take_fork(t_philo *philo, int x)
{
	if (x == 0)
	{
		pthread_mutex_lock(&philo->fork);
		philo->hfork = 1;
	}
	if (x == 1)
	{
		pthread_mutex_lock(philo->fork_r);
		philo->hfork_r = 1;
	}
}

int	get_fork(t_philo *philo)
{
	if (philo->name % 2 == 0)
		take_fork(philo, 0);
	else
		take_fork(philo, 1);
	if (die_test(philo) == 1)
		return (1);
	print_action(philo, 4);
	if (philo->name % 2 == 0)
		take_fork(philo, 1);
	else
		take_fork(philo, 0);
	if (die_test(philo) == 1)
		return (1);
	print_action(philo, 4);
	return (0);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->fork);
	philo->hfork = 0;
	pthread_mutex_unlock(philo->fork_r);
	philo->hfork_r = 0;
	philo->eat_nb++;
}

void	meals_nb_ok(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->meals_lock);
	philo->args->meals_finished++;
	pthread_mutex_unlock(&philo->args->meals_lock);
}
