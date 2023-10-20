/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molla <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:50:57 by molla             #+#    #+#             */
/*   Updated: 2023/07/28 17:07:14 by molla            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str, int x)
{
	long	nb;
	int		sign;

	nb = 0;
	sign = 1;
	if (*str == '-')
		return (2147483648);
	while (*str >= '0' && *str <= '9')
		nb = (nb * 10) + (*str++ - 48);
	nb = nb * sign;
	if (x == 1 && (nb < -2147483648 || nb > 2147483647 || *str != '\0' || \
	nb == 0))
		return (2147483648);
	if (x == 0 && (nb < -2147483648 || nb > 2147483647 || *str != '\0'))
		return (2147483648);
	return (nb);
}

int	elapsed(t_philo *philo, int x)
{
	int	seconds;
	int	microseconds;
	int	elapsed;

	if (x == 0)
	{
		gettimeofday(&philo->end_time, NULL);
		seconds = philo->end_time.tv_sec - philo->args->start_time.tv_sec;
		microseconds = philo->end_time.tv_usec - \
			philo->args->start_time.tv_usec;
	}
	else
	{
		gettimeofday(&philo->end_eat_time, NULL);
		seconds = philo->end_eat_time.tv_sec - philo->start_eat_time.tv_sec;
		microseconds = philo->end_eat_time.tv_usec - \
			philo->start_eat_time.tv_usec;
	}
	elapsed = seconds * 1000.0 + microseconds / 1000.0;
	return (elapsed);
}

int	my_usleep(t_philo *philo, int i)
{
	if (i == 0)
		i = philo->args->eat_time;
	else if (i == 1)
		i = philo->args->sleep_time;
	else if (i == 2)
		i = philo->args->die_time;
	while (i > 0)
	{
		if (i > 10)
		{
			usleep(10000);
			if (die_test(philo) == 1)
				return (1);
			i -= 10;
		}
		else
		{
			usleep(i * 1000);
			if (die_test(philo) == 1)
				return (1);
			i = 0;
		}
	}
	return (0);
}

void	print_action(t_philo *philo, int x)
{
	if (x == 0)
	{
		pthread_mutex_lock(&philo->args->print_lock);
		printf("%d %d died\n", elapsed(philo, 0), philo->name);
		pthread_mutex_unlock(&philo->args->print_lock);
		return ;
	}
	pthread_mutex_lock(&philo->die_lock);
	if (philo->die == 0)
	{
		pthread_mutex_unlock(&philo->die_lock);
		pthread_mutex_lock(&philo->args->print_lock);
		if (x == 1)
			printf("%d %d is eating\n", elapsed(philo, 0), philo->name);
		else if (x == 2)
			printf("%d %d is sleeping\n", elapsed(philo, 0), philo->name);
		else if (x == 3)
			printf("%d %d is thinking\n", elapsed(philo, 0), philo->name);
		else if (x == 4)
			printf("%d %d has taken a fork\n", elapsed(philo, 0), philo->name);
		pthread_mutex_unlock(&philo->args->print_lock);
	}
}
