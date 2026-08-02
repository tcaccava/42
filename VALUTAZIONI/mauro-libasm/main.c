/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbardett <mbardett@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 19:26:00 by mbardett          #+#    #+#             */
/*   Updated: 2026/08/01 20:42:20 by mbardett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

size_t ft_strlen(const char *s);
char *ft_strcpy(char *dest, const char *src);
int ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);
ssize_t ft_read(int fd, void *buf, size_t count);
char *ft_strdup(const char *s);

int tests_run = 0;
int tests_passed = 0;

void check_long(const char *label, long got, long expected)
{
    tests_run++;
    if (got == expected)
    {
        tests_passed++;
        printf("\033[38;5;46m[PASS] %s -> got %ld\033[0m\n", label, got);
    }
    else
        printf("\033[38;5;202m[FAIL] %s -> got %ld, expected %ld\033[0m\n", label, got, expected);
}

void check_str(const char *label, const char *got, const char *expected)
{
    tests_run++;
    if ((got == NULL && expected == NULL) ||
        (got && expected && strcmp(got, expected) == 0))
    {
        tests_passed++;
        printf("\033[38;5;46m[PASS] %s -> got \"%s\"\033[0m\n", label, got ? got : "(null)");
    }
    else
        printf("\033[38;5;202m[FAIL] %s -> got \"%s\", expected \"%s\"\033[0m\n",
               label, got ? got : "(null)", expected ? expected : "(null)");
}

char *make_long_string(size_t len)
{
    char *s = malloc(len + 1);
    memset(s, 'a', len);
    s[len] = '\0';
    return s;
}

void test_strlen(void)
{
    printf("--- ft_strlen ---\n");

    check_long("ft_strlen(\"\")", ft_strlen(""), strlen(""));

    char *longstr = make_long_string(5000);
    check_long("ft_strlen(5000 x 'a')", ft_strlen(longstr), strlen(longstr));
    free(longstr);
}

void test_strcpy(void)
{
    printf("--- ft_strcpy ---\n");

    char b1[16];
    char b2[16];
    ft_strcpy(b1, "");
    strcpy(b2, "");
    check_str("ft_strcpy(dest, \"\")", b1, b2);

    size_t len = 5000;
    char *longstr = make_long_string(len);
    char *b3 = malloc(len + 1);
    char *b4 = malloc(len + 1);
    ft_strcpy(b3, longstr);
    strcpy(b4, longstr);
    check_str("ft_strcpy(dest, 5000 x 'a')", b3, b4);
    free(longstr);
    free(b3);
    free(b4);
}

void test_strcmp(void)
{
    printf("--- ft_strcmp ---\n");

    check_long("ft_strcmp(\"\", \"\")", ft_strcmp("", ""), strcmp("", ""));

    long r1 = ft_strcmp("", "abc");
    long e1 = strcmp("", "abc");
    check_long("ft_strcmp(\"\", \"abc\") sign", (r1 > 0) - (r1 < 0), (e1 > 0) - (e1 < 0));

    long r2 = ft_strcmp("abc", "");
    long e2 = strcmp("abc", "");
    check_long("ft_strcmp(\"abc\", \"\") sign", (r2 > 0) - (r2 < 0), (e2 > 0) - (e2 < 0));

    long r3 = ft_strcmp("abc", "abd");
    long e3 = strcmp("abc", "abd");
    check_long("ft_strcmp(\"abc\", \"abd\") sign", (r3 > 0) - (r3 < 0), (e3 > 0) - (e3 < 0));

    long r4 = ft_strcmp("abd", "abc");
    long e4 = strcmp("abd", "abc");
    check_long("ft_strcmp(\"abd\", \"abc\") sign (swapped)", (r4 > 0) - (r4 < 0), (e4 > 0) - (e4 < 0));
}

void test_write(void)
{
    printf("--- ft_write ---\n");

    ssize_t real_ret = write(1, "ft_write stdout test\n", 22);
    ssize_t ft_ret = ft_write(1, "ft_write stdout test\n", 22);
    check_long("ft_write(stdout) return value", ft_ret, real_ret);

    const char *path = "/tmp/ft_asm_write_test.txt";
    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    ssize_t open_ret = ft_write(fd, "hello", 5);
    check_long("ft_write(open fd) return value", open_ret, 5);
    close(fd);
    unlink(path);

    errno = 0;
    ssize_t real_bad = write(-1, "x", 1);
    int real_errno = errno;
    errno = 0;
    ssize_t ft_bad = ft_write(-1, "x", 1);
    int ft_errno = errno;
    check_long("ft_write(wrong fd) return value", ft_bad, real_bad);
    check_long("ft_write(wrong fd) errno", ft_errno, real_errno);
}

void test_read(void)
{
    printf("--- ft_read ---\n");

    const char *path = "/tmp/ft_asm_read_test.txt";
    int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    write(fd, "hello", 5);
    close(fd);

    fd = open(path, O_RDONLY);
    char buf[42];
    memset(buf, 0, sizeof(buf));
    ssize_t open_ret = ft_read(fd, buf, sizeof(buf) - 1);
    check_long("ft_read(open fd) return value", open_ret, 5);
    check_str("ft_read(open fd) content", buf, "hello");
    close(fd);
    unlink(path);

    errno = 0;
    char dummy[8];
    ssize_t real_bad = read(-1, dummy, 8);
    int real_errno = errno;
    errno = 0;
    ssize_t ft_bad = ft_read(-1, dummy, 8);
    int ft_errno = errno;
    check_long("ft_read(wrong fd) return value", ft_bad, real_bad);
    check_long("ft_read(wrong fd) errno", ft_errno, real_errno);

    printf("Type something: ");
    fflush(stdout);
    memset(buf, 0, sizeof(buf));
    ssize_t stdin_ret = ft_read(0, buf, sizeof(buf) - 1);
    if (stdin_ret > 0 && buf[stdin_ret - 1] == '\n')
        buf[stdin_ret - 1] = '\0';
    printf("[INFO] ft_read(stdin) -> got %zd bytes: \"%s\"\n", stdin_ret, buf);
}

void test_strdup(void)
{
    printf("--- ft_strdup ---\n");

    char *d1 = ft_strdup("");
    check_str("ft_strdup(\"\")", d1, "");
    free(d1);

    char *longstr = make_long_string(5000);
    char *d2 = ft_strdup(longstr);
    check_str("ft_strdup(5000 x 'a')", d2, longstr);
    free(longstr);
    free(d2);
}

int main(void)
{
    test_strlen();
    test_strcpy();
    test_strcmp();
    test_write();
    test_read();
    test_strdup();

    printf("\n%d/%d tests passed\n", tests_passed, tests_run);

    return 0;
}