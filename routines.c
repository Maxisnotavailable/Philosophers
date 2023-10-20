/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	even_routine(t_philo *philo)
{
	if (die_test(philo) == 1)
		return (1);
	if (get_fork(philo) == 1)
		return (1);
	print_action(philo, 1);
	gettimeofday(&philo->start_eat_time, NULL);
	if (my_usleep(philo, 0) == 1)
		return (1);
	put_forks(philo);
	if (philo->args->meals_nb != 0 && philo->eat_nb == philo->args->meals_nb)
		meals_nb_ok(philo);
	if (die_test(philo) == 1)
		return (1);
	print_action(philo, 2);
	if (my_usleep(philo, 1) == 1)
		return (1);
	print_action(philo, 3);
	return (0);
}

int	odd_routine(t_philo *philo)
{
	if (die_test(philo) == 1)
		return (1);
	if (odd_wait_test(philo) == 1)
		return (1);
	if (get_fork(philo) == 1)
		return (1);
	print_action(philo, 1);
	gettimeofday(&philo->start_eat_time, NULL);
	if (my_usleep(philo, 0) == 1)
		return (1);
	put_forks(philo);
	if (philo->args->meals_nb != 0 && philo->eat_nb == philo->args->meals_nb)
		meals_nb_ok(philo);
	print_action(philo, 2);
	if (my_usleep(philo, 1) == 1)
		return (1);
	print_action(philo, 3);
	if ((philo->args->eat_time > philo->args->sleep_time) && \
		elapsed(philo, 0) + ((philo->args->sleep_time) > \
		(philo->args->eat_time * philo->pos)))
	{
		if (my_usleep(philo, 1) == 1)
			return (1);
	}
	return (0);
}

void	routine(void *arg)
{
	t_philo	*philo;

	(void)(philo);
	philo = (t_philo *)arg;
	philo->wait_begin = 0;
	philo->wait = 1;
	if (philo->name % 2 != 0 && my_usleep(philo, 0) == 1)
		return ;
	while (1)
	{
		if (philo->args->philo_nb % 2 == 0)
		{
			if (even_routine(philo) == 1)
				return ;
		}
		else
		{
			if (odd_routine(philo) == 1)
				return ;
		}
	}
}

void	alone_routine(void *arg)
{
	t_philo	*philo;

	(void)(philo);
	philo = (t_philo *)arg;
	printf("0 1 has taken a fork\n");
	my_usleep(philo, 2);
	printf("%d 1 died\n", philo->args->die_time);
	pthread_mutex_lock(&philo->die_lock);
	philo->die = 1;
	pthread_mutex_unlock(&philo->die_lock);
	return ;
}
