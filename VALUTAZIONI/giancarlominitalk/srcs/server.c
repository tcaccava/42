/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:58:19 by girizzi           #+#    #+#             */
/*   Updated: 2025/05/22 20:26:49 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	sig_handler(int sig, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	bit = 7;

	(void)context;
	if (sig == SIGUSR1)
		c |= (1 << bit);
	if (bit == 0)
	{
		if (c == '\0')
		{
			ft_putchar_fd('\n', 1);
			kill(info->si_pid, SIGUSR2);
		}
		else
			ft_putchar_fd(c, 1);
		c = 0;
		bit = 7;
	}
	else
		bit--;
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_printf("\033[32mPID: %d\n", getpid());
	ft_printf("\033[0m\n");
	while (1)
		pause();
}
