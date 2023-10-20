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

int	main(int argc, char **argv)
{
	int				i;
	t_philo			*philo;
	t_args			args;

	if (parsing(argc, argv, &args) == 1)
		return (1);
	philo = malloc(sizeof(t_philo) * args.philo_nb);
	if (!philo)
		return (1);
	if (args.philo_nb == 1)
		return (alone_philo(args, philo), 0);
	if (mutex_create(&args, philo) == 1)
		return (1);
	if (philos_create(&args, philo) == 1)
		return (1);
	while (1)
	{
		if (watcher(&args, philo) == 0)
			return (0);
	}
	i = -1;
	while (++i < args.philo_nb)
		pthread_join(philo[i].th, NULL);
	free(philo);
	return (0);
}
