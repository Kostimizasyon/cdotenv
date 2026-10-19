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

	FILE* env_file = fopen(file_name, "wx");

	if (!env_file) {
		perror("fopen");
		return FILE_NOT_FOUND;
	}

	char c;

	char* temp = key;
	size_t key_size = strlen(key);

	bool searching = false;
	int i = 0;
	int counter = 0;
	bool new_line = true;
	
	while ( ( c = getc(env_file) ) !=  EOF ) {

		if ( c == temp[0] && new_line ) {
			searching = true;
			i++;
		}

		if (searching) {
			if ( c ==  temp[i] ) {
				i++;
				if (i == key_size) {
					i = 0;
					break;
				}

			}
			else { 
				searching = false;
				new_line = false;
			}

		}

		if (c == '\n') new_line = true;

		counter++;
	}

	// we found it 
	if (searching) {

		fseek(env_file, counter - 1, SEEK_CUR);

		size_t res = fwrite(key, key_size, 1, env_file);

		if (res < 1) {
			perror("fwrite");
			return WRITE_FAIL;
		}

		res = fwrite("=", 1, 1, env_file);

		if (res < 1) {
			perror("fwrite");
			return WRITE_FAIL;
		}

		res = fwrite(val, sizeof(val), 1, env_file);
			
		if (res < 1) {
			perror("fwrite");
			return WRITE_FAIL;
		}

	}

	else {
		perror("The key was unable to be found");
	}


	fclose(env_file);

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

	FILE* env_file = fopen(file_name, mode);

	if (!env_file) {
		perror("fopen");
		return FILE_NOT_FOUND;
	}

	size_t res = fwrite(key, sizeof(key), 1, env_file);

	if (res < 1) {
		perror("fwrite");
		return WRITE_FAIL;
	}

	res = fwrite("=", 1, 1, env_file);

	if (res < 1) {
		perror("fwrite");
		return WRITE_FAIL;
	}

	res = fwrite(val, sizeof(val), 1, env_file);
		
	if (res < 1) {
		perror("fwrite");
		return WRITE_FAIL;
	}

	
	return SUCESS;
}
