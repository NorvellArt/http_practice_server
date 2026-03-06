#include "server.h"

#include <stdio.h>
#include <stdlib.h>

Server server_constructor(
    int domain,
    int service,
    int protocol,
    int port,
    int backlog,
    unsigned long interface,
    void (*run_client_socket)(Server *server))
{
    Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;
    server.interface = interface;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);

    if (server.socket == 0)
    {
        perror("failed to connect socket!");
        exit(1);
    }

    if ((bind(server.socket,
              (struct sockaddr *)&server.address,
              sizeof(server.address))) < 0)
    {
        perror("failed to bind socket!");
        exit(1);
    }

    if ((listen(server.socket, server.backlog)) < 0)
    {
        perror("failed to start listening!");
        exit(1);
    }

    server.run_client_socket = run_client_socket;

    return server;
}