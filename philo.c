/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
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
	if (x == 1 && (nb < -2147483648 || nb > 2147483647 || *str != '\0' ||
		nb == 0))
		return (2147483648);
	if (x == 0 && (nb < -2147483648 || nb > 2147483647 || *str != '\0'))
		return (2147483648);
	return (nb);
}

int	parsing(int argc, char **argv, t_args *args)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("Error\nUsage: philo <philo_nb> <die_time> <eat_time> \
	<sleep_time> [<meals_nb>]\n"), 1);
	if (ft_atoi(argv[1], 1) == 2147483648 || (argc == 6 &&
		(ft_atoi(argv[5], 1) == 2147483648)))
		return(printf("Error\nInvalid argument(s)\n"), 1);
	i = 1;
	while (++i < 5)
	{
		if (ft_atoi(argv[i], 0) == 2147483648)
			return(printf("Error\nInvalid argument(s)\n"), 1);
	}
	args->philo_nb = ft_atoi(argv[1], 1);
	args->die_time = ft_atoi(argv[2], 0);
	args->eat_time = ft_atoi(argv[3], 0);
	args->sleep_time = ft_atoi(argv[4], 0);
	if (argc == 6)
		args->meals_nb = ft_atoi(argv[5], 1);
	if (args->philo_nb == 1)
		return (printf("0 1 died\n"), 1); ///// CREER THREAD ET MUTEX
	return (0);
}

int	elapsed(t_philo *philo_received, int x)
{
	int	seconds;
	int	microseconds;
	int	elapsed;

	if (x == 0)  // temps passé depuis le debut
	{
		gettimeofday(&philo_received->end_time, NULL);
		seconds = philo_received->end_time.tv_sec - philo_received->args->start_time.tv_sec;
		microseconds = philo_received->end_time.tv_usec - philo_received->args->start_time.tv_usec;
	}
	else  // temps passé depuis le debut du dernier repas
	{
		gettimeofday(&philo_received->end_eat_time, NULL);
		seconds = philo_received->end_eat_time.tv_sec - philo_received->start_eat_time.tv_sec;
		microseconds = philo_received->end_eat_time.tv_usec - philo_received->start_eat_time.tv_usec;
	}
	elapsed = seconds * 1000.0 + microseconds / 1000.0;
	return (elapsed);
}

int	die_test(t_philo *philo_received)
{
	pthread_mutex_lock(&philo_received->die_lock);
	if (philo_received->die == 1)
	{
		pthread_mutex_unlock(&philo_received->die_lock);
		if (philo_received->hfork == 1)
			pthread_mutex_unlock(&philo_received->fork);
		if (philo_received->hfork_r == 1)
			pthread_mutex_unlock(philo_received->fork_r);
		return (1);
	}
	pthread_mutex_unlock(&philo_received->die_lock);

	philo_received->elapsed_eat = elapsed(philo_received, 1);
	philo_received->elapsed_start = elapsed(philo_received, 0);
	if (philo_received->elapsed_eat > philo_received->args->die_time)
	{
		pthread_mutex_lock(&philo_received->args->print_lock);
		printf("%d %d died\n", philo_received->elapsed_start, philo_received->name);
		pthread_mutex_unlock(&philo_received->args->print_lock);
		if (philo_received->hfork == 1)
			pthread_mutex_unlock(&philo_received->fork);
		if (philo_received->hfork_r == 1)
			pthread_mutex_unlock(philo_received->fork_r);
		pthread_mutex_lock(&philo_received->die_lock);
		philo_received->die = 1;
		pthread_mutex_unlock(&philo_received->die_lock);
		return (1);
	}
	return (0);
}

int	my_usleep(t_philo *philo_received, int i)
{
	if (i == 0)
		i = philo_received->args->eat_time;
	else if (i == 1)
		i = philo_received->args->sleep_time;
	while (i > 0)
	{
		if (i > 10)
		{
			usleep(10000);
			if (die_test(philo_received) == 1)
				return (1);
			i -= 10;
		}
		else
		{
			usleep(i * 1000);
			if (die_test(philo_received) == 1)
				return (1);
			i = 0;
		}
	}
	return (0);
}

int	wait_test_begin(t_philo *philo_received)
{
	if (elapsed(philo_received, 0) > philo_received->args->eat_time * philo_received->pos)
	{
		if (my_usleep(philo_received, 0) == 1)
			return (-1);
		return (1);
	}
	return (0);
}

int	wait_test(t_philo *philo_received, int wait)
{
	if (elapsed(philo_received, 0) > ((philo_received->args->eat_time * philo_received->pos) + (philo_received->args->eat_time * philo_received->args->philo_nb * wait)))
	{
		if (my_usleep(philo_received, 0) == 1)
			return (-1);
		wait++;
		return (wait);
	}
	return (wait);
}

int	get_fork(t_philo *philo_received)
{
	if (philo_received->name % 2 == 0)
	{
		pthread_mutex_lock(&philo_received->fork);
		philo_received->hfork = 1;
	}
	else
	{
		pthread_mutex_lock(philo_received->fork_r);
		philo_received->hfork_r = 1;
	}
	if (die_test(philo_received) == 1)
		return (1);
	pthread_mutex_lock(&philo_received->args->print_lock);
	printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	pthread_mutex_unlock(&philo_received->args->print_lock);
	if (philo_received->name % 2 == 0)
	{
		pthread_mutex_lock(philo_received->fork_r);
		philo_received->hfork_r = 1;
	}
	else
	{
		pthread_mutex_lock(&philo_received->fork);
		philo_received->hfork = 1;
	}
	if (die_test(philo_received) == 1)
		return (1);
	pthread_mutex_lock(&philo_received->args->print_lock);
	printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	pthread_mutex_unlock(&philo_received->args->print_lock);
	return (0);
}

int	even_routine(t_philo *philo_received)
{
	if (die_test(philo_received) == 1)
		return (1);
	if (get_fork(philo_received) == 1)
		return (1);
	/*if (philo_received->eat_nb == 0)
	{
		philo_received->elapsed_start = elapsed(philo_received, 0);
		if (philo_received->elapsed_start > philo_received->args->die_time)
		{
			printf("%d %d died\n", philo_received->elapsed_start, philo_received->name); // UTILE ?
			return (1);
		}
	}*/
	pthread_mutex_lock(&philo_received->args->print_lock);
	printf("%d %d is eating\n", elapsed(philo_received, 0), philo_received->name);
	pthread_mutex_unlock(&philo_received->args->print_lock);

	gettimeofday(&philo_received->start_eat_time, NULL);
	if (my_usleep(philo_received, 0) == 1)
		return (1);
	
	pthread_mutex_unlock(&philo_received->fork);
	philo_received->hfork = 0;
	pthread_mutex_unlock(philo_received->fork_r);
	philo_received->hfork_r = 0;

	philo_received->eat_nb++;
	if (philo_received->args->meals_nb != 0 && philo_received->eat_nb == philo_received->args->meals_nb)
	{
		pthread_mutex_lock(&philo_received->args->meals_lock);
		philo_received->args->meals_finished++;
		pthread_mutex_unlock(&philo_received->args->meals_lock);
	}

	if (die_test(philo_received) == 1)
		return (1);

	pthread_mutex_lock(&philo_received->args->print_lock);
	printf("%d %d is sleeping\n", elapsed(philo_received, 0), philo_received->name);
	pthread_mutex_unlock(&philo_received->args->print_lock);
	if (my_usleep(philo_received, 1) == 1)
		return (1);
	pthread_mutex_lock(&philo_received->args->print_lock);
	printf("%d %d is thinking\n", elapsed(philo_received, 0), philo_received->name);
	pthread_mutex_unlock(&philo_received->args->print_lock);
	return (0);
}

void	routine(void *arg)
{
	t_philo	*philo_received;

	(void)(philo_received);
	philo_received = (t_philo *)arg;
	philo_received->wait_begin = 0;
	philo_received->wait = 1;
	if (philo_received->name % 2 != 0 && my_usleep(philo_received, 0) == 1)
		return ;
	while (1)
	{
		if (philo_received->args->philo_nb % 2 == 0)
		{
			if (even_routine(philo_received) == 1)
				return ;
		}
		else
		{
			if (die_test(philo_received) == 1)
				return ;

			if (philo_received->args->eat_time >= philo_received->args->sleep_time)
			{
				if (philo_received->wait_begin == 0)
				{
					philo_received->wait_begin = wait_test_begin(philo_received);
					if (philo_received->wait_begin == -1)
						return ;
				}
				else
				{
					philo_received->wait = wait_test(philo_received, philo_received->wait);
					if (philo_received->wait == -1)
						return ;
				}
			}
			else if (philo_received->args->eat_time < philo_received->args->sleep_time && philo_received->name == philo_received->args->philo_nb && philo_received->wait_begin == 0)
			{
				if (my_usleep(philo_received, 0) == 1)
					return ;
				philo_received->wait_begin++;
			}


			if (get_fork(philo_received) == 1)
				return ;

			/*if (philo_received->eat_nb == 0)///////
			{
				philo_received->elapsed_start = elapsed(philo_received, 0);
				if (philo_received->elapsed_start > philo_received->args->die_time)
				{
					printf("%d %d died\n", philo_received->elapsed_start, philo_received->name);

					return ;
				}
			}*/

			pthread_mutex_lock(&philo_received->args->print_lock);
			printf("%d %d is eating\n", elapsed(philo_received, 0), philo_received->name);
			pthread_mutex_unlock(&philo_received->args->print_lock);
			gettimeofday(&philo_received->start_eat_time, NULL);
			if (my_usleep(philo_received, 0) == 1)
				return ;

			pthread_mutex_unlock(&philo_received->fork);
			philo_received->hfork = 0;
			pthread_mutex_unlock(philo_received->fork_r);
			philo_received->hfork_r = 0;
			philo_received->eat_nb++;

			if (philo_received->args->meals_nb != 0 && philo_received->eat_nb == philo_received->args->meals_nb)
			{
				pthread_mutex_lock(&philo_received->args->meals_lock);
				philo_received->args->meals_finished++;
				pthread_mutex_unlock(&philo_received->args->meals_lock);
			}
			pthread_mutex_lock(&philo_received->args->print_lock);
			printf("%d %d is sleeping\n", elapsed(philo_received, 0), philo_received->name);
			pthread_mutex_unlock(&philo_received->args->print_lock);

			if (my_usleep(philo_received, 1) == 1)
				return ;

			pthread_mutex_lock(&philo_received->args->print_lock);
			printf("%d %d is thinking\n", elapsed(philo_received, 0), philo_received->name);
			pthread_mutex_unlock(&philo_received->args->print_lock);
			if ((philo_received->args->eat_time > philo_received->args->sleep_time) && elapsed(philo_received, 0) + ((philo_received->args->sleep_time) > (philo_received->args->eat_time * philo_received->pos)))
			{
				if (my_usleep(philo_received, 1) == 1)
					return ;
			}
		}
	}
}

int	main (int argc, char **argv)
{
	int				i;
	int				ret;
	t_philo			*philo;
	t_args			args;


	if (parsing(argc, argv, &args) == 1)
		return (1);
	philo = malloc(sizeof(t_philo) * args.philo_nb);
	args.meals_finished = 0;
	i = -1;
	gettimeofday(&args.start_time, NULL);
	while (++i < args.philo_nb)
	{
		ret = pthread_mutex_init(&(philo[i].fork), NULL);
		if (ret != 0)
			return (1);
		philo[(i + args.philo_nb - 1) % args.philo_nb].fork_r = &philo[i].fork;
		ret = pthread_mutex_init(&(philo[i].die_lock), NULL);
		if (ret != 0)
			return (1);
	}
	ret = pthread_mutex_init(&(args.meals_lock), NULL);
	if (ret != 0)
		return (1);
	i = -1;
	ret = pthread_mutex_init(&(args.print_lock), NULL);
	if (ret != 0)
		return (1);
	i = -1;
	while (++i < args.philo_nb)
	{
		philo[i].name = i + 1;
		philo[i].pos = args.philo_nb - i;
		philo[i].eat_nb = 0;
		philo[i].die = 0;
		philo[i].hfork = 0;
		philo[i].hfork_r = 0;
		philo[i].args = &args;
		ret = pthread_create(&(philo[i].th), NULL, (void *)routine, &philo[i]);
		if (ret != 0)
			return (1);
	}

	while (1)
	{
		//vérif si le plus petit nombre de eat a atteint meals_nb
		pthread_mutex_lock(&args.meals_lock);
		if (args.meals_finished >= args.philo_nb)
		{
			pthread_mutex_unlock(&args.meals_lock);
			i = -1;
			while (++i < args.philo_nb)
			{
				pthread_mutex_lock(&philo[i].die_lock);
				philo[i].die = 1;
				pthread_mutex_unlock(&philo[i].die_lock);
			}
			i = -1;
			while (++i < args.philo_nb) 
				pthread_join(philo[i].th, NULL);
			free(philo);
			return (0);
		}
		//vérif si un philo est mort -> 
		else
		{
			i = -1;
			while (++i < args.philo_nb)
			{
				pthread_mutex_lock(&philo[i].die_lock);
				if (philo[i].die == 1)
				{
					pthread_mutex_unlock(&philo[i].die_lock);
					i = -1;
					while (++i < args.philo_nb)
					{
						pthread_mutex_lock(&philo[i].die_lock);
						philo[i].die = 1;
						//printf(" +++  %d philo %d die = 1\n", elapsed(&philo[i], 0), i);
						pthread_mutex_unlock(&philo[i].die_lock);
					}
					i = -1;
					while (++i < args.philo_nb)
						pthread_join(philo[i].th, NULL);
					free(philo);
					return (0);
				}
				pthread_mutex_unlock(&philo[i].die_lock);
				
			}
		}
		pthread_mutex_unlock(&args.meals_lock);
		usleep(100);
	}

	i = -1;
	while (++i < args.philo_nb) 
		pthread_join(philo[i].th, NULL);
	free(philo);
	return (0);
}
