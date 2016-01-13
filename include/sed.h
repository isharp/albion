#ifndef SED_H
#define SED_H

typedef struct{
	char line[1024];
}buff_line_t;

void log_msg(const char *msg, const char *nick);
const char * find_msg(const char *msg);

#endif