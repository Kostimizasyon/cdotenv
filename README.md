# dotenv

A simple .env parser for C for setting env variables.

just put

```txt
// .env or anything else u want idk

{ENV_NAME}={ENV_VAL}
```

Then in ur program:

```c
#include "cdotenv.h"

read_env("PATH_TO_ENV")

// then just
char* env_name = getenv("{ENV_NAME}")
```

## Extended

If you also want to change env files from C, use `cdotenv_extended.h`. It includes `cdotenv.h`, so `read_env` works too and you only need the one import.

The write and replace code lives in the extended header only, so if you just read envs, stick with `cdotenv.h` and skip the extra overhead.

```c
#include "cdotenv_extended.h"

// read like normal
read_env("PATH_TO_ENV");

// add a new KEY=VAL line (appends by default)
write_env("NEW_KEY", "new_val", "PATH_TO_ENV", NULL);

// same thing, explicit mode
write_env("NEW_KEY", "new_val", "PATH_TO_ENV", "a");

// overwrite the whole file with just this line
write_env("NEW_KEY", "new_val", "PATH_TO_ENV", "w");

// change the value of an existing key
replace_env("EXISTING_KEY", "other_val", "PATH_TO_ENV");
```

`mode` is a normal `fopen` mode and defaults to `"a"` (append) when `NULL`. Passing `NULL` as the file path uses `.env`.

- `"w"` truncates the file, so everything else in it is lost.
- `replace_env` only touches the line whose key matches exactly (`KEY` won't match `KEY_2`). Other lines stay as they are.
- If the key isn't found, the file is left untouched.
- It writes to a temp file and renames it over the original, so a crash mid-write won't corrupt your `.env`.
