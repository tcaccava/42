#include "lemipc.h"

#define MSG_SIZE	(sizeof(t_msg) - sizeof(long))

/*
	Team coordination over the message queue. 
	Each team keeps one message of type <team> in the queue 
	holding the tile of the enemy to attack.
	msgrcv() consumes messages, so every player reads the 
	team's message and posts it back on each turn. 
	All of this happens while the board lock is held, so a turn is 
	atomic with respect to the queue.
*/

/* 
	Drain every pending message for our team and keep the most 
	recent one 
*/
static int	read_team_target(t_game *g, t_pos *out)
{
	t_msg	msg;
	int		got;

	got = 0;
	while (msgrcv(g->ipc.msqid, &msg, MSG_SIZE, g->team,
			IPC_NOWAIT | MSG_NOERROR) != -1)
	{
		out->x = msg.x;
		out->y = msg.y;
		got = 1;
	}
	return (got);
}

static void	post_team_target(t_game *g)
{
	t_msg	msg;

	msg.mtype = g->team;
	msg.x = g->target.x;
	msg.y = g->target.y;
	msgsnd(g->ipc.msqid, &msg, MSG_SIZE, IPC_NOWAIT);
}

/*
	Keep the team's target while an enemy still stands there. 
	If it moved or died, retarget to the enemy closest to where 
	it was. Without any team target yet, pick our nearest enemy.
*/
void	comms_update_target(t_game *g)
{
	t_shared	*s;
	t_pos		last;
	t_pos		from;
	int			known;

	s = g->ipc.shm;
	known = read_team_target(g, &last);
	from = g->pos;
	if (known)
		from = last;
	if (known && is_enemy(cell_at(s, last.x, last.y), g->team))
		g->target = last;
	else if (!nearest_enemy(s, from, g->team, NULL, &g->target))
	{
		g->has_target = 0;
		return ;
	}
	g->has_target = 1;
	post_team_target(g);
}
