#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cdotenv.h"
#include "cdotenv_utils.h"

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
