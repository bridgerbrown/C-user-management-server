#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../data_structures/dictionary/dictionary.c"
#include "http_request.h"

#include <sys/socket.h>
#include <netinet/in.h>

struct HTTPServer {
    int domain;
    int service;
    int protocol;
    u_int32_t interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
    struct Dictionary routes;

    void (*register_routes)(
        struct HTTPServer *server, 
        char *(*route_function)(struct HTTPServer *server, struct HTTPRequest *request),
        char *uri, 
        int methods_len
    );
    void (*start_server)(struct HTTPServer *server);
    void *(handle_client_request)(void *arg);
};

enum HTTPMethods {
    CONNECT,
    GET,
    POST,
    DELETE
};

enum StatusCode {
    SUCCESS = 0,
    SOCKET_READ_FAILED = 1,
    CLIENT_CLOSED_CONNECTION = 2,
    ROUTE_NOT_FOUND = 3,
    MEMORY_ALLOCATION_FAILED = 4
};

struct HTTPServer http_server_constructor(void);
void register_routes(struct HTTPServer *server, 
                    char *(*route_function)(struct HTTPServer *server, struct HTTPRequest *request),
                    char *uri, 
                    int methods_len);
void start_server(struct HTTPServer *server);
#endif
