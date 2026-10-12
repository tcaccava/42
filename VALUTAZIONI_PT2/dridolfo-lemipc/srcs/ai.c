#include "lemipc.h"

static const int	g_dx[4] = {0, 0, -1, 1};
static const int	g_dy[4] = {-1, 1, 0, 0};

typedef struct s_bfs
{
	short	first[BOARD_SIZE];
	short	queue[BOARD_SIZE];
	char	seen[BOARD_SIZE];
}	t_bfs;

/* 
	A tile the ipc may walk on: free, and not a spot where it would 
	get killed.
*/
static int	can_step(const t_shared *s, int x, int y, int team)
{
	t_pos	p;

	p.x = x;
	p.y = y;
	return (cell_at(s, x, y) == 0 && !is_surrounded(s, p, team));
}

/*
	BFS over 4-directional moves from our tile to the closest 
	free tile next to `goal`. Writes the first step of that path.
	The starting direction is randomized so ties don't always 
	break the same.
*/
static int	bfs_first_step(const t_shared *s, const t_game *g, t_pos goal,
		t_pos *step)
{
	t_bfs	b;
	t_pos	p;
	int		head;
	int		tail;
	int		rot;

	ft_bzero(b.seen, sizeof(b.seen));
	b.queue[0] = g->pos.y * BOARD_W + g->pos.x;
	b.first[b.queue[0]] = -1;
	b.seen[b.queue[0]] = 1;
	head = 0;
	tail = 1;
	rot = rand() % 4;
	while (head < tail)
	{
		int cur = b.queue[head++];

		p.x = cur % BOARD_W;
		p.y = cur / BOARD_W;
		if (b.first[cur] != -1 && chebyshev(p, goal) == 1)
		{
			step->x = b.first[cur] % BOARD_W;
			step->y = b.first[cur] / BOARD_W;
			return (1);
		}
		for (int k = 0; k < 4; k++)
		{
			int nx = p.x + g_dx[(k + rot) % 4];
			int ny = p.y + g_dy[(k + rot) % 4];
			int n = ny * BOARD_W + nx;

			if (!can_step(s, nx, ny, g->team) || b.seen[n])
				continue ;
			b.seen[n] = 1;
			b.first[n] = (b.first[cur] == -1) ? n : b.first[cur];
			b.queue[tail++] = n;
		}
	}
	return (0);
}

static int	random_step(const t_shared *s, const t_game *g, t_pos *step)
{
	int	rot;
	int	d;

	rot = rand() % 4;
	for (int k = 0; k < 4; k++)
	{
		d = (k + rot) % 4;
		if (can_step(s, g->pos.x + g_dx[d], g->pos.y + g_dy[d], g->team))
		{
			step->x = g->pos.x + g_dx[d];
			step->y = g->pos.y + g_dy[d];
			return (1);
		}
	}
	return (0);
}

static int	closer_teammates(const t_shared *s, const t_game *g, t_pos target)
{
	t_pos	p;
	int		mine;
	int		n;

	mine = manhattan(g->pos, target);
	n = 0;
	for (p.y = 0; p.y < BOARD_H; p.y++)
	{
		for (p.x = 0; p.x < BOARD_W; p.x++)
		{
			if (s->board[p.y][p.x] == g->team
				&& (p.x != g->pos.x || p.y != g->pos.y)
				&& manhattan(p, target) < mine)
				n++;
		}
	}
	return (n);
}

/*
	Two attackers are enough to kill: the two teammates closest 
	to the team target go for it, the others harass the 
	nearest other enemy instead.
*/
static t_pos	choose_goal(const t_shared *s, const t_game *g)
{
	t_pos	other;

	if (closer_teammates(s, g, g->target) >= 2
		&& nearest_enemy(s, g->pos, g->team, &g->target, &other))
		return (other);
	return (g->target);
}

/* 
	One move per lock: up, down, left or right. Hold position 
	once in place. 
*/
void	ai_play(t_game *g)
{
	t_shared	*s;
	t_pos		goal;
	t_pos		step;

	s = g->ipc.shm;
	if (!g->has_target)
		return ;
	goal = choose_goal(s, g);
	if (chebyshev(g->pos, goal) == 1)
		return ;
	if (!bfs_first_step(s, g, goal, &step) && !random_step(s, g, &step))
		return ;
	s->board[g->pos.y][g->pos.x] = 0;
	s->board[step.y][step.x] = g->team;
	g->pos = step;
}
