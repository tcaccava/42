#include "lemipc.h"

/* glibc leaves the definition of semun to the caller */
typedef union u_semun
{
	int				val;
	struct semid_ds	*buf;
	unsigned short	*array;
}	t_semun;

static int	sem_change(int semid, int num, int delta, int flags)
{
	struct sembuf	op;

	op.sem_num = num;
	op.sem_op = delta;
	op.sem_flg = flags;
	while (semop(semid, &op, 1) == -1)
	{
		if (errno != EINTR)
			return (-1);
	}
	return (0);
}

/*
	SEM_UNDO on the lock: if a process dies while holding it (even with
	SIGKILL), the kernel gives the token back and nobody deadlocks.
*/
int	ipc_lock(t_ipc *ipc)
{
	return (sem_change(ipc->semid, SEM_LOCK, -1, SEM_UNDO));
}

int	ipc_unlock(t_ipc *ipc)
{
	return (sem_change(ipc->semid, SEM_LOCK, 1, SEM_UNDO));
}

/*
	The first process to create the semaphore set owns the game creation.
	Linux zero-initializes new semaphores, so the lock is "held" until we
	SETVAL it to 1: anyone racing us blocks in semop() until then.
*/
static int	open_lock(t_ipc *ipc)
{
	t_semun	arg;

	ipc->semid = semget(IPC_KEY, SEM_COUNT, IPC_CREAT | IPC_EXCL | IPC_PERMS);
	if (ipc->semid != -1)
	{
		arg.val = 1;
		if (semctl(ipc->semid, SEM_LOCK, SETVAL, arg) == 0)
			return (0);
		semctl(ipc->semid, 0, IPC_RMID);
		return (-1);
	}
	if (errno != EEXIST)
		return (-1);
	ipc->semid = semget(IPC_KEY, SEM_COUNT, 0);
	if (ipc->semid == -1)
		return (-1);
	return (0);
}

static void	ipc_destroy(t_ipc *ipc)
{
	if (ipc->shmid != -1)
		shmctl(ipc->shmid, IPC_RMID, NULL);
	if (ipc->msqid != -1)
		msgctl(ipc->msqid, IPC_RMID, NULL);
	semctl(ipc->semid, 0, IPC_RMID);
}

/*
	Called with the lock held. If no player is registered anymore 
	we are the last one out and remove every IPC object. 
	Removing the semaphore set also wakes anyone blocked on the 
	lock with EIDRM. 
	Else just unlock.
	Returns 1 when the IPCs were destroyed.
*/
static int	ipc_release(t_ipc *ipc)
{
	int	last;

	last = (semctl(ipc->semid, SEM_PLAYERS, GETVAL) == 0);
	if (last)
		ipc_destroy(ipc);
	else
		ipc_unlock(ipc);
	if (ipc->shm)
		shmdt(ipc->shm);
	ipc->shm = NULL;
	return (last);
}

/* Called with the lock held, so creating shm/msgq here cannot race. */
static int	attach_locked(t_ipc *ipc)
{
	ipc->shmid = shmget(IPC_KEY, sizeof(t_shared), IPC_CREAT | IPC_PERMS);
	if (ipc->shmid == -1)
		return (print_error("shmget"), -1);
	ipc->msqid = msgget(IPC_KEY, IPC_CREAT | IPC_PERMS);
	if (ipc->msqid == -1)
		return (print_error("msgget"), -1);
	ipc->shm = shmat(ipc->shmid, NULL, 0);
	if (ipc->shm == (void *)-1)
	{
		ipc->shm = NULL;
		return (print_error("shmat"), -1);
	}
	return (0);
}

/*
	No registered player means either a brand new game or leftovers 
	from players that were killed abnormally: start from a clean 
	board and queue.
*/
static void	reset_game(t_ipc *ipc)
{
	t_msg	msg;

	ft_bzero(ipc->shm, sizeof(t_shared));
	while (msgrcv(ipc->msqid, &msg, sizeof(msg) - sizeof(long), 0,
			IPC_NOWAIT | MSG_NOERROR) != -1)
		;
}

static int	join_locked(t_game *g)
{
	t_ipc	*ipc;

	ipc = &g->ipc;
	if (attach_locked(ipc) == -1)
		return (-1);
	if (semctl(ipc->semid, SEM_PLAYERS, GETVAL) == 0)
		reset_game(ipc);
	else if (ipc->shm->winner != WINNER_NONE)
	{
		ft_putstr_fd("lemipc: a game is ending, try again in a moment\n",
			STDERR_FILENO);
		return (-1);
	}
	if (board_spawn(ipc->shm, g) == -1)
	{
		ft_putstr_fd("lemipc: the board is full\n", STDERR_FILENO);
		return (-1);
	}
	if (sem_change(ipc->semid, SEM_PLAYERS, 1, SEM_UNDO) == -1)
	{
		ipc->shm->board[g->pos.y][g->pos.x] = 0;
		return (print_error("semop"), -1);
	}
	return (0);
}

int	game_join(t_game *g)
{
	int	attempt;

	attempt = 0;
	while (attempt++ < JOIN_RETRIES)
	{
		g->ipc.shmid = -1;
		g->ipc.msqid = -1;
		g->ipc.shm = NULL;
		if (open_lock(&g->ipc) == -1 || ipc_lock(&g->ipc) == -1)
		{
			if (errno == EIDRM || errno == EINVAL || errno == ENOENT)
				continue ;
			return (print_error("semaphore"), -1);
		}
		if (join_locked(g) == 0)
			return (ipc_unlock(&g->ipc));
		ipc_release(&g->ipc);
		return (-1);
	}
	ft_putstr_fd("lemipc: could not join the game "
		"(stale IPC objects? check ipcs)\n", STDERR_FILENO);
	return (-1);
}

/*
	Called with the lock held. 
	Returns 1 if we were the last player.
	Once the game is decided the survivors leave their tile in place, 
	so the final board stays readable by the viewer.
*/
int	game_leave(t_game *g)
{
	t_shared	*s;

	s = g->ipc.shm;
	if (s->winner == WINNER_NONE && s->board[g->pos.y][g->pos.x] == g->team)
		s->board[g->pos.y][g->pos.x] = 0;
	sem_change(g->ipc.semid, SEM_PLAYERS, -1, SEM_UNDO | IPC_NOWAIT);
	return (ipc_release(&g->ipc));
}

/* 
	The viewer only attaches to an existing game, read-only, and never 
	creates 
*/
int	ipc_attach_viewer(t_ipc *ipc)
{
	ipc->msqid = -1;
	ipc->shm = NULL;
	ipc->semid = semget(IPC_KEY, SEM_COUNT, 0);
	if (ipc->semid == -1)
		return (-1);
	ipc->shmid = shmget(IPC_KEY, sizeof(t_shared), 0);
	if (ipc->shmid == -1)
		return (-1);
	ipc->shm = shmat(ipc->shmid, NULL, SHM_RDONLY);
	if (ipc->shm == (void *)-1)
	{
		ipc->shm = NULL;
		return (-1);
	}
	return (0);
}
