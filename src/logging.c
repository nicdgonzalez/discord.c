#include "logging.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>
#include <stdarg.h>

#include "format.h"

#define LOGGING_FN_PARAMS struct logger *logger, const char *format, ...
#define LOGGING_FN_BASE(level) \
    va_list args; \
    va_start(args, format); \
    struct logging_message message; \
    memcpy(&message.info.args, &args, sizeof(args)); \
    logging_message_init(logger, level, format, &message); \
    logging_write_message(&message); \
    va_end(args)

struct logging_message {
    char *content;
    struct message_info {
        enum logging_level level;
        va_list args;
    } info;
};

static struct logging_manager {
    FILE *fp;
    char library_name[50];
    char date_time_fmt[32];
    enum logging_level level;
    bool once;
} manager_ctx = {
    .fp = NULL,
    .library_name = "unnamed",
    .date_time_fmt = "%Y-%m-%d %H:%M:%S",
    .level = kLogLevelDebug,
    .once = false
};

static void date_time_now(char *dest, uint64_t size, const char *fmt) {
    time_t epoch = time(&epoch);
    struct tm *time_info = localtime((const time_t *) &epoch);
    strftime(dest, size, fmt, time_info);
    return;
}

static LoggingCode open_logging_file() {
    CreateDirectory("logs", NULL);

    struct format file_name;
    format_init(&file_name, "./logs/{}.{}.log");
    format_add_argument(&file_name, manager_ctx.library_name);

    char date_time[16];
    date_time_now((char *) &date_time, sizeof(date_time), "%Y%m%d_%H%M%S");
    format_add_argument(&file_name, date_time);

    manager_ctx.fp = fopen(format_to_str(&file_name), "w+");
    format_free(&file_name);
    return kLoggingOK;
}

static void logging_message_init(
    struct logger *logger,
    enum logging_level level,
    const char *content,
    struct logging_message *message
) {
    struct format fmt;
    // logger_name LEVEL @ YYYY-MM-DD HH:MM:SS]: content
    format_init(&fmt, "{} @ {} [{}]: {}\n");
    format_add_argument(&fmt, logger->name);

    char date_time[] = "YYYY-MM-DD HH:MM:SS";
    date_time_now(
        (char *) &date_time,
        sizeof(date_time),
        (const char *) manager_ctx.date_time_fmt
    );
    format_add_argument(&fmt, (const char *) &date_time);

    char level_to_str[5][6] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    format_add_argument(&fmt, (const char *) &level_to_str[level]);

    format_add_argument(&fmt, content);

    message->info.level = level;
    message->content = (char *) calloc(fmt.length, sizeof(char));
    strcpy(message->content, format_to_str(&fmt));
    format_free(&fmt);
    return;
}

static LoggingCode logging_message_free(struct logging_message *message) {
    free(message->content);
    return kLoggingOK;
}

static LoggingCode logging_write_message(struct logging_message *message) {
    if (message->info.level >= manager_ctx.level) {
        vfprintf(manager_ctx.fp, message->content, message->info.args);
        fflush(manager_ctx.fp);
    }
    logging_message_free(message);
    return kLoggingOK;
}

const char * logging_strerror(LoggingCode code) {
    switch (code) {
      case kLoggingOK:
        return "No error.";
      default:
        break;
    }
    return "Value provided was not a valid Logging error code.";
}

LoggingCode logging_global_init() {
    if (manager_ctx.once) {
        return kLoggingOK;
    }

    open_logging_file();

    manager_ctx.once |= true;
    return kLoggingOK;
}

LoggingCode logconf_set_lib_name(const char *name) {
    strcpy((char *) &manager_ctx.library_name, name);
    return kLoggingOK;
}

LoggingCode logconf_set_datetime_fmt(const char *fmt) {
    strcpy((char *) &manager_ctx.date_time_fmt, fmt);
    return kLoggingOK;
}

LoggingCode logconf_set_level(enum logging_level level) {
    manager_ctx.level = level;
    return kLoggingOK;
}

LoggingCode log_debug(LOGGING_FN_PARAMS) {
    LOGGING_FN_BASE(kLogLevelDebug);
    return kLoggingOK;
}

LoggingCode log_info(LOGGING_FN_PARAMS) {
    LOGGING_FN_BASE(kLogLevelInfo);
    return kLoggingOK;
}

LoggingCode log_warn(LOGGING_FN_PARAMS) {
    LOGGING_FN_BASE(kLogLevelWarn);
    return kLoggingOK;
}

LoggingCode log_error(LOGGING_FN_PARAMS) {
    LOGGING_FN_BASE(kLogLevelError);
    return kLoggingOK;
}

LoggingCode log_fatal(LOGGING_FN_PARAMS) {
    LOGGING_FN_BASE(kLogLevelFatal);
    exit(EXIT_FAILURE);
    return kLoggingOK;
}
