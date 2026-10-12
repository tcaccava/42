#include "lemipc.h"

volatile sig_atomic_t	g_stop = 0;

static void	on_signal(int sig)
{
	(void)sig;
	g_stop = 1;
}

/*
	No SA_RESTART: a signal interrupts usleep()/semop() so the 
	loop notices g_stop quickly and the player leaves the board 
	cleanly.
*/
static int	setup_signals(void)
{
	static const int	sigs[] = {SIGINT, SIGTERM, SIGHUP, SIGQUIT};
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	for (size_t i = 0; i < sizeof(sigs) / sizeof(*sigs); i++)
	{
		if (sigaction(sigs[i], &sa, NULL) == -1)
			return (-1);
	}
	signal(SIGPIPE, SIG_IGN);
	return (0);
}

static int	parse_team(const char *arg, int *team)
{
	long	value;

	if (*arg == '\0')
		return (-1);
	value = 0;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (-1);
		value = value * 10 + (*arg++ - '0');
		if (value > TEAM_MAX)
			return (-1);
	}
	if (value < 1)
		return (-1);
	*team = (int)value;
	return (0);
}

static int	usage(void)
{
	ft_putstr_fd("usage: ./lemipc <team>   join the game as a player of "
		"team 1-99\n"
		"       ./lemipc -v       watch the board (read-only viewer)\n",
		STDERR_FILENO);
	return (1);
}

int	main(int argc, char **argv)
{
	int	team;

	if (argc != 2)
		return (usage());
	if (setup_signals() == -1)
		return (print_error("sigaction"), 1);
	if (ft_strncmp(argv[1], "-v", 3) == 0)
		return (run_viewer());
	if (parse_team(argv[1], &team) == -1)
		return (usage());
	srand((unsigned int)(getpid() ^ time(NULL)));
	return (run_player(team));
}
