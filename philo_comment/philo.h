/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faozturk <faozturk@42kocaeli.tr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 10:51:01 by faozturk          #+#    #+#             */
/*   Updated: 2022/07/27 10:51:06 by faozturk         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>	//sys->system call

typedef struct s_data
{
	int				num_philo;
	int				t_die;			//time_to_die->filozof bu kadar süre yemek yemezse ölecek
	int				t_eat;			//time_to_eat->yemeğe harcanacak zaman
	int				t_sleep;		//time_to_sleep->uyumaya harcanacak zaman
	int				must_eat;		//number_of_times_each_philosopher_must_eat->her filozof en az bu kadar yemek yedikten sonra process sonlanacaktır.
	int				sum_meal;		//her bir filozof için must_eat miktarı sağlandığında bir artacak en son filozof sayısı kadar olmasını bekleriz
	long			start_time;		//her bir filozofun yemeğe başladığı zaman
	int				is_died;
	pthread_mutex_t	print_lock;		//çıktılar birbirine karışmasın diye kilit
	pthread_mutex_t	*fork_lock;		//çatallar için koyulan kilit
}	t_data;

typedef struct s_philo
{
	pthread_t	thread;		//her filozof için ayrı oluşturacağımız thread
	long		last_meal;	//filozofun son yemek yeme zamanı
	int			ate;		//filozofun yediği yemek sayacı
	int			philo_id;
	int			left_fork;
	int			right_fork;
	t_data		*data_of_philo;		//üstteki structın verilerini çekmek için
}	t_philo;

//init
void	init_data(t_data *data, int ac, char **av);
t_philo	*init_philo(t_data *data);
//utils
void	check_args(t_data *data, int ac);
void	print(t_philo *philo, char *state);
long	get_time(void);
int		check_meals(t_philo *philo);
int		ft_atoi(char *str);
//dinner
void	fin_dinner(t_philo *philo, t_data *data);
void	eat_func(t_philo *philo);
void	*loop(void *ptr);
void	*monitoring(void *ptr);
void	start_dinner(t_philo *philo);

#endif
