#include "lemipc.h"

/* Tile content, or -1 when (x, y) is outside the board */
int	cell_at(const t_shared *s, int x, int y)
{
	if (x < 0 || y < 0 || x >= BOARD_W || y >= BOARD_H)
		return (-1);
	return (s->board[y][x]);
}

int	is_enemy(int value, int team)
{
	return (value > 0 && value != team);
}

int	manhattan(t_pos a, t_pos b)
{
	return (abs(a.x - b.x) + abs(a.y - b.y));
}

int	chebyshev(t_pos a, t_pos b)
{
	int	dx;
	int	dy;

	dx = abs(a.x - b.x);
	dy = abs(a.y - b.y);
	if (dx > dy)
		return (dx);
	return (dy);
}

/*
	True when at least two players of the same enemy team 
	stand on the 8 tiles around p. Used for the death check 
	and, on hypothetical tiles, to avoid walking into a trap.
*/
int	is_surrounded(const t_shared *s, t_pos p, int team)
{
	int	seen[8];
	int	n;
	int	v;
	int	i;

	n = 0;
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			v = cell_at(s, p.x + dx, p.y + dy);
			if ((dx == 0 && dy == 0) || !is_enemy(v, team))
				continue ;
			for (i = 0; i < n; i++)
			{
				if (seen[i] == v)
					return (1);
			}
			seen[n++] = v;
		}
	}
	return (0);
}

static int	spawnable(const t_shared *s, int i, int team, int safe_only)
{
	t_pos	p;

	p.x = i % BOARD_W;
	p.y = i / BOARD_W;
	if (s->board[p.y][p.x] != 0)
		return (0);
	return (!safe_only || !is_surrounded(s, p, team));
}

static int	count_spawnable(const t_shared *s, int team, int safe_only)
{
	int	n;

	n = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
		n += spawnable(s, i, team, safe_only);
	return (n);
}

/*
	Drop the player on a random empty tile, preferring one 
	where it would not be dead on arrival. 
	Returns -1 when the board is full.
*/
int	board_spawn(t_shared *s, t_game *g)
{
	int	candidates;
	int	safe_only;
	int	pick;
	int	i;

	safe_only = 1;
	candidates = count_spawnable(s, g->team, safe_only);
	if (candidates == 0)
	{
		safe_only = 0;
		candidates = count_spawnable(s, g->team, safe_only);
	}
	if (candidates == 0)
		return (-1);
	pick = rand() % candidates;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		if (spawnable(s, i, g->team, safe_only) && pick-- == 0)
			break ;
	}
	g->pos.x = i % BOARD_W;
	g->pos.y = i / BOARD_W;
	s->board[g->pos.y][g->pos.x] = g->team;
	return (0);
}

/* 
	Closest enemy of `team` from `from`, ignoring the tile `skip` 
	if given.
*/
int	nearest_enemy(const t_shared *s, t_pos from, int team,
		const t_pos *skip, t_pos *out)
{
	t_pos	p;
	int		best;
	int		d;

	best = -1;
	for (p.y = 0; p.y < BOARD_H; p.y++)
	{
		for (p.x = 0; p.x < BOARD_W; p.x++)
		{
			if (!is_enemy(s->board[p.y][p.x], team)
				|| (skip && skip->x == p.x && skip->y == p.y))
				continue ;
			d = manhattan(from, p);
			if (best == -1 || d < best)
			{
				best = d;
				*out = p;
			}
		}
	}
	return (best != -1);
}

/* The board is the only source of truth about who is still playing */
void	board_stats(const t_shared *s, t_stats *st)
{
	int	v;

	ft_bzero(st, sizeof(*st));
	for (int y = 0; y < BOARD_H; y++)
	{
		for (int x = 0; x < BOARD_W; x++)
		{
			v = s->board[y][x];
			if (v <= 0 || v > TEAM_MAX)
				continue ;
			st->total++;
			if (++st->count[v] == 1)
				st->teams++;
			if (st->count[v] == 2)
				st->armed++;
		}
	}
}

/*
	The fight starts START_DELAY seconds after the board first 
	holds 2+ teams, one of which has 2+ players. The delay 
	gives the remaining players time to join. 
	Once started, it stays started.
*/
void	update_phase(t_shared *s, const t_stats *st)
{
	if (s->running)
		return ;
	if (st->teams < 2 || st->armed < 1)
		s->start_at = 0;
	else if (s->start_at == 0)
		s->start_at = time(NULL) + START_DELAY;
	else if (time(NULL) >= s->start_at)
		s->running = 1;
}
