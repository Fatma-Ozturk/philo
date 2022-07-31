/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:46:12 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/25 10:46:18 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	current_time = get_time() - philo->data_of_philo->start_time;	//o anki zamandan sürecin başlangıç zamanı çıkınca ne kadar süredir masanın başındalar bulunur
	pthread_mutex_lock(&philo->data_of_philo->print_lock);		//outputlar tutarlı olsun diye kilit kullanıyoruz
	if (!philo->data_of_philo->is_died)
		printf("%3ld %3d %s\n", current_time, philo->philo_id, state);
	pthread_mutex_unlock(&philo->data_of_philo->print_lock);
}

long	get_time(void)
{
	struct timeval	time;		//"sys/time.h"de default tanımlı struct çağırıyoruz

	gettimeofday(&time, NULL);	//2.argüman saat dilimini belirtiyor.Posixte artık kullanılan bir argüman değil
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));		// tv_sec->seconds	tv_usec->microseconds
}																//(second*1000=milisaniye) + (microsecond/1000=milisaniye) //süreyi milisaniyeye çeviriyoruz

void	check_args(t_data *data, int ac)
{
	if (ac != 5 && ac != 6)
	{
		printf("Error: Incorrect count of argument.\n");
		exit(1);
	}
	if (data->num_philo < 1 || data->num_philo > 200 || data->t_die < 0 || \
		data->t_eat < 0 || data->t_sleep < 0)
	{
		printf("Error: Incorrect arguments.\n");
		exit(1);
	}
}

int	check_meals(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo->data_of_philo->must_eat > 0)	//opsiyonel argüman verildiyse
	{
		while (i < philo->data_of_philo->num_philo)
		{
			if (philo[i].ate >= philo->data_of_philo->must_eat)		//her filozof yemesi gereken miktarı yemiş mi yediyse sum_meal arttırılır.
				philo->data_of_philo->sum_meal++;
			i++;
		}
		if (philo->data_of_philo->sum_meal >= philo->data_of_philo->num_philo) //sum_meal filozof sayısına eşitse zaten her filozof yemesi gereken miktarı yemiş 
		{																		//ve is_died flagı ayarlanıp monitoring ve loop NULL döner.
			philo->data_of_philo->is_died = 1;
			return (1);
		}
	}
	return (0);
}
