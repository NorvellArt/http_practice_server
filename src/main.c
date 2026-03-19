#include "server.h"
#include "router.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void route_home_handler(int client_socket) {
    const char *body = "<html><body>Home</body></html>";
    http_router_write(client_socket, 200, "Content-Type: text/html\r\n", body);
    close(client_socket);
}

static void route_test_handler(int client_socket) {
    const char *body = "<html><body>Test</body></html>";
    http_router_write(client_socket, 200, "Content-Type: text/html\r\nX-Powered-By: C\r\n", body);
    close(client_socket);
}

int main()
{
    Router router;
    router_init(&router);
    router_add_route(&router, "GET", "/", route_home_handler);
    router_add_route(&router, "GET", "/test", route_test_handler);

    Server server;
    server_init(&server, 8080, &router);

    printf("Server started on port 8080...\n");

    server_run(&server);
}