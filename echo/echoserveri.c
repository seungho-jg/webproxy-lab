#include "csapp.h"

void echo(int connfd);
void sigchld_handler(int sig)
{
    while (waitpid(-1, 0, WNOHANG) > 0)
        ;
    return;
}
int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;

    //char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    } 
    port = atoi(argv[1]);

    Signal(SIGCHLD, sigchld_handler);  
    listenfd = Open_listenfd(port);
    while (1)
    {
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        if(Fork() == 0){
            Close(listenfd);
            echo(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }
    exit(0);
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0){
        printf("server recived %d bytes\n", (int)n);
        printf("server recived %s \n", buf);

        char mod[MAXLINE];
        sprintf(mod,"태욱이: %s",buf);
        Rio_writen(connfd, mod, strlen(mod));
    }
}