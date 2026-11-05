#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cdotenv.h"


int
read_env(const char *path) {

	FILE* env_file = fopen(path, "r");
	
	if (env_file == NULL) {
		perror(path);
		return FILE_NOT_FOUND;
	}


	char* line = NULL;
	size_t cap;
	
	ssize_t len;
	
	while ( ( len = getline(&line, &cap, env_file) ) != -1) {

		int res;

		char* eq = memchr(line, '=', len);

		if (!eq) {
			free(line);
			perror("Failed to parse keys, check .env");
			return WRITE_FAIL;
		}

		*eq = '\0';
		char *key = line;
		char *val = eq +1;  

		res = setenv(key, val, 0);

		if (res == -1) {
			perror("Unable to set env variables");
			return SET_ENV_FAIL;
		}
	
	}

	free(line);

	if (fclose(env_file) != 0) {
		perror("fclose");
		return CLOSE_FAIL;
	}

	return SUCESS;

}
