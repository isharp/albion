#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sed.h>

int log_ind = 0;
buff_line_t log_buff[100];

void log_msg(const char *msg, const char *nick) 
{
	char buff[1024];
	bzero(buff, 1024);
	int i = 0;

	while (nick[i] != '!') {
		i++;
	}

	strncat(buff, nick, i);
	strcat(buff, ": ");
	strcat(buff, msg);

	memcpy(log_buff[log_ind].line, buff, 1024);

	if (log_ind == 99){
		log_ind = 0;
	} else{
		log_ind++;
	}
}

const char * find_msg(const char *msg)
{
	static char search[1024] = "";
	static char replace[1024] = "";
	static char output[1024] = "";

	bzero(search, 1024);
	bzero(replace, 1024);
	bzero(output, 1024);
	
	int ind = 4;
	int mark = 0;
	int found = 0;
	char * edit;

	while (msg[ind] != '/'){
		ind++;
	}

	memcpy(&search, &msg[3], ind++ - 3);
	mark = ind;

	while (msg[ind] != '\0'){
		ind++;
	}

	memcpy(&replace, &msg[mark], ind - mark);
	ind = log_ind - 2;

	while (!found){
		if ((edit = strstr(log_buff[ind].line, search)) != NULL){
			if (strncmp(log_buff[ind].line, ".s/", 3)) {
				found = 1;
			}
		} else {
			if (!ind){
				ind = 99;
			} else {
				ind--;
			}
		}
	}

	strncat(output, log_buff[ind].line, strlen(log_buff[ind].line)
                                         - strlen(edit));
	strcat(output, replace);
	strcat(output, edit + strlen(search));

	return output;

}
