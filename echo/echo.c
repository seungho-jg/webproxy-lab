#include "csapp.h"

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