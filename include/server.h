#ifndef SERVER_H
#define SERVER_H

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024

#include <sys/socket.h>
#include <netinet/in.h>

struct Server
{
    int domain;
    int service;
    int protocol;
    int port;
    int backlog;
    unsigned long interface;

    struct sockaddr_in address;

    int socket;

    void (*run_response_socket)(struct Server *server);
};

struct Server server_constructor(
    int domain,
    int service,
    int protocol,
    int port,
    int backlog,
    unsigned long interface,
    void (*launch)(struct Server *server)
);

#endif