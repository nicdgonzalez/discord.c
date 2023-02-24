#ifndef NDG_INTERNAL_H_
#define NDG_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif  // !__cplusplus

#include <winsock2.h>
#include <stdbool.h>

#include <openssl/ssl.h>

typedef enum socket_code {
    kSocketOK,
    kSocketHostName,
    kSocketFileDescriptor,
    kSocketConnect,
    kSocketSSLContext,
    kSocketSSL
} SocketCode;

struct socket_context {
    WSADATA wsa_data;
    SOCKADDR_IN service;
    SOCKET socket_fd;
    SSL_CTX *ssl_ctx;
    SSL *ssl;
};

const char * socket_strerror(SocketCode);

SocketCode connect_to_server(
    struct socket_context *,
    const char *,
    const char *,
    bool
);

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !NDG_INTERNAL_H_
