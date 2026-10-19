#ifdef __cplusplus
extern "C" {
#endif
// replace the given key with the new  value inside of the env file
int replace_env(const char* key, const char* value, const char* file_name);

// write into a env file
int write_env(const char* key, const char* value, const char* file_name, const char* mode);
#ifdef __cplusplus
}
#endif
