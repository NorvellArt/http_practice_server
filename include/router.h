#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#define INITIAL_ROUTE_CAPACITY 10

typedef void (*http_handler)(int client_socket);

typedef struct Route {
    char *method;
    char *path;
    http_handler handler;
} Route;

typedef struct Router {
    int route_count;
    int route_capacity;
    Route *routes;
} Router;

Router* http_router_new(void);
void http_router_stop(Router *router);

#endif