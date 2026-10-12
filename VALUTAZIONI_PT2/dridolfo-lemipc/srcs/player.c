#include "lemipc.h"

static void	announce(const t_game *g, const char *what, int with_pos)
{
	t_buf	b;

	b.len = 0;
	buf_str(&b, "[");
	buf_nbr(&b, getpid());
	buf_str(&b, "] team ");
	buf_nbr(&b, g->team);
	buf_str(&b, ": ");
	buf_str(&b, what);
	if (with_pos)
	{
		buf_str(&b, " at (");
		buf_nbr(&b, g->pos.x);
		buf_str(&b, ", ");
		buf_nbr(&b, g->pos.y);
		buf_str(&b, ")");
	}
	buf_char(&b, '\n');
	buf_flush(&b, STDOUT_FILENO);
}

/*
	One turn, played while holding the board lock. 
	The death check comes first: a surrounded player must leave 
	before trying to move.
*/
static t_end	play_turn(t_game *g)
{
	t_shared	*s;
	t_stats		st;

	s = g->ipc.shm;
	if (g_stop)
		return (END_QUIT);
	if (is_surrounded(s, g->pos, g->team))
		return (END_KILLED);
	board_stats(s, &st);
	update_phase(s, &st);
	if (!s->running)
		return (END_NONE);
	if (st.teams == 1)
	{
		s->winner = g->team;
		return (END_WON);
	}
	if (st.armed == 0)
	{
		s->winner = WINNER_DRAW;
		return (END_DRAW);
	}
	comms_update_target(g);
	ai_play(g);
	return (END_NONE);
}

static void	announce_end(const t_game *g, t_end end)
{
	if (end == END_KILLED)
		announce(g, "surrounded and killed", 1);
	else if (end == END_WON)
		announce(g, "victory, last team standing!", 0);
	else if (end == END_DRAW)
		announce(g, "draw, no team can kill anymore", 0);
	else
		announce(g, "left the game", 0);
}

int	run_player(int team)
{
	t_game	g;
	t_end	end;

	ft_bzero(&g, sizeof(g));
	g.team = team;
	if (game_join(&g) == -1)
		return (1);
	announce(&g, "joined", 1);
	end = END_NONE;
	while (end == END_NONE)
	{
		if (ipc_lock(&g.ipc) == -1)
		{
			print_error("lost access to the game");
			shmdt(g.ipc.shm);
			return (1);
		}
		end = play_turn(&g);
		if (end != END_NONE)
			break ;
		ipc_unlock(&g.ipc);
		usleep(TURN_US);
	}
	announce_end(&g, end);
	if (game_leave(&g))
		announce(&g, "last player out, IPC resources removed", 0);
	return (0);
}
