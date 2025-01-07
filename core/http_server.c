#include "http_server.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>  

struct HTTPServer http_server_constructor()
{
    struct HTTPServer server;
    server.domain = AF_INET; // IPv4 address family for communication
    server.service = SOCK_STREAM; // stream-oriented sockets for type of socket communication, to use TCP
    server.protocol = 0; // 0 = automatically choose appropriate protocol based on domain and service, here is TCP
    server.interface = INADDR_ANY; // bind ANY available network interfaces (any IP's) to the server,
    server.port = 80; // standard port for HTTP traffic
    server.backlog = 255; // max length of queue for incoming connections

    server.routes = dictionary_constructor(compare_key_strings);
    server.register_routes = register_routes;
    server.launch = launch;

    return server;
};

void register_routes(struct HTTPServer *server, 
                    char *(*route_function)(struct HTTPServer *server, struct HTTPRequest *request),
                    char *uri, 
                    int methods_len)
{
    struct Route route;
    va_list methods;
    va_start(methods, methods_len);
    for (int i = 0; i < methods_len; i++)
    {
        route.methods[i] = va_arg(methods, int);
    }

    char buffer[strlen(uri) + 1]; // + 1 for the \0
    route.uri = buffer;
    strcpy(route.uri, uri);
    route.route_function = route_function;
    server->routes.insert(&server->routes, uri, strlen(uri) + 1, &route, sizeof(route));
}

void start_server(struct HTTPServer *server)
{
    // start thread pool and open sockets
    struct ThreadPool thread_pool = thread_pool_constructor(20);
    struct sockaddr *socket_address = (struct sockaddr *)&server->address;
    socklen_t address_len = (socklen_t)sizeof(server->address);

    while (1)
    {
        struct Client *client = malloc(sizeof(struct Client));
        // accept incoming connections
        client->client = accept(server->server.socket, socket_address, &address_len);
        client->server = server;
        // pass client to thread pool
        struct ThreadJob job = thread_job_constructor(handler, client);
        thread_pool.add_work(&thread_pool, job);
    }
}

enum StatusCode handle_client_request(void *arg)
{
    // read client request
    struct Client *client = (struct Client *)arg;
    char request_str[30000];
    ssize_t bytes_read = read(client->client, request_str, sizeof(request_str));
    if (bytes_read == -1) {
        perror("Socket read failed");
        close(client->client);
        free(client);
        return SOCKET_READ_FAILED;
    }
    if (bytes_read == 0) {
        perror("Client closed the connection\n");
        close(client->client);
        free(client);
        return CLIENT_CLOSED_CONNECTION;
    }

    struct HTTPRequest request = http_request_constructor(request_str);

    // proccess and respond to client
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    struct Route *route = client->server->routes.search(&client->server->routes, uri, strlen(uri) + 1);
    char *response = route->route_function(client->server, &request);
    write(client->client, response, strlen(uri) + 1);

    // cleanup
    close(client->client);
    free(client);
    http_request_destructor(&request);

    return SUCCESS;
}
