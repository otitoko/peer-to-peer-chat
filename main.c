#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define MSG_CHAR_LIMIT 256
#define SENDER "lain"
#define RECEIVER "anon"


int server();
int client();

int send_msg(char *buf, int sockfd);
int receive_msg(char *buf,int acceptfd);

struct sockaddr_in listener_addr;

int main(){

    char input[3];
    int type;

    printf("Hello World!\n");
    printf("Choose handler type:\n");
    printf("1. Client\n");
    printf("2. Server\n");

    fgets(input, sizeof(input),stdin);

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    type = atoi(input);
    
    if(type==1){
        client();
    }
    else{
        server();
    }



    return 0;
}


int server(){
    int serverfd = socket(AF_INET,SOCK_STREAM,0);

    listener_addr.sin_family = AF_INET;
    listener_addr.sin_port = htons(5555);
    listener_addr.sin_addr.s_addr = INADDR_ANY;


    if (serverfd<0){
        printf("Error creating socket.\n");
        return 1;
    }

    int bindfd = bind(serverfd,(struct sockaddr *)&listener_addr, sizeof(listener_addr));
    if(bindfd<0){
        printf("Error binding socket.\n");
        return 1;
    }
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(listener_addr.sin_addr), ip_address, INET_ADDRSTRLEN);

    printf("Waiting for connection on port %d at %s...\n",ntohs(listener_addr.sin_port), ip_address);

    listen(serverfd, 5);

    int acceptfd = accept(serverfd,NULL,NULL);
    if(acceptfd<0){
        printf("Could not connect to client\n");
    }
    else{
        printf("Connected to anon\n");
    }


    while(1){
        char *buf = malloc(MSG_CHAR_LIMIT);

        //memset(buf,0,sizeof(buf));
        printf("anon: ");
        int ret=receive_msg(buf,acceptfd);
        assert(ret==0);
        
    }

    close(acceptfd);
    close(serverfd);

}
    

int client(){
    int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd<0){
        printf("\nError creating a socket.\n");
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(5555),
    };


    char addr_buf[17];
    printf("\nConnect to server: ");
    fgets(addr_buf,17,stdin);


    size_t len = strlen(addr_buf);
    if (len > 0 && addr_buf[len - 1] == '\n') {
        addr_buf[len - 1] = '\0';
    }

    if(strlen(addr_buf)==0){
        strcpy(addr_buf,"127.0.0.1");
    }
    if(inet_pton(AF_INET,addr_buf,&server_addr.sin_addr)<=0){
        printf("\nCould not assign ip address");
    }

    int connection_status = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(connection_status < 0){
        printf("\nCould not connect to server");
        return 1;
    }

    while(1){
        char *buf = malloc(MSG_CHAR_LIMIT);


        printf("lain: ");
        send_msg(buf,sockfd);


    }

    close(sockfd);

}

int receive_msg(char *buf,int acceptfd){

        ssize_t bytes = recv(acceptfd, buf, MSG_CHAR_LIMIT-1,0);


        if(bytes<0){
            printf("\nError recieving message");
            return 1;
        }
        else if(bytes == 0){
            printf("\nClient has disconnected");
            return 1;
        }
        else{
            printf("%s",buf);
            fflush(stdout);
            return 0;
        }
}

int send_msg(char *buf, int sockfd){
    buf=fgets(buf, MSG_CHAR_LIMIT, stdin);

    assert(buf!=NULL);

    ssize_t bytes = send(sockfd, buf, strlen(buf),0);
    fflush(stdout);
    return 0;
}
