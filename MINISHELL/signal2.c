/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:22:01 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:56:40 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_orig_sig_handlers(struct sigaction *sa_int,
		struct sigaction *sa_quit)
{
	sigaction(SIGINT, NULL, sa_int);
	sigaction(SIGQUIT, NULL, sa_quit);
}

void	set_default_signal_handlers(void)
{
	struct sigaction	sa_default;

	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

void	ignore_signals(void)
{
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, NULL);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

int	handle_child_termination_signals(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			return (131);
		}
		else if (sig == SIGINT)
		{
			return (130);
		}
	}
	return (WEXITSTATUS(status));
}
