#include "http_request.h"
#include "../data_structures/queue.h"

#include <string.h>
#include <stdlib.h>

struct HTTPRequest http_request_constructor(char *request_str) {
    struct HTTPRequest request;
    char *request_str_copy = malloc(strlen(request_str) + 1);
    strcpy(request_str_copy, request_str);

    // convert from string literal and replace blank line with |
    char requested[strlen(request_str) + 1];
    for (int i = 0; i < strlen(requested) - 2; i++) {
        if (requested[i] == '\n' && requested[i + 1] == '\n') { // escape character represents single character
            requested[i + 1] = '|'; // using unique character to represent space between header and content body
        }
    }

    // use strtok to chop up request string to turn into pieces based on delimiter,
    // a particular character that we can use to reference sections of a string.
    char *request_line = strtok(request_str_copy, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    // parse each section
    extract_request_line_fields(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);

    free(request_str_copy);

    return request;
};

void http_request_destructor(struct HTTPRequest *request) {
	dictionary_destructor(&request->request_line);
	dictionary_destructor(&request->header_fields);
	dictionary_destructor(&request->body);
}

void extract_request_line_fields(struct HTTPRequest *request, char *request_line) {
	// copy string literal
	char fields[strlen(request_line) + 1];
	strcpy(fields, request_line);

	// separate string on spaces for each section
	char *method = strtok(fields, " ");
	char *uri = strtok(NULL, " ");
	char *http_version = strtok(NULL, "\0");

	// insert results into dictionary
	struct Dictionary request_line_dict = dictionary_constructor(compare_key_strings);
	request_line_dict.insert(&request_line_dict, "method", strlen("method"), method, strlen(method) + 1);
	request_line_dict.insert(&request_line_dict, "uri", strlen("uri"), method, strlen(uri) + 1);
	request_line_dict.insert(&request_line_dict, "http_version", strlen("http_version"), method, strlen(http_version) + 1);

	// save dictionary to request object
	request->request_line = request_line_dict;

	if (request->request_line.search(&request->request_line, "GET", sizeof("GET") + 1)) {
		extract_body(request, (char *)request->request_line.search(&request->request_line, "uri", sizeof("uri") + 1));
	}
}

void extract_header_fields(struct HTTPRequest *request, char *header_fields) {
	char fields[strlen(header_fields) + 1];
	strcpy(fields, header_fields);

	struct Queue headers = queue_constructor();
	char *field = strtok(fields, "\n");
	while (field) {
		headers.push(&headers, field, strlen(field) + 1);
		field = strtok(NULL, "\n");
	}

	request->header_fields = dictionary_constructor(compare_key_strings);
	char *header = (char *)headers.peek(&headers);
	while (header) {
		char *key = strtok(header, ":");
		char *value = strtok(NULL, "\0");
		if (value) {
			if (value[0] == ' ') value++;
			request->header_fields.insert(&request->header_fields, key, strlen(key) + 1, value, strlen(value) + 1);
		}
		headers.pop(&headers);
		header = (char *)headers.peek(&headers);
	}

	queue_destructor(&headers);
}

// parses body according to content type in header fields
void extract_body(struct HTTPRequest *request, char *body) {
    if (body == NULL) return;

	char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
	if (content_type) {
		struct Dictionary body_fields = dictionary_constructor(compare_key_strings);
		if (strcmp(content_type, "application/x-www-form-urlencoded") == 0) {
			// collect each kv pair as a set and store in queue
			struct Queue fields = queue_constructor();
			char *field = strtok(body, "&");
			while (field) {
				fields.push(&fields, field, strlen(field) + 1);
                field = strtok(NULL, "&");
			}
			field = fields.peek(&fields);
			while (field) {
				char *key = strtok(field, "=");
				char *value = strtok(NULL, "\0");
				if (value[0] == ' ') value++;
				body_fields.insert(&body_fields, key, strlen(key) + 1, value, strlen(value) + 1);
				fields.pop(&fields);
				field = fields.peek(&fields);
			}
			queue_destructor(&fields);
		} else {
			body_fields.insert(&body_fields, "data", sizeof("data"), body, strlen(body) + 1);
		}
		request->body = body_fields;
	}
}
