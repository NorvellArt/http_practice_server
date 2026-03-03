#include "server.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void run_response_socket(struct Server *server)
{
    int response_socket;
    socklen_t addressLength = sizeof(server->address);

    char buffer[BUFFER_SIZE];

    char response[] = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 15\r\n"
                          "\r\n"
                          "Hello, World!!!";

    while (1) {
        printf("\nWaiting for requests...\n");
        response_socket = accept(server->socket, (struct sockaddr *)&server->address, &addressLength);
        if (response_socket < 0) {
            perror("Error: accept");
            continue;
        }

        read(response_socket, buffer, BUFFER_SIZE);
        printf("Request: %s\n", buffer);
        write(response_socket, response, strlen(response));
        close(response_socket);
    }
}

int main()
{
    struct Server server = server_constructor(
        AF_INET,
        SOCK_STREAM,
        0,
        PORT,
        10,
        INADDR_LOOPBACK,
        run_response_socket
    );

    server.run_response_socket(&server);
}