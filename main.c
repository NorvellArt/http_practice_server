#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10

int main()
{
    int server_socket, response_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error: socket creation");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error: socket bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, BACKLOG) < 0)
    {
        perror("Error: socket listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port: %d\n", PORT);

    while (1) {
        printf("\nWaiting for requests...\n");
        response_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (response_socket < 0) {
            perror("Error: accept");
            continue;
        }

        printf("Request from: %s\n", inet_ntoa(client_addr.sin_addr));

        read(response_socket, buffer, sizeof(buffer) - 1);
        printf("Request: %s\n", buffer);
        char response[] = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 13\r\n"
                          "\r\n"
                          "Hello, World!";
        write(response_socket, response, strlen(response));
        close(response_socket);
    }

    close(server_socket);
    return 0;
}