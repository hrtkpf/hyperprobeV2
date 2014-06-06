#ifdef DEBUG 
#define DPRINTF(fmt, ...) \
    do { printf(fmt, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
    do { } while (0)
#endif
