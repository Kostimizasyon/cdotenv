# dotenv

A simple .env parser for C for setting env variables.

just put

```txt
{ENV_NAME}={ENV_VAL}
```

Then in ur program:

```c
#include "cdotenv.h"

read_env("PATH_TO_ENV")

// then just
char* env_name = getenv("{ENV_NAME}")
```
