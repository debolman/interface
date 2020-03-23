#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <netdb.h>




void *UDP_listener(void *vargp)
{
    while(true) {
        socklen_t len;
        int UDP_recved_len = recvfrom(sockfd, udp_buffer, sizeof(udp_buffer), 0, (struct sockaddr*)&cliaddr,&len);
        if(UDP_recved_len>0) {
            if(udp_raw) {
              for(int n =0 ; n<UDP_recved_len;n++) printf("%02X ", udp_buffer[n]);
                printf("\n");
            }
            if(udp_buffer[0] == 0x50) {
                memcpy(&tlm, udp_buffer,UDP_recved_len);
                //printf("%d \n", tlm.rssi);
                char a[30];
                sprintf(a,"%d",tlm.rssi);
                gtk_label_set_text(GTK_LABEL(rssi_v)  ,a);
                sprintf(a,"%d",tlm.snr);
                gtk_label_set_text(GTK_LABEL(snr_v)  ,a);
                sprintf(a,"%d",tlm.milis);
                gtk_label_set_text(GTK_LABEL(millis_v)  ,a);
                sprintf(a,"%d",tlm.freq_er_hz);
                gtk_label_set_text(GTK_LABEL(freq_err_v)  ,a);
            }
        }
        if(serial_activate) {   int wrote_bytes =  write(fd,&udp_buffer,UDP_recved_len);
        }
    }
}


void socket_initialize() {
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(udp_serv_port);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void UDP_send(unsigned char *hello, int leng) {
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(7072);
    cliaddr.sin_addr.s_addr = inet_addr("192.168.3.56");
    sendto(sockfd, (const char *)hello, leng, 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

