#ifndef SERVER_H
#define SERVER_H

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Server
{
    int domain;
    int service;
    int protocol;
    int port;
    int backlog;
    unsigned long interface;

    struct sockaddr_in address;

    int socket;

    void (*run_client_socket)(struct Server *server);
} Server;

Server server_constructor(
    int domain,
    int service,
    int protocol,
    int port,
    int backlog,
    unsigned long interface,
    void (*run_client_socket)(Server *server)
);

#endif