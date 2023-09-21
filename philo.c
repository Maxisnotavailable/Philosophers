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
	<sleep_time> [<meat_nb>]\n"), 1);
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
		args->meat_nb = ft_atoi(argv[5], 1);
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
		seconds = philo_received->end_time.tv_sec - philo_received->args.start_time.tv_sec;
		microseconds = philo_received->end_time.tv_usec - philo_received->args.start_time.tv_usec;
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
	philo_received->elapsed_eat = elapsed(philo_received, 1);
	philo_received->elapsed_start = elapsed(philo_received, 0);
	if (philo_received->elapsed_eat > philo_received->args.die_time)
	{
		printf("%d %d died\n", philo_received->elapsed_start, philo_received->name);
		return (1);
	}
	return (0);
}

int	get_fork(t_philo *philo_received)
{
	if (philo_received->name % 2 == 0)
	{
		pthread_mutex_lock(&philo_received->fork);
		printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	}
	else
	{
		pthread_mutex_lock(philo_received->fork_r);
		printf("%d %d has taken a fork R\n", elapsed(philo_received, 0), philo_received->name);
	}
	if (die_test(philo_received) == 1)
		exit (1);
		//return (1);
	//printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	if (philo_received->name % 2 == 0)
	{
		pthread_mutex_lock(philo_received->fork_r);
		printf("%d %d has taken a fork R\n", elapsed(philo_received, 0), philo_received->name);
	}
	else
	{
		pthread_mutex_lock(&philo_received->fork);
		printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	}
	if (die_test(philo_received) == 1)
		exit (1);
		//return (1);
	//printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
	return (0);
}

int	my_usleep(t_philo *philo_received, int i)
{
	if (i == 0)
		i = philo_received->args.eat_time;
	else if (i == 1)
		i = philo_received->args.sleep_time;
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

void	routine(void *arg)
{
	t_philo	*philo_received;
	int		i;
	int		time;

	(void)(philo_received);
	philo_received = (t_philo *)arg;
	i = philo_received->args.philo_nb;
	time = 0;
	if (philo_received->name % 2 != 0 && my_usleep(philo_received, 0) == 1)
		return ;
		//usleep(1000);
	/*if (philo_received->name == i && philo_received->args.philo_nb % 2 != 0)
		my_usleep(philo_received, 0);*/
		//usleep(philo_received->args.eat_time * 1000);
	while (1)
	{
		if (philo_received->args.philo_nb % 2 == 0)
		{
			if (die_test(philo_received) == 1)
				return ;
			if (get_fork(philo_received) == 1)
				return ;
			if (philo_received->eat_nb == 0)
			{
				philo_received->elapsed_start = elapsed(philo_received, 0);
				if (philo_received->elapsed_start > philo_received->args.die_time)
				{
					printf("%d %d START died\n", philo_received->elapsed_start, philo_received->name);
					return ;
				}
			}
			printf("%d %d is eating\n", elapsed(philo_received, 0), philo_received->name);
			gettimeofday(&philo_received->start_eat_time, NULL);
			//usleep(philo_received->args.eat_time * 1000);
			if (my_usleep(philo_received, 0) == 1)
				return ;
			philo_received->eat_nb++;
			pthread_mutex_unlock(&philo_received->fork);
			pthread_mutex_unlock(philo_received->fork_r);
			if (philo_received->args.meat_nb != 0 && philo_received->eat_nb >= philo_received->args.meat_nb)
				exit(1);
			printf("%d %d is sleeping\n", elapsed(philo_received, 0), philo_received->name);
			//usleep(philo_received->args.sleep_time * 1000);
			if (my_usleep(philo_received, 1) == 1)
				return ;
			printf("%d %d is thinking\n", elapsed(philo_received, 0), philo_received->name);
		}
		else
		{
			if (die_test(philo_received) == 1)
				return ;
			if (elapsed(philo_received, 0) >= time && philo_received->name == i )
			{
				printf("%d philo %d    i = %d\n",philo_received->elapsed_start, philo_received->name, i);
				my_usleep(philo_received, 0);
				//usleep(philo_received->args.eat_time * 1000);
				//i--;
				if (i == 0)
					i = philo_received->args.philo_nb;
			}
			/*if (philo_received->name == i)
			{
				printf("%d philo %d    i = %d\n",philo_received->elapsed_start, philo_received->name, i);
				my_usleep(philo_received, 0);
				//usleep(philo_received->args.eat_time * 1000);
				//i--;
				if (i == 0)
					i = philo_received->args.philo_nb;
			}*/

			if (get_fork(philo_received) == 1)
				return ;
			/*pthread_mutex_lock(&philo_received->fork);
			printf("%d %d has taken a fork\n", elapsed(philo_received, 0), philo_received->name);
			if (die_test(philo_received) == 1)
				return ;
			pthread_mutex_lock(philo_received->fork_r);
			printf("%d %d has taken a fork R\n", elapsed(philo_received, 0), philo_received->name);
			if (die_test(philo_received) == 1)
				return ;*/

			if (philo_received->eat_nb == 0)
			{
				philo_received->elapsed_start = elapsed(philo_received, 0);
				if (philo_received->elapsed_start > philo_received->args.die_time)
				{
					printf("%d %d START died\n", philo_received->elapsed_start, philo_received->name);
					return ;
				}
			}

			printf("%d %d is eating\n", elapsed(philo_received, 0), philo_received->name);
			gettimeofday(&philo_received->start_eat_time, NULL);
			//usleep(philo_received->args.eat_time * 1000);
			if (my_usleep(philo_received, 0) == 1)
				return ;
			philo_received->eat_nb++;
			i--;
			
			pthread_mutex_unlock(&philo_received->fork);
			pthread_mutex_unlock(philo_received->fork_r);



			if (philo_received->args.meat_nb != 0 && philo_received->eat_nb >= philo_received->args.meat_nb)
				exit(1);
			printf("%d %d is sleeping\n", elapsed(philo_received, 0), philo_received->name);
			//usleep(philo_received->args.sleep_time * 1000);
			if (my_usleep(philo_received, 1) == 1)
				return ;
			printf("%d %d is thinking\n", elapsed(philo_received, 0), philo_received->name);
		}
	}
}

void	printphilo(t_philo	*philo)
{
	printf("philo name : %d addr fork = %p addr fork_r = %p th = %ld\n", philo->name, (void *)&philo->fork, (void *)philo->fork_r, philo->th);
}

int	main (int argc, char **argv)
{
	int				i;
	int				ret;
	t_philo			*philo;
	t_args			args;


	if (parsing(argc, argv, &args) == 1)
		return (1);
	if (args.philo_nb == 1)
	{
		printf("0 1 died\n");
		exit(1);
	}
	philo = malloc(sizeof(t_philo) * args.philo_nb);
	i = -1;
	gettimeofday(&args.start_time, NULL);
	while (++i < args.philo_nb)
	{
		ret = pthread_mutex_init(&(philo[i].fork), NULL);
		if (ret != 0)
			return (1);
		philo[(i + args.philo_nb - 1) % args.philo_nb].fork_r = &philo[i].fork;
	}
	i = -1;
	while (++i < args.philo_nb)
	{
		philo[i].name = i + 1;
		philo[i].eat_nb = 0;
		philo[i].args = args;
		ret = pthread_create(&(philo[i].th), NULL, (void *)routine, &philo[i]);
		if (ret != 0)
			return (1);
	}
	


	i = -1;
	while (++i < args.philo_nb) 
		pthread_join(philo[i].th, NULL);
	free(philo);
	return (0);
}