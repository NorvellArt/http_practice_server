#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#define INITIAL_ROUTE_CAPACITY 10

typedef void (*http_handler)(int client_socket);

typedef struct Route {
    char *path;
    char *method;
    http_handler handler;
} Route;

typedef struct Router {
    Route routes[10];
    int route_count;
} Router;

typedef struct ClientInfo {
    int client_socket;
    Router *router;
} ClientInfo;

void router_init(Router *router);
void router_add_route(Router *router, const char *method, const char *path, http_handler handler);
void http_router_write(int socket, int status_code, const char *custom_headers, const char *body);
#endif