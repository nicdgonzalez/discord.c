#ifndef NDG_LOGGING_H_
#define NDG_LOGGING_H_

#ifdef __cplusplus
extern "C" {
#endif  // !__cplusplus

#include <time.h>
#include <stdarg.h>

typedef enum logging_code {
    kLoggingOK
} LoggingCode;

enum logging_level {
    kLogLevelDebug,
    kLogLevelInfo,
    kLogLevelWarn,
    kLogLevelError,
    kLogLevelFatal,
    kLogLevelNotSet
};

struct logger {
    char *name;
};

const char * logging_strerror(LoggingCode);

LoggingCode logging_global_init();

LoggingCode logconf_set_lib_name(const char *);

LoggingCode logconf_set_datetime_fmt(const char *);

LoggingCode logconf_set_level(enum logging_level);

LoggingCode log_debug(struct logger *, const char *, ...);

LoggingCode log_info(struct logger *, const char *, ...);

LoggingCode log_warn(struct logger *, const char *, ...);

LoggingCode log_error(struct logger *, const char *, ...);

LoggingCode log_fatal(struct logger *, const char *, ...);

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !NDG_LOGGING_H_
