#include "cdotenv_extended.h"

int
replace_env(const char* key, const char* val, const char* file_name) {

	if (!file_name) {
		file_name = ".env";
	}

	FILE* env_file = fopen(file_name, "r");   // read the original

	if (!env_file) {
		perror("fopen");
		return FILE_NOT_FOUND;
	}

	char tmp_name[256];
	snprintf(tmp_name, sizeof tmp_name, "%s.tmp", file_name);

	FILE* tmp_file = fopen(tmp_name, "w");    // write the result here

	if (!tmp_file) {
		perror("fopen");
		fclose(env_file);
		return FILE_NOT_FOUND;
	}

	size_t key_size = strlen(key);
	size_t i = 0;             // key chars matched so far on this line
	bool new_line = true;     // still at the start of a line (matching phase)
	bool skipping = false;    // dropping the rest of the old line
	bool found = false;
	int c;                    // int, not char, or EOF comparison breaks

	while ( ( c = getc(env_file) ) != EOF ) {

		if (skipping) {
			if (c == '\n') {
				skipping = false;
				new_line = true;
			}
			continue;
		}

		if (new_line) {
			if (i < key_size && c == key[i]) {
				i++;
				continue;
			}

			if (i == key_size && c == '=') {
				// full key followed by '=': write the replacement
				fprintf(tmp_file, "%s=%s\n", key, val);
				found = true;
				skipping = true;
				new_line = false;
				i = 0;
				continue;
			}

			// match failed: write the chars we held back
			fwrite(key, 1, i, tmp_file);
			i = 0;
			new_line = false;
		}

		fputc(c, tmp_file);

		if (c == '\n') new_line = true;
	}

	// file ended mid-match
	if (i > 0) fwrite(key, 1, i, tmp_file);

	fclose(env_file);

	if (fclose(tmp_file) != 0) {
		perror("fclose");
		remove(tmp_name);
		return CLOSE_FAIL;
	}

	if (!found) {
		fprintf(stderr, "The key was unable to be found\n");
		remove(tmp_name);
		return SUCESS;
	}

	if (rename(tmp_name, file_name) != 0) {
		perror("rename");
		remove(tmp_name);
		return WRITE_FAIL;
	}

	return SUCESS;
}

int
write_env(const char* key, const char* val, const char* file_name, const char* mode) {

	if (!file_name) {
		file_name = ".env";
	}

	if (!mode) {
		mode = "a";
	}

	FILE *env_file = fopen(file_name, mode);
	if (!env_file) {
	    perror("fopen");
	    return FILE_NOT_FOUND;
	}

	if (fprintf(env_file, "%s=%s\n", key, val) < 0) {
	    perror("fprintf");
	    fclose(env_file);
	    return WRITE_FAIL;
	}

	if (fclose(env_file) != 0) {
	    perror("fclose");
	    return CLOSE_FAIL;
	}
	
	return SUCESS;
}

int
mem_readenv(const char *path) {

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

// readenv on a buffer
int
buf_readenv(const char *path) {

	FILE* env_file = fopen(path, "r");
	
	if (env_file == NULL) {
		perror(path);
		return FILE_NOT_FOUND;
	}

	char c;

	char key[KEY_BUF_SIZE], val[VALUE_BUF_SIZE];

	int ki = 0, vi = 0;

	// 0 is key 1 is val
	bool active = 0;
	while ( ( c = getc(env_file) ) != EOF) {
		if ( c == ' ') {
			continue;
		}

		else if ( c == '\n') {
			if (ki > 0 && vi > 0) {
				key[ki] = '\0';
				val[vi] = '\0';

				c = setenv(key, val, 1);

				if (c == -1) {
					perror("Unable to set env variables");
					return SET_ENV_FAIL;
				}

			}
			ki = 0;
			vi = 0;

			
			active = 0;

			continue;
		}
		else if ( c == '=' && !active ) {
			active = 1;
		}
		else if (!active) {
			if (ki < KEY_BUF_SIZE - 1 ) key[ki++] = c;
			else {
				perror("Possible key overflow! Exiting");
				return KEY_OVERFLOW;
			}
		    }
		else {
			if (vi < VALUE_BUF_SIZE - 1) val[vi++] = c;
			else {
				perror("Possible value overflow! Exiting");
				return VALUE_OVERFLOW;
			}
		}
	}

	if (fclose(env_file) != 0) {
		perror("fclose");
		return CLOSE_FAIL;
	}

	return SUCESS;

}
