/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:46:56 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/27 10:53:20 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	fin_dinner(t_philo **philo)
{
	t_philo	*tmp;
	int		i;
	int		status;

	tmp = *philo;
	i = -1;
	while (++i < tmp->num_philo)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			while (i < tmp->num_philo)
			{
				kill(tmp->philo_id[i], SIGKILL);
				i++;
			}
			break ;
		}
	}
	sem_close(tmp->print_lock);
	sem_close(tmp->fork_lock);
	free(tmp->philo_id);
	free(tmp);
}

void	*monitoring(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (!(check_meals(philo)))
	{
		if (get_time() - philo->last_meal > philo->t_die)
		{
			print(philo, "died");
			philo->is_died = 1;
			exit(-1);
		}
		usleep(1000);
	}
	exit(0);
}

void	start_dinner(t_philo *philo)
{
	pthread_create(&philo->monitor, NULL, &monitoring, philo);
	if (philo->index % 2 == 1)
		usleep(1600);
	while (!philo->is_died)
	{
		print(philo, "is thinking");
		sem_wait(philo->fork_lock);
		print(philo, "has taken a fork");
		sem_wait(philo->fork_lock);
		print(philo, "has taken a fork");
		print(philo, "is eating");
		usleep(philo->t_eat * 1000);
		philo->last_meal = get_time();
		sem_post(philo->fork_lock);
		sem_post(philo->fork_lock);
		philo->ate++;
		print(philo, "is sleeping");
		usleep(philo->t_sleep * 1000);
	}
	pthread_join(philo->monitor, NULL);
}
