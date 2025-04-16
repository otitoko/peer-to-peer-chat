#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>

#define MSG_CHAR_LIMIT 256
#define SENDER "lain"
#define RECEIVER "anon"

//main functions
int server();
int client();


struct thread_args{
    int sockfd;
    int acceptfd;
};
//message sending and receiving
void* send_msg(void *args);
void* receive_msg(void *args);

//message jannying and succh
int empty_string_check(char *buf);


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
    pthread_t receive_thread,send_thread;
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

    struct thread_args server_thread_args={
        .acceptfd=acceptfd,
        .sockfd=acceptfd
    };

    pthread_create(&receive_thread,NULL,&receive_msg,&server_thread_args);
    pthread_create(&send_thread,NULL,&send_msg,&server_thread_args);

    pthread_join(receive_thread,NULL);
    pthread_join(send_thread,NULL);
    

    close(acceptfd);
    close(serverfd);

    return 1;
}
    

int client(){
    pthread_t receive_thread,send_thread;
    

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

    struct thread_args client_thread_args ={
        .sockfd=sockfd,
        .acceptfd=sockfd
    };
    pthread_create(&receive_thread,NULL,&receive_msg,&client_thread_args);
    pthread_create(&send_thread,NULL,&send_msg,&client_thread_args);

    pthread_join(receive_thread,NULL);
    pthread_join(send_thread,NULL);

    close(sockfd);
    
    return 0;
}

void* receive_msg(void* args){
    struct thread_args* receive_args=(struct thread_args*) args;
        //prevent zero char sending
        char *buf = malloc(MSG_CHAR_LIMIT);
        while(1){
            memset(buf,0,MSG_CHAR_LIMIT);
            ssize_t bytes = recv(receive_args->acceptfd, buf, MSG_CHAR_LIMIT-1,0);


            if(bytes<0){
                printf("\nError recieving message");
                exit(EXIT_FAILURE);
            }
            else if(bytes == 0){
                printf("\nClient has disconnected");
                exit(EXIT_FAILURE);
            }
            else{
                printf("received: %s",buf);
                fflush(stdout);
            }
        }

        free(buf);
}

void* send_msg(void *args){
    struct thread_args* send_args=(struct thread_args*) args;

    char *buf = malloc(MSG_CHAR_LIMIT);
    while(1){
        buf=fgets(buf, MSG_CHAR_LIMIT, stdin);


        assert(buf!=NULL);

        if(empty_string_check(buf)){
            continue;
        }
        ssize_t bytes = send(send_args->sockfd, buf, strlen(buf),0);
        printf("sent: %s",buf);
        fflush(stdout);
    }
        free(buf);
}

int empty_string_check(char *buf){
    int count=0;
    while(*buf){
        if(!isspace((unsigned char)*buf)){
            return 0;
        }
        count++;
        buf++;
    }
    return 1;
}
