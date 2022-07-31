/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:45:46 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/27 10:53:38 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static t_philo	*init_data(int ac, char **av)
{
	t_philo	*tmp;

	tmp = (t_philo *)malloc(sizeof(t_philo));	//structa yer açtık
	tmp->num_philo = ft_atoi(av[1]);
	tmp->num_fork = tmp->num_philo;	//filozof sayısı kadar çatal var
	if (tmp->num_philo == 1)
	{
		printf("Philo 1 died.\n");
		exit(1);
	}
	tmp->t_die = ft_atoi(av[2]);
	tmp->t_eat = ft_atoi(av[3]);
	tmp->t_sleep = ft_atoi(av[4]);
	tmp->must_eat = 0;
	if (ac == 6)
		tmp->must_eat = ft_atoi(av[5]);
	tmp->is_died = 0;
	tmp->ate = 0;
	return (tmp);
}

t_philo	*init_philo(int ac, char **av)
{
	t_philo	*philo;

	philo = init_data(ac, av);
	philo->philo_id = (int *)malloc(sizeof(int) * philo->num_fork);	//her bir filozof için yer açıyoruz
	sem_unlink("/print_lock");	//semafor kullanımı biterse semaforun otomatik silinmesini istiyoruz
	sem_unlink("/fork_lock");
	philo->print_lock = sem_open("/print_lock", O_CREAT, 0644, 1); //2.parametre flag ilk kez oluşturduğumuz için O_CREAT
	philo->fork_lock = sem_open("/fork_lock", O_CREAT, 0644, philo->num_fork);		//3.parametre mode yani semafora erişim izni
	return (philo);											//4.parametre semafor başlangıç değeri
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	int		i;

	i = 0;
	if (ac != 5 && ac != 6)
	{
		printf("Error: Wrong number of arguments\n");
		exit(-1);
	}
	philo = init_philo(ac, av);
	check_args(philo);
	philo->start_time = get_time();
	while (i < philo->num_philo)
	{
		philo->philo_id[i] = fork(); //her bir filozof için fork işlemi yapılıp child process oluşturulur
		if (philo->philo_id[i] == 0) //pid ilk forklandığında 0 oluyor
		{
			philo->index = i + 1;
			philo->last_meal = get_time(); //bu 0 da atanabilir
			start_dinner(philo);
		}
		i++;
	}
	fin_dinner(&philo);
	return (0);
}
