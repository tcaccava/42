#ifndef LEMIPC_H
# define LEMIPC_H

# include <errno.h>
# include <signal.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/sem.h>
# include <sys/shm.h>
# include <sys/types.h>
# include <time.h>
# include <unistd.h>
# include "libft.h"

/* Board geometry */
# define BOARD_W		20
# define BOARD_H		12
# define BOARD_SIZE		(BOARD_W * BOARD_H)

/* Game tuning */
# define TEAM_MAX		99
# define TURN_US		150000
# define VIEW_US		100000
# define WAIT_US		500000
# define START_DELAY	3
# define JOIN_RETRIES	3

/* 
	System V IPC: one key shared by the semaphore set, 
	the shm and the msgq 
*/
# define IPC_KEY		0x4c454d49		// LEMI in ASCII 
# define IPC_PERMS		0666
# define SEM_LOCK		0
# define SEM_PLAYERS	1
# define SEM_COUNT		2

# define WINNER_NONE	0
# define WINNER_DRAW	-1

# define BUF_SIZE		(BOARD_SIZE * 16 + 4096)

typedef struct s_pos
{
	int	x;
	int	y;
}	t_pos;

/* 
	Everything that lives in the shared memory segment. 
	0 = empty tile. 
*/
typedef struct s_shared
{
	int		board[BOARD_H][BOARD_W];
	time_t	start_at;
	int		running;
	int		winner;
}	t_shared;

/* 
	Team message: mtype is the team number, payload is the tile to 
	attack 
*/
typedef struct s_msg
{
	long	mtype;
	int		x;
	int		y;
}	t_msg;

typedef struct s_ipc
{
	int			semid;
	int			shmid;
	int			msqid;
	t_shared	*shm;
}	t_ipc;

typedef struct s_game
{
	t_ipc	ipc;
	t_pos	pos;
	t_pos	target;
	int		team;
	int		has_target;
}	t_game;

typedef struct s_stats
{
	int	count[TEAM_MAX + 1];
	int	total;
	int	teams;
	int	armed;
}	t_stats;

typedef enum e_end
{
	END_NONE,
	END_KILLED,
	END_WON,
	END_DRAW,
	END_QUIT
}	t_end;

typedef struct s_buf
{
	char	data[BUF_SIZE];
	size_t	len;
}	t_buf;

extern volatile sig_atomic_t	g_stop;

/* ipc.c */
int		game_join(t_game *g);
int		game_leave(t_game *g);
int		ipc_attach_viewer(t_ipc *ipc);
int		ipc_lock(t_ipc *ipc);
int		ipc_unlock(t_ipc *ipc);

/* board.c */
int		cell_at(const t_shared *s, int x, int y);
int		is_enemy(int value, int team);
int		manhattan(t_pos a, t_pos b);
int		chebyshev(t_pos a, t_pos b);
int		is_surrounded(const t_shared *s, t_pos p, int team);
int		board_spawn(t_shared *s, t_game *g);
int		nearest_enemy(const t_shared *s, t_pos from, int team,
			const t_pos *skip, t_pos *out);
void	board_stats(const t_shared *s, t_stats *st);
void	update_phase(t_shared *s, const t_stats *st);

/* comms.c */
void	comms_update_target(t_game *g);

/* ai.c */
void	ai_play(t_game *g);

/* player.c / viewer.c */
int		run_player(int team);
int		run_viewer(void);

/* utils.c */
void	buf_str(t_buf *b, const char *s);
void	buf_char(t_buf *b, char c);
void	buf_nbr(t_buf *b, long n);
void	buf_flush(t_buf *b, int fd);
void	print_error(const char *context);

#endif
