#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <netinet/in.h>

typedef struct s_client
{
    int id;
    char *buf;
} t_client;

t_client clients[65536];
fd_set active, read_set;
int max_fd, next_id = 0, sockfd;

void fatal_error(void)
{
    write(2, "Fatal error\n", 12);
    exit(1);
}

char* ft_strdup(char *s){
    if(!s)
        return NULL;
    char *dup = malloc(strlen(s) + 1);
    if(!dup)
        return NULL;
    int i = 0;
    while(s[i]){
        dup[i] = s[i];
        i++;
    }
    dup[i] = 0;
    return dup;
}

char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (!buf && !add)
        return NULL;
    if (!buf)
        return ft_strdup(add);
    if (!add)
        return buf;
    len = strlen(buf) + strlen(add);
    newbuf = malloc(len + 1);
    if (!newbuf)
        return NULL;
    newbuf[0] = 0;
    if (buf)
        strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return newbuf;
}

int extract_message(char **buf, char **msg)
{
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return 0;
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                return -1;
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return 1;
        }
        i++;
    }
    return 0;
}

void send_all(int except, char *msg)
{
    for (int fd = 0; fd <= max_fd; fd++)
    {
        if (FD_ISSET(fd, &active) && fd != except && fd != sockfd)
            send(fd, msg, strlen(msg), 0);
    }
}
void add_client(int fd)
{
    char buf[128];

    if (fd > max_fd)
        max_fd = fd;
    clients[fd].id = next_id++;
    clients[fd].buf = NULL;
    FD_SET(fd, &active);
    sprintf(buf, "server: client %d just arrived\n", clients[fd].id);
    send_all(fd, buf);
}
void remove_client(int fd)
{
    char buf[128];

    sprintf(buf, "server: client %d just left\n", clients[fd].id);
    send_all(fd, buf);
    free(clients[fd].buf);
    clients[fd].buf = NULL;
    clients[fd].id = -1;
    FD_CLR(fd, &active);
    close(fd);
    if (fd == max_fd)
    {
        while (max_fd > sockfd && !FD_ISSET(max_fd, &active))
            max_fd--;
    }
}

void handle_message(int fd)
{
    char *msg;
    char buf[4096];

    while (extract_message(&clients[fd].buf, &msg))
    {
        sprintf(buf, "client %d: %s", clients[fd].id, msg);
        send_all(fd, buf);
        free(msg);
    }
}

int main(int ac, char **av)
{
    struct sockaddr_in addr;
    int fd;
    char buf[4096];

    if (ac != 2)
    {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_error();

    max_fd = sockfd;
    FD_ZERO(&active);
    FD_SET(sockfd, &active);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = htons(atoi(av[1]));

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        fatal_error();
    if (listen(sockfd, 128) < 0)
        fatal_error();

    bzero(clients, sizeof(clients));

    while (1)
    {
        read_set = active;

        if (select(max_fd + 1, &read_set, NULL, NULL, NULL) < 0)
            continue;// oppure fatal_error()

        for (fd = 0; fd <= max_fd; fd++)
        {
            if (!FD_ISSET(fd, &read_set))
                continue;

            if (fd == sockfd)
            {
                int new_fd = accept(sockfd, NULL, NULL);
                if (new_fd >= 0)
                    add_client(new_fd);
            }
            else
            {
                int r = recv(fd, buf, sizeof(buf) - 1, 0);
                if (r <= 0)
                {
                    remove_client(fd);
                }
                else
                {
                    buf[r] = 0;
                    clients[fd].buf = str_join(clients[fd].buf, buf);
                    if (!clients[fd].buf)
                        fatal_error();
                    handle_message(fd);
                }
            }
        }
    }
    return 0;
}