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

    // 创建客户端套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接到服务器
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
        // 进行加密通信
        //char buffer[] = "Hello, Server!";
        while(1){
        printf("Enter message to send to server (type 'exit' to end): ");
        fgets(buffer, sizeof(buffer), stdin);

        SSL_write(ssl, buffer, strlen(buffer));

        // 可选：接收服务器的响应
        char response[1024];
        int bytes = SSL_read(ssl, response, sizeof(response));
        if (bytes > 0) {
            response[bytes] = '\0';
            printf("Received from server: %s", response);
        }
        }
    }
    /* 发送数据给服务器并接收响应
    while (1) {
        printf("Enter message to send to server (type 'exit' to end): ");
        fgets(buffer, sizeof(buffer), stdin);

        // 发送数据给服务器
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        // 接收服务器的响应
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            perror("Error receiving data");
            break;
        }

        printf("Received from server: %s", buffer);
    }
    */
    // 关闭套接字
    close(client_socket);

    return 0;
}

