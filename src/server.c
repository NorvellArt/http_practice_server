#include "server.h"

static void *client_thread(void *arg);

void server_init(Server *server, int port, Router *router)
{
    server->port = port;
    server->router = router;

    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)};

    bind(server->server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server->server_fd, 10);
}

void server_run(Server *server)
{
    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server->server_fd, (struct sockaddr *)&client_address, &client_len);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        ClientInfo *info = malloc(sizeof(ClientInfo));
        if (!info)
        {
            perror("Memory allocation failed for client_info");
            close(client_socket);
            continue;
        }
        info->client_socket = client_socket;
        info->router = server->router;

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, info) != 0)
        {
            perror("Thread creation failed");
            close(client_socket);
            free(info);
            continue;
        }
        pthread_detach(tid);
    }
}

static char* allocate_buffer(size_t size) {
    char *buffer = malloc(size);
    if (!buffer)
        perror("Failed to allocate buffer");
    return buffer;
}

static char* expand_buffer(char *buffer, size_t *current_size) {
    *current_size *= 2;
    char *new_buffer = realloc(buffer, *current_size);
    if (!new_buffer) {
        perror("Failed to expand buffer");
        free(buffer);
    }
    return new_buffer;
}

static char* read_request_from_socket(int socket)
{
    size_t capacity = READ_CHUNK_SIZE;
    size_t total_read = 0;

    char *buffer = allocate_buffer(capacity);
    if (!buffer)
        return NULL;

    while (1) {
        if (total_read + READ_CHUNK_SIZE > capacity) {
            buffer = expand_buffer(buffer, &capacity);
            if (!buffer)
                return NULL;
        }
        ssize_t bytes_read = read(socket, buffer + total_read, READ_CHUNK_SIZE);
        if (bytes_read < 0) {
            perror("read failed");
            free(buffer);
            return NULL;
        }
        if (bytes_read == 0)
            break;
        total_read += bytes_read;
        buffer[total_read] = '\0';
        if (strstr(buffer, "\r\n\r\n") != NULL)
            break;
    }
    return buffer;
}

void write_string(int socket, const char *str) {
    write(socket, str, strlen(str));
}

static int parse_request_line(const char *request, char *method, char *path) {
    return sscanf(request, "%15s %255s", method, path) == 2;
}

static void handle_client_request(ClientInfo *info)
{
    char method[16] = {0};
    char path[256] = {0};

    char *request = read_request_from_socket(info->client_socket);
    if (!request) {
        close(info->client_socket);
        return;
    }

    if (!parse_request_line(request, method, path)) {
        write_string(info->client_socket, "HTTP/1.1 400 Bad Request\r\n\r\n");
        free(request);
        close(info->client_socket);
        return;
    }
    free(request);

    int route_found = 0;

    for (int i = 0; i < info->router->route_count; i++) {
        if (strcmp(info->router->routes[i].method, method) == 0 &&
            strcmp(info->router->routes[i].path, path) == 0) {
            info->router->routes[i].handler(info->client_socket);
            route_found = 1;
            break;
        }
    }

    if (!route_found) {
        http_router_write(info->client_socket, 404, "Content-Type: text/html\r\n", "<html><body>Not Found</body></html>");
        close(info->client_socket);
    }
}

static void *client_thread(void *arg)
{
    ClientInfo *info = (ClientInfo *)arg;
    handle_client_request(info);
    free(info);

    return NULL;
}
