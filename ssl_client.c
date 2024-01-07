// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD *method;
    SSL_CTX *ctx;
    method = TLS_client_method();
    ctx = SSL_CTX_new(method);
    if(!ctx){
        perror("Unable to create SSL context");
        exit(EXIT_FAILURE);
    }

    SSL *ssl;
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_socket);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        //char buffer[] = "Hello, Server!";
        while(1){
            printf("Enter message to send to server (type 'exit' to end): ");
            fgets(buffer, sizeof(buffer), stdin);

            SSL_write(ssl, buffer, strlen(buffer));

            char response[1024];
            int bytes = SSL_read(ssl, response, sizeof(response));
            if (bytes > 0) {
                response[bytes] = '\0';
                printf("Received from server: %s", response);
            }
        }
    }
    
    close(client_socket);

    return 0;
}

