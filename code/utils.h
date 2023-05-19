#ifndef UTILS_H
#define UTILS_H

#define UNUSED(x) ((void)(x))

#define internal static
#define global static
#define local_persist static

#define ARRAY_LEN(arr) (sizeof(arr)/sizeof(arr[0]))

#define ERROR_EXIT(cond, msg, ...)                  \
    do {                                            \
        if ((cond)) {                               \
            fprintf(stderr, (msg), __VA_ARGS__);    \
            exit(1);                                \
        }                                           \
    } while(0)

#endif // UTILS_H
