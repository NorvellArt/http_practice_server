#ifndef SERVER_H
#define SERVER_H

#define _POSIX_C_SOURCE 200809L

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "router.h"

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define READ_CHUNK_SIZE 1024

typedef struct {
    int port;
    int server_fd;
    Router *router;
} Server;

void server_init(Server *server, int port, Router *router);
void server_run(Server *server);

#endif