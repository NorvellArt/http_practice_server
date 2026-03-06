#include <stdlib.h>
#include <stdio.h>

#include "router.h"

Router *http_router_new(void)
{
    Router *router = malloc(sizeof(Router));
    if (!router) {
        perror("Error: Failed to allocate router");
        exit(EXIT_FAILURE);
    }

    router->route_count = 0;
    router->route_capacity = INITIAL_ROUTE_CAPACITY;
    router->routes = malloc(sizeof(Route) * router->route_capacity);
    if (!router->routes) {
        perror("Error: Failed to allocate routes");
        free(router);
        exit(EXIT_FAILURE);
    }

    return router;
}

void http_router_stop(Router *router) {
    if (router) {
        for (int i = 0; i < router->route_count; i++) {
            free(router->routes[i].method);
            free(router->routes[i].path);
        }
        free(router->routes);
        free(router);
    }
}