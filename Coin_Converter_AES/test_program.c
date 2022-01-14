//Program to test the running status of Coin Converter

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define PORT     1800
#define MAXLINE 1024

int main() {
    int sockfd;
    unsigned char buffer[MAXLINE], recv_buffer[MAXLINE];	
    unsigned char buffer_echo[MAXLINE]={0,0,2,0,0,4,2,0,0,0,0,0,22,22,0,0,1,0x3E,0x3E};
	struct sockaddr_in  servaddr;

    char ip_address[1][256] = {"73.66.181.49"};
    int port_no = 18000;

    memcpy(buffer,buffer_echo,MAXLINE);	

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_no);
    servaddr.sin_addr.s_addr = inet_addr(ip_address[0]);

    int n, len=0;
    for(int i=0;i<MAXLINE;i++){
        len++;
        if(buffer[i]==62 && buffer[i-1]==62){
            break;	}
    }
    printf("Len = %d\n", len);

    sendto(sockfd, (const char *)buffer, len,
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
    n = recvfrom(sockfd, (char *)recv_buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    printf("n: %d\n", n);

    for(i=0;i<n;i++){	
        printf("%d,", recv_buffer[i]);
    }
    printf("\n");
    close(sockfd);
    return 0;
}