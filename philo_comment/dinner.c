/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 10:46:56 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/25 10:47:01 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fin_dinner(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_mutex_destroy(&data->fork_lock[i]); //çatal mutexleri destroy edilir
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);	//yazma mutexi destroy edilir
	free(data->fork_lock); //mallocla açtığımız yerler freelenir
	free(philo);
}

void	eat_func(t_philo *philo)
{
	pthread_mutex_lock(&philo->data_of_philo->fork_lock[philo->left_fork]);		//sol ve sağ çatal kilidi aktif edilir.
	pthread_mutex_lock(&philo->data_of_philo->fork_lock[philo->right_fork]);
	print(philo, "has taken left fork");
	print(philo, "has taken right fork");
	print(philo, "is eating");
	usleep(philo->data_of_philo->t_eat * 1000);
	philo->last_meal = get_time();	//yemek bitince son yemek yediği zamanı güncelliyoruz
	pthread_mutex_unlock(&philo->data_of_philo->fork_lock[philo->left_fork]);	//çatal kilitleri açılıyor.
	pthread_mutex_unlock(&philo->data_of_philo->fork_lock[philo->right_fork]);
}

void	*loop(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->philo_id % 2 == 0)	//çift numaralı filozofları bir miktar bekletiyoruz ve o arada tek numaralılar koşuyor.
		usleep(1600);
	while (philo->data_of_philo->is_died != 1)
	{
		eat_func(philo);
		print(philo, "is sleeping");
		usleep(philo->data_of_philo->t_sleep * 1000); //dışarıdan gelen süre milisaniye cinsi 1000 ile çarpıp microsaniye cinsi uyutuyoruz
		print(philo, "is thinking");  //hiçbir şey yapmıyorsa geri kalan her sürede düşünür.
		philo->ate++;	//yediği yemek sayacı bir arttırıyouz
	}
	return (NULL);
}

void	*monitoring(void *ptr)		//threadin gönderileceği fonksiyon parametre olarak sadece (void *) alır.
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)ptr;		//değişken türü dönüşümü yapılır.
	while (!check_meals(philo))  //opsiyonel argüman gönderildiyse filozoflar yemeleri gereeken miktar kadar yediler mi onun kontrolü
	{							//argüman yoksa her zaman 0 dönecek sonsuz döngüde çalışacak
		i = 0;
		while (i < philo->data_of_philo->num_philo)
		{
			if (get_time() - philo[i].last_meal > philo->data_of_philo->t_die)		//o anki zamandan, son yemek yenilen zamanı çıkarttığımızda filozofoun ne kadar süre aç olduğunu buluyoruz
			{																		//time_to_die süresini aştıysa filozof aç kalmış olacak ve ölecektir
				print(&philo[i], "died");
				philo->data_of_philo->is_died = 1;
				return (NULL);
			}
			i++;
		}
		usleep(1000); // 1 ms bekleyip check_meals fonksiyonuna gidecek
	}
	return (NULL);
}

void	start_dinner(t_philo *philo)
{
	int			i;
	pthread_t	monitor;	//sürekli gözlemleme yapacak bir thread daha tanımlıyoruz.

	i = -1;
	philo->data_of_philo->start_time = get_time();		//sürecin başladığı zaman
	while (++i < philo->data_of_philo->num_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, &loop, &philo[i]) != 0)	//2.parametre attr,threadin özelliklerini belirtir.NULL atanınca varsayılan özellikler yüklenir.
		{																	//4.parametre fonksiyona gönderilerecek parametrenin adresi
			printf("Failed to create thread");
			exit(1);														//fonksiyon başarılıysa 0 döner değilse error number döner.
		}
	}
	pthread_create(&monitor, NULL, &monitoring, philo);		//monitor threadi yaratılıyor.
	i = -1;
	while (++i < philo->data_of_philo->num_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)		//Threadin çalışmasının devamlılığını sağlar,process sonlanmaması için
		{													//2.parametre thread exit code
			printf("Failed to create thread");
			exit(2);
		}
	}
	pthread_join(monitor, NULL);		//monitor thread devamlılığı için
}
