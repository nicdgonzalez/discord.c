#ifndef NDG_HTTP_H_
#define NDG_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif  // !__cplusplus

#include "hashtable.h"

typedef enum discord_code {
    kDiscordOK
} DiscordCode;

enum http_method {
    kHTTPMethodGet,
    kHTTPMethodPost,
    kHTTPMethodPut,
    kHTTPMethodPatch,
    kHTTPMethodDelete
};

struct http_request {
    enum http_method method;
    const char path[256];
    const char host[256];
    char raw_str[4096];
};

const char * discord_strerror(DiscordCode);

DiscordCode discord_global_init();

DiscordCode discord_run(const char *);

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !NDG_HTTP_H_
