#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define PORT 12345
#define MAC "30:03:C8:AC:9F:50"

int main(){
    /*
    int dev_id = hci_get_route(NULL);
    if (dev_id<0){
        perror("No Bluetooth Adapter Available");
        exit(1);
    }
    */

    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    char buff[1024] = {0};
    int server = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    loc_addr.rc_family = AF_BLUETOOTH;
    //loc_addr.rc_bdaddr = ;
    char mac[18]=MAC;
    str2ba(mac, &loc_addr.rc_bdaddr);
    loc_addr.rc_channel = (uint8_t)PORT;

    bind(server, (struct sockaddr*)&loc_addr, sizeof(loc_addr));

    ba2str(&loc_addr.rc_bdaddr, buff);
    fprintf(stdout, "local %s\n", buff);

    listen(server,1);
    printf("Waiting for connection...\n");
    //while(1){
    socklen_t opt = sizeof(rem_addr);
    int client = accept(server, (struct sockaddr*)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr, buff);
    fprintf(stdout, "Connected to %s\n", buff);

    memset(buff, 0, sizeof(buff));
    //char recv[1024] = {0};
    int bytes_read = read(client, buff, sizeof(buff));
    if(bytes_read>0){
        printf("Received [%s]\n", buff);
    }else{
        printf("No data");
    }
    close(client);
    close(server);
    return 0;
    //}
}