/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:46:12 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/27 10:54:18 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi(char *str)
{
	int	nbr;
	int	i;
	int	sign;

	nbr = 0;
	i = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] >= '0' && str[i] <= '9')
	{
		while (str[i] >= '0' && str[i] <= '9')
			nbr = (nbr * 10) + (str[i++] - '0');
	}
	else
	{
		printf("Error: Argument has non digit character.\n");
		exit(1);
	}
	return (nbr * sign);
}

void	print(t_philo *philo, char *state)
{
	long	current_time;

	current_time = get_time() - philo->start_time;
	sem_wait(philo->print_lock);
	if (!philo->is_died)
		printf("%5ld %3d %s\n", current_time, philo->index, state);
	sem_post(philo->print_lock);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	check_args(t_philo *philo)
{
	if (philo->num_philo < 1 || philo->num_philo > 200 || philo->t_die < 0 || \
		philo->t_eat < 0 || philo->t_sleep < 0)
	{
		printf("Error: Incorrect arguments.\n");
		exit(1);
	}
}

int	check_meals(t_philo *philo)
{
	if (philo->must_eat > 0)
	{
		if (philo->ate >= philo->must_eat)
		{
			philo->is_died = 1;
			return (1);
		}
	}
	return (0);
}
