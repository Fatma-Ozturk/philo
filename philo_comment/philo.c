/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:45:46 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/25 10:46:00 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philo(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = malloc(sizeof(t_philo) * data->num_philo);		//her filozof için yer ayırtıyoruz
	while (i < data->num_philo)
	{
		philo[i].philo_id = i + 1;	//dokümanda filozof numaraları 1 den başlasın dediği için indexleri bir arttırıyoruz.
		philo[i].left_fork = i;		//solundaki çatalın indexi filozofun bir eksiği olacak.
		philo[i].right_fork = i + 1;
		philo[i].last_meal = get_time();	//filozofun son yemek yeme zamanını sistemden çekiyoruz.
		philo[i].ate = 0;
		philo[i].data_of_philo = data;		//her filozof için verileri ilk structtan alıyoruz.
		pthread_mutex_init(&data->fork_lock[i], NULL);	//çatal kilitlerini başlatıyoruz.
		i++;
	}
	i--;
	philo[i].right_fork = 0;	//son filozofun sağındaki çatal her zaman 0 numara
	return (philo);
}

void	init_data(t_data *data, int ac, char **av)
{
	data->num_philo = ft_atoi(av[1]);
	if (data->num_philo == 1)
	{
		printf("Philo 1 died.\n");
		exit(1);
	}
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	data->must_eat = 0;		//opsiyonel argüman verilmezse 0 kalacak
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->is_died = 0;
	data->sum_meal = 0;
	data->fork_lock = malloc(sizeof(pthread_mutex_t) * data->num_philo);	//her çatal kilidine(filozof sayısı kadar) yer açıyoruz
	pthread_mutex_init(&data->print_lock, NULL);	//yazdırma kilidi başlatılıyor
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philo;	//philo[]

	init_data(&data, ac, av);
	check_args(&data, ac);
	philo = init_philo(&data);
	start_dinner(philo);
	fin_dinner(philo, &data);
	return (0);
}
