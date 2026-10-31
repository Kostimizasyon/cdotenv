#ifndef __cdotenv_extended_h__
#define __cdotenv_extended_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "cdotenv.h"
// replace the given key with the new  value inside of the env file
int replace_env(const char* key, const char* value, const char* file_name);

// write into a env file
int write_env(const char* key, const char* value, const char* file_name, const char* mode);

// read env variables WITHOUT the risk of overflow
int mem_readenv(const char* path);

#ifdef __cplusplus
}
#endif

#endif
