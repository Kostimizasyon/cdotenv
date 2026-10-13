# dotenv

A simple .env parser for C for setting env variables.

just put

```c

#include "dotenv.c"

read_env("PATH_TO_ENV")

// PATH_TO_ENV
{ENV_NAME}anything here={ENV_VAL}
```

Then use `getenv({ENV_NAME})`
