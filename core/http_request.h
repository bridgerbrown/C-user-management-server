#ifndef HTTPRequest_h
#define HTTPRequest_h

#include "../data_structures/dictionary/dictionary.h"

struct HTTPRequest {
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary body;
};

struct HTTPRequest http_request_constructor(char *request_str);
void http_request_destructor(struct HTTPRequest *request);

void extract_request_line_fields(struct HTTPRequest *request, char *request_line);
void extract_header_fields(struct HTTPRequest *request, char *header_fields);
void extract_body(struct HTTPRequest *request, char *body);
#endif
