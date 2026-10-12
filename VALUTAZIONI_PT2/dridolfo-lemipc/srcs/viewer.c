#include "lemipc.h"

#define NB_COLORS		12
#define COLOR_RESET		"\033[0m"
#define CLEAR_EOL		"\033[K\n"

static const char	*g_colors[NB_COLORS] = {
	"\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m",
	"\033[1;35m", "\033[1;36m", "\033[0;31m", "\033[0;32m",
	"\033[0;33m", "\033[0;34m", "\033[0;35m", "\033[0;36m"
};

static void	put_team(t_buf *b, int team, int pad)
{
	buf_str(b, g_colors[(team - 1) % NB_COLORS]);
	if (pad && team < 10)
		buf_char(b, ' ');
	buf_nbr(b, team);
	buf_str(b, COLOR_RESET);
}

static void	render_status(t_buf *b, const t_shared *s, int over)
{
	long	left;

	buf_str(b, "lemipc  ");
	if (s->winner > 0)
	{
		buf_str(b, "Team ");
		put_team(b, s->winner, 0);
		buf_str(b, " wins!");
	}
	else if (s->winner == WINNER_DRAW)
		buf_str(b, "Draw: no team can kill anymore");
	else if (over)
		buf_str(b, "Game over: every player left");
	else if (s->running)
		buf_str(b, "Fight!");
	else if (s->start_at)
	{
		left = s->start_at - time(NULL);
		buf_str(b, "Starting in ");
		buf_nbr(b, left > 0 ? left : 0);
		buf_str(b, "s...");
	}
	else
		buf_str(b, "Waiting for players (2+ teams, one with 2+ players)");
	buf_str(b, CLEAR_EOL);
}

static void	render_board(t_buf *b, const t_shared *s)
{
	int	v;

	buf_char(b, '+');
	for (int x = 0; x < BOARD_W * 3; x++)
		buf_char(b, '-');
	buf_str(b, "+" CLEAR_EOL);
	for (int y = 0; y < BOARD_H; y++)
	{
		buf_char(b, '|');
		for (int x = 0; x < BOARD_W; x++)
		{
			v = s->board[y][x];
			if (v > 0)
			{
				put_team(b, v, 1);
				buf_char(b, ' ');
			}
			else
				buf_str(b, " . ");
		}
		buf_str(b, "|" CLEAR_EOL);
	}
	buf_char(b, '+');
	for (int x = 0; x < BOARD_W * 3; x++)
		buf_char(b, '-');
	buf_str(b, "+" CLEAR_EOL);
}

static void	render_teams(t_buf *b, const t_shared *s)
{
	t_stats	st;

	board_stats(s, &st);
	buf_str(b, "Players: ");
	buf_nbr(b, st.total);
	buf_str(b, "   Teams:");
	for (int t = 1; t <= TEAM_MAX; t++)
	{
		if (st.count[t] == 0)
			continue ;
		buf_char(b, ' ');
		put_team(b, t, 0);
		buf_char(b, ':');
		buf_nbr(b, st.count[t]);
	}
	buf_str(b, CLEAR_EOL);
}

/* Whole frame built in memory and written at once to avoid flicker */
static void	render(const t_shared *s, int over)
{
	t_buf	b;

	b.len = 0;
	buf_str(&b, "\033[H");
	render_status(&b, s, over);
	render_board(&b, s);
	render_teams(&b, s);
	buf_str(&b, "\033[J");
	buf_flush(&b, STDOUT_FILENO);
}

static int	wait_for_game(t_ipc *ipc)
{
	int	shown;

	shown = 0;
	while (!g_stop)
	{
		if (ipc_attach_viewer(ipc) == 0)
			return (0);
		if (!shown)
			ft_putstr_fd("\033[H\033[2JWaiting for a game... (Ctrl-C to quit)\n",
				STDOUT_FILENO);
		shown = 1;
		usleep(WAIT_US);
	}
	return (-1);
}

/*
	Read-only display client: it never creates the game and never 
	writes to it. Players modify the board concurrently, 
	so every read still goes through the semaphore. 
	When the last player removes the IPCs our semop
	fails with EIDRM/EINVAL: no writer is left, and the segment 
	stays mapped until we detach, so one last unlocked read gives 
	the final result.
*/
int	run_viewer(void)
{
	t_ipc		ipc;
	t_shared	snap;
	int			over;

	ft_bzero(&snap, sizeof(snap));
	ft_putstr_fd("\033[?25l", STDOUT_FILENO);
	if (wait_for_game(&ipc) == 0)
	{
		ft_putstr_fd("\033[2J", STDOUT_FILENO);
		over = 0;
		while (!g_stop && !over)
		{
			if (ipc_lock(&ipc) == -1)
			{
				over = 1;
				ft_memcpy(&snap, ipc.shm, sizeof(snap));
			}
			else
			{
				ft_memcpy(&snap, ipc.shm, sizeof(snap));
				ipc_unlock(&ipc);
			}
			render(&snap, over);
			if (!over)
				usleep(VIEW_US);
		}
		shmdt(ipc.shm);
	}
	ft_putstr_fd("\033[?25h", STDOUT_FILENO);
	return (0);
}
