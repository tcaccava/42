#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dst, const char *src);
int	ft_strcmp(const char *s1, const char *s2);
ssize_t	ft_write(int fd, const void *buf, size_t count);
ssize_t	ft_read(int fd, void *buf, size_t count);
char	*ft_strdup(const char *s);

static int g_fail = 0;

static void check(const char *name, int ok)
{
	printf("%-40s %s\n", name, ok ? "OK" : "KO");
	if (!ok)
		g_fail = 1;
}

static int sign(int n)
{
	return (n > 0) - (n < 0);
}

static void test_strlen(void)
{
	const char *cases[] = {"", "a", "ciao", "ciao mondo",
		"una frase un po' piu lunga per sicurezza", NULL};

	for (int i = 0; cases[i]; i++)
	{
		char name[64];
		snprintf(name, sizeof(name), "ft_strlen(\"%.20s\")", cases[i]);
		check(name, ft_strlen(cases[i]) == strlen(cases[i]));
	}
}

static void test_strcpy(void)
{
	const char *cases[] = {"", "a", "ciaogfjkwjgkfjgkfkgjkfjgfjdkgjfdkgjfdjgkjfkgjfdgjkdfjgkf mondo", NULL};
	char dst1[64];
	char dst2[64];

	for (int i = 0; cases[i]; i++)
	{
		memset(dst1, 'X', sizeof(dst1));
		memset(dst2, 'X', sizeof(dst2));
		char *r1 = ft_strcpy(dst1, cases[i]);
		char *r2 = strcpy(dst2, cases[i]);
		char name[64];
		snprintf(name, sizeof(name), "ft_strcpy(\"%.20s\")", cases[i]);
		check(name, r1 == dst1 && (r2 == dst2)
			&& memcmp(dst1, dst2, strlen(cases[i]) + 1) == 0);
	}
}

static void test_strcmp(void)
{
	const char *pairs[][2] = {
		{"", "libero"}, {"a", "a"}, {"a", "b"}, {"b", "a"},
		{"cane", "casa"}, {"ciao", "ciaoo"}, {"ciaoo", "ciao"},
		{"pippo", "pippo"},
	};

	for (size_t i = 0; i < sizeof(pairs) / sizeof(pairs[0]); i++)
	{
		char name[64];
		snprintf(name, sizeof(name), "ft_strcmp(\"%.10s\", \"%.10s\")",
			pairs[i][0], pairs[i][1]);
		check(name, sign(ft_strcmp(pairs[i][0], pairs[i][1]))
			== sign(strcmp(pairs[i][0], pairs[i][1])));
	}
}

static void test_write(void)
{
	ssize_t r = ft_write(1, "prova scrittura\n", 16);
	check("ft_write(1, ..., 16) == 16", r == 16);

	errno = 0;
	r = ft_write(-1, "x", 1);
	check("ft_write(-1) -> -1 / EBADF", r == -1 && errno == EBADF);

	errno = 0;
	write(-1, "x", 1);
	int errno_libc = errno;
	errno = 0;
	ft_write(-1, "x", 1);
	check("errno identico alla libc", errno == errno_libc);
}

static void test_read(void)
{
	char buf[64];

	int fd = open("/etc/hostname", O_RDONLY);
	if (fd >= 0)
	{
		char buf2[64];
		ssize_t r1 = ft_read(fd, buf, 10);
		lseek(fd, 0, SEEK_SET);
		ssize_t r2 = read(fd, buf2, 10);
		check("ft_read(file, 10) come read", r1 == r2
			&& (r1 <= 0 || memcmp(buf, buf2, r1) == 0));
		close(fd);
	}

	errno = 0;
	ssize_t r = ft_read(-1, buf, 1);
	check("ft_read(-1) -> -1 / EBADF", r == -1 && errno == EBADF);

	check("ft_read(0, buf, 0) == 0", ft_read(0, buf, 0) == 0);
}

static void test_strdup(void)
{
	const char *cases[] = {"", "a", "ciao mondo", NULL};

	for (int i = 0; cases[i]; i++)
	{
		char *dup = ft_strdup(cases[i]);
		char name[64];
		snprintf(name, sizeof(name), "ft_strdup(\"%.20s\")", cases[i]);
		check(name, dup != NULL && dup != cases[i]
			&& strcmp(dup, cases[i]) == 0);
		free(dup);
	}
}

int main(void)
{
	printf("--- ft_strlen ---\n");
	test_strlen();
	printf("--- ft_strcpy ---\n");
	test_strcpy();
	printf("--- ft_strcmp ---\n");
	test_strcmp();
	printf("--- ft_write ---\n");
	test_write();
	printf("--- ft_read ---\n");
	test_read();
	printf("--- ft_strdup ---\n");
	test_strdup();

	printf("\n%s\n", g_fail ? ">>> QUALCOSA NON VA <<<" : ">>> TUTTO OK <<<");
	return g_fail;
}
