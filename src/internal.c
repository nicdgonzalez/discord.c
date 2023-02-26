#include "internal.h"

#include <winsock2.h>
#include <stdint.h>
#include <stdbool.h>

#include <openssl/ssl.h>

#include "logging.h"
#include "utils.h"

enum HostNameType {
    kHostNameInvalid,
    kHostNameDomain,
    kHostNameIPv4
};

struct logger internal_log = { .name = "Internal" };

static enum HostNameType get_host_name_type(const char *host) {
    host += strlen(host) - 1;
    while (*(host - 1) != '.') {
        host -= 1;
    }

    return (
        kHostNameDomain * (IS_ALPHA(*host))
        + kHostNameIPv4 * (IS_NUMERIC(*host))
    );
}

static SocketCode set_session_host(
    struct socket_context *ctx,
    const char *host
) {
    uint32_t *addr_ptr = (uint32_t *) &ctx->service.sin_addr.S_un.S_addr;
    HOSTENT *hostent = NULL;

    switch (get_host_name_type(host)) {
      case kHostNameDomain:
        log_debug(&internal_log, "Resolved host name as Domain.");
        hostent = gethostbyname(host);

        if (!hostent) {
            log_error(&internal_log, "Failed to get host by name.");
            return kSocketHostName;
        }

        *addr_ptr = inet_addr(inet_ntoa(*(IN_ADDR *) hostent->h_addr_list[0]));
        break;

      case kHostNameIPv4:
        log_info(&internal_log, "Resolved host name as IPv4.");
        *addr_ptr = inet_addr(host);
        break;

      default:
        log_error(&internal_log, "Failed to resolve host name type.");
        return kSocketHostName;
    }

    return kSocketOK;
}

static SocketCode initiate_ssl_protocol(struct socket_context *ctx) {
    OPENSSL_init_ssl(0, NULL);
    const SSL_METHOD *method = TLS_client_method();
    ctx->ssl_ctx = SSL_CTX_new(method);

    if (!ctx->ssl_ctx) {
        log_error(&internal_log, "Failed to configure SSL context.");
        return kSocketSSLContext;
    }
    log_info(&internal_log, "Successfully configured SSL context.");

    ctx->ssl = SSL_new(ctx->ssl_ctx);

    if (ctx->ssl == NULL) {
        log_error(&internal_log, "Failed to initiate SSL protocol.");
        return kSocketSSL;
    }

    SSL_set_fd(ctx->ssl, ctx->socket_fd);

    if (SSL_connect(ctx->ssl) != 1) {
      log_error(&internal_log, "Failed to complete the SSL handshake.");
      return kSocketSSL;
    }
    log_info(&internal_log, "Successfully completed the SSL handshake.");

    return kSocketOK;
}

SocketCode connect_to_server(
    struct socket_context *ctx,
    const char *host,
    const char *port,
    bool use_ssl
) {
    ctx->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ctx->socket_fd == INVALID_SOCKET) {
        log_error(&internal_log, "Failed to create server socket.");
        return kSocketFileDescriptor;
    }
    log_info(&internal_log, "Successfully created server socket.");

    int32_t result = 0;
    if ((result = set_session_host(ctx, host))) {
        log_error(&internal_log, "Failed to set host name.");
        return result;
    }
    log_info(&internal_log, "Successfully set host name.");

    ctx->service.sin_family = AF_INET;
    ctx->service.sin_port = htons(strtol(port, NULL, 0));

    result = connect(
            ctx->socket_fd,
            (SOCKADDR *) &ctx->service,
            sizeof(ctx->service)
    );

    if (result == SOCKET_ERROR) {
        log_error(&internal_log, "Failed to connect to '%s:%s'.", host, port);
        return kSocketConnect;
    }
    log_info(&internal_log, "Connected to server '%s:%s'.", host, port);

    if (use_ssl) {
        log_info(&internal_log, "Initiating SSL protocol...");
        return initiate_ssl_protocol(ctx);
    }

    return kSocketOK;
}

const char * socket_strerror(SocketCode code) {
    switch (code) {
      case kSocketOK:
        return "No error.";
      case kSocketConnect:
        return "Failed to connect to server.";
      case kSocketHostName:
        return "Failed to resolve host name type.";
      case kSocketFileDescriptor:
        return "Failed to create server socket.";
      case kSocketSSLContext:
        return "Failed to configure SSL context.";
      case kSocketSSL:
        return "Failed to initiate SSL protocol.";
    }
    return "Value provided is not a valid Socket error code.";
}
