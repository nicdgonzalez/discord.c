#include "http.h"

#include <stdint.h>

#include "internal.h"
#include "logging.h"

static struct socket_context ctx;
struct logger http_log = { .name = "HTTP" };

DiscordCode discord_global_init() {
    if (WSAStartup(0x202, &ctx.wsa_data)) {
        log_error(&http_log, "Failed to start Winsocks API for this process.");
    }
    connect_to_server(&ctx, "discord.com", "443", true);
    return kDiscordOK;
}

DiscordCode http_request(struct http_request *request) {
    return kDiscordOK;
}
