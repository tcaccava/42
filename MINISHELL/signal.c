/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:24:37 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:24:39 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0;
	}
	else if (signum == SIGQUIT)
	{
		g_signal = signum;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Erreur configuration SIGINT");
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("Erreur configuration SIGQUIT");
		exit(1);
	}
	rl_catch_signals = 0;
}

void	handle_signals(void)
{
	if (g_signal == SIGINT)
	{
		write(STDOUT_FILENO, "\nla", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0;
	}
	else if (g_signal == SIGQUIT)
		g_signal = 0;
}
