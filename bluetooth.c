#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <pthread.h>

#define PORT 12345
#define MAC "30:03:C8:AC:9F:50"

void *handle_client(void *arg);

int main() {
    int status;
    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    char buff[1024] = {0};
    int server = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = 20;

    status = bind(server, (struct sockaddr*)&loc_addr, sizeof(loc_addr));
    if (status < 0) {
        fprintf(stderr, "-- socket binding failed!\n");
        exit(-1);
    }

    ba2str(&loc_addr.rc_bdaddr, buff);
    fprintf(stdout, "local %s\n", buff);

    status = listen(server, 1);
    if (status < 0) {
        fprintf(stderr, "-- socket binding failed!\n");
        exit(-1);
    }
    printf("Waiting for connection...\n");

    socklen_t opt = sizeof(rem_addr);
    int client = accept(server, (struct sockaddr*)&rem_addr, &opt);

    if (client < 0) {
        fprintf(stderr, "-- failed to connect to client\n");
        exit(-1);
    } else {
        printf("-- client connected successfully\n");
    }

    ba2str(&rem_addr.rc_bdaddr, buff);
    fprintf(stdout, "Connected to %s\n", buff);

    // Create a thread for handling client communication
    pthread_t thread;
    if (pthread_create(&thread, NULL, handle_client, (void*)&client) != 0) {
        perror("pthread_create");
        exit(-1);
    }

    while (1) {
        printf("Enter a message to send to the client: \n");
        fgets(buff, sizeof(buff), stdin);
        write(client, buff, strlen(buff));
        printf("Sent: [%s]\n", buff);
    }

    close(server);
    return 0;
}

void *handle_client(void *arg) {
    int client = *((int *)arg);
    char buff[1024] = {0};

    while (1) {
        memset(buff, 0, sizeof(buff));
        int bytes_read = read(client, buff, sizeof(buff));
        if (bytes_read > 0) {
            printf("Received: [%s]\n", buff);
            //const char *response = "Message received!";
            //write(client, response, strlen(response));
        } else {
            printf("No data\n");
        }
    }

    close(client);
    pthread_exit(NULL);
}
