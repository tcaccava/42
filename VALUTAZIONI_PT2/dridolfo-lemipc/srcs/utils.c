#include "lemipc.h"

/*
	Small output buffer: lines and frames are assembled in memory 
	and sent with a single write() so concurrent players don't 
	interleave their output.
*/

void	buf_str(t_buf *b, const char *s)
{
	size_t	n;

	n = ft_strlen(s);
	if (n > BUF_SIZE - b->len)
		n = BUF_SIZE - b->len;
	ft_memcpy(b->data + b->len, s, n);
	b->len += n;
}

void	buf_char(t_buf *b, char c)
{
	if (b->len < BUF_SIZE)
		b->data[b->len++] = c;
}

void	buf_nbr(t_buf *b, long n)
{
	char			digits[24];
	unsigned long	u;
	int				i;

	u = n;
	if (n < 0)
	{
		buf_char(b, '-');
		u = -(unsigned long)n;
	}
	i = 0;
	do
	{
		digits[i++] = '0' + u % 10;
		u /= 10;
	} while (u);
	while (i > 0)
		buf_char(b, digits[--i]);
}

void	buf_flush(t_buf *b, int fd)
{
	size_t	off;
	ssize_t	w;

	off = 0;
	while (off < b->len)
	{
		w = write(fd, b->data + off, b->len - off);
		if (w == -1 && errno == EINTR)
			continue ;
		if (w <= 0)
			break ;
		off += w;
	}
	b->len = 0;
}

void	print_error(const char *context)
{
	const char	*reason;
	t_buf		b;

	reason = strerror(errno);
	b.len = 0;
	buf_str(&b, "lemipc: ");
	buf_str(&b, context);
	buf_str(&b, ": ");
	buf_str(&b, reason);
	buf_char(&b, '\n');
	buf_flush(&b, STDERR_FILENO);
}
