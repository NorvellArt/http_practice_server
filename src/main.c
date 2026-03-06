#include "server.h"
#include "router.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void run_client_socket(Server *server)
{
    int client_socket;
    socklen_t addressLength = sizeof(server->address);

    char buffer[BUFFER_SIZE];

    char response[] = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 15\r\n"
                          "\r\n"
                          "Hello, World!!!";

    while (1) {
        printf("\nWaiting for requests...\n");
        client_socket = accept(server->socket, (struct sockaddr *)&server->address, &addressLength);
        if (client_socket < 0) {
            perror("Error: accept");
            continue;
        }

        read(client_socket, buffer, BUFFER_SIZE);
        printf("Request: %s\n", buffer);
        write(client_socket, response, strlen(response));
        close(client_socket);
    }
}

int main()
{
    Server server = server_constructor(
        AF_INET,
        SOCK_STREAM,
        0,
        PORT,
        10,
        INADDR_LOOPBACK,
        run_client_socket
    );

    Router *router = http_router_new();
    
    server.run_client_socket(&server);

    http_router_stop(router);

}