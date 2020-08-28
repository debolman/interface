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
    green();
    printf(" and listening on port %d\n", udp_serv_port);
    normal();
    while(true) {
        socklen_t len;
        int UDP_recved_len = recvfrom(sockfd, udp_buffer, sizeof(udp_buffer), 0, (struct sockaddr*)&cliaddr,&len);
        if(UDP_recved_len>0) {
            //UDP_send_f(udp_buffer,UDP_recved_len);
            
            if(udp_raw) {
              for(int n =0 ; n<UDP_recved_len;n++) printf("%02X ", udp_buffer[n]);
                printf("\n");
            }
            if(udp_buffer[0] == 0x70) {
                memcpy(&param, udp_buffer,UDP_recved_len);
                
                write_wo_connection(param.id,param.SF, param.coding, param.crc, param.pwr_db, param.pwr_pa,  param.band_i, param.freq_i, param.beacon , param.milis);
                
                
        }
            if(udp_buffer[0] == 0x71) {
                    memcpy(&param_green, udp_buffer,UDP_recved_len);

                    //write_wo_connection(param.id,param.SF, param.coding, param.crc, param.pwr_db, param.pwr_pa,  param.band_i, param.freq_i, param.beacon , param.milis);
                print_green();
                    
            }
        if(serial_activate) {   int wrote_bytes =  write(fd,&udp_buffer,UDP_recved_len);
        }
        }
        
    }
}


void socket_initialize() {
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        red();
        printf("Socket creation failed");
        normal();
        exit(EXIT_FAILURE);
    }
    else {
        green();
        printf("UDP socket activated..");
        normal();
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(udp_serv_port);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        red();
        printf("bind failed\n");
        normal();
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

void UDP_send_f(unsigned char *hello, int leng) {
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(7072);
    cliaddr.sin_addr.s_addr = inet_addr("192.168.2.56");
    sendto(sockfd, (const char *)hello, leng, 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void print_green() {
    float a = param_green.milis/1000;
    printf("\r5V-V %1.2f   5V-I %04.0f   5V_P %04.0f    12V-V %02.2f    12V-I %04.0f    12V-P %04.0f     Millis %06.0f", param_green.V5_v, param_green.V5_i, param_green.V5_p, param_green.V12_v, param_green.V12_i, param_green.V12_p, a);
    fflush(stdout);
}

