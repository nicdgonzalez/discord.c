// This section is still a work in progress. I am updating the repo because
// I have successfully requested and received a valid response from Discord.
// TODO: Write a response parser and convert Hash Table to str (and vice-versa)

#include "http.h"

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "format.h"
#include "hashtable.h"
#include "internal.h"
#include "logging.h"

#define DISCORD_TOKEN_LEN 70

struct discord_client_context {
    char token[DISCORD_TOKEN_LEN + 1];
} discord_client;

static struct socket_context ctx;
static struct logger http_log = { .name = "HTTP" };
const char DISCORD_HOST[] = "discord.com";

DiscordCode discord_global_init() {
    if (WSAStartup(0x202, &ctx.wsa_data)) {
        log_error(&http_log, "Failed to start Winsocks API for this process.");
    }
    SocketCode result = connect_to_server(&ctx, "discord.com", "443", true);
    log_debug(&http_log, "Socket: %s", socket_strerror(result));
    return kDiscordOK;
}

DiscordCode http_request_get(struct http_request *request, const char *path) {
    struct format content;
    format_init(
        &content,
        "GET /api/v10{} HTTP/1.1\r\n"
        "Host: {}\r\n"
        "Connection: close\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: {}\r\n"
        "Authorization: Bot {}\r\n"
        "\r\n"
    );
    format_add_argument(&content, path);
    format_add_argument(&content, (const char *) &DISCORD_HOST);
    format_add_argument(&content, "0");
    format_add_argument(&content, discord_client.token);

    strcpy((char *) &request->raw_str, format_to_str(&content));
    format_free(&content);
    return kDiscordOK;
}

DiscordCode discord_run(const char *token) {
    strcpy((char *) &discord_client.token, token);

    struct http_request request;
    http_request_get(&request, "/gateway");

    char response[2048] = { '\0' };
    char buffer[2048] = { '\0' };

    int bytes = SSL_write(ctx.ssl, &request.raw_str, sizeof(request.raw_str));
    bytes = SSL_read(ctx.ssl, &buffer, sizeof(buffer));
    while (bytes > 0) {
        bytes = SSL_read(ctx.ssl, &buffer, sizeof(buffer));
        strcat((char *) &response, (const char *) &buffer);
        memset(&buffer, '\0', sizeof(buffer));
        break;
    }

    log_info(&http_log, "Response: %s\n", response);
    SSL_shutdown(ctx.ssl);
    return kDiscordOK;
}
