// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    SSL_CTX *ctx;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD *method;
    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if(!ctx){
        perror("Unable to create SSL context");
        exit(EXIT_FAILURE);
    }

    SSL_CTX_set_ecdh_auto(ctx, 1);
    if(SSL_CTX_use_certificate_file(ctx, "server_cert.crt", SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if(SSL_CTX_use_PrivateKey_file(ctx, "server_key.key", SSL_FILETYPE_PEM) <=0 ){
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }


    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    SSL *ssl;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_socket);

    if(SSL_accept(ssl) <= 0){
        ERR_print_errors_fp(stderr);
    }else{
        //char buffer[1024];
        int bytes;

        do {
            bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes > 0) {
                buffer[bytes] = '\0';
                printf("Received: %s", buffer);
                const char* response = "Hello, Client! I received your message.";
                SSL_write(ssl, response, strlen(response));
            }
        } while (bytes > 0);

        SSL_shutdown(ssl);

    }
    
    close(client_socket);
    close(server_socket);
    SSL_free(ssl);
    return 0;
}

