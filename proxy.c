#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int connfd);
void make_header(char *final_header, char *hostname, char *path, rio_t *client_rio);
int server_connection(char *hostname, int port);
void parse_uri(char *uri, char *hostname, int *port, char *path);


int main(int argc, char **argv) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  if (argc != 2){
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  while (1) {
    clientlen = sizeof(clientaddr);

    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);

    doit(connfd);
    Close(connfd);
  }

  printf("%s", user_agent_hdr);
  return 0;
}
// doit:
  void doit(int connfd) {
    int serverfd, port;
    char server_header[MAXLINE];
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLINE];
    rio_t clientrio, serverrio;

    Rio_readinitb(&clientrio, connfd);
    Rio_readlineb(&clientrio, buf, MAXLINE);
    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET")){
      printf("Proxy does not implement the method.");
      return;
    }

    parse_uri(uri, hostname, &port, path);
    make_header(server_header, hostname, path, &clientrio);

    serverfd = server_connection(hostname, port);

    Rio_readinitb(&serverrio, serverfd);
    Rio_writen(serverfd, server_header, strlen(server_header));

    size_t response;
    while ((response = Rio_readlineb(&serverrio, buf, MAXLINE)) != 0){
      Rio_writen(connfd, buf, response);
    }

    Close(serverfd);

  }

  void make_header(char *final_header, char *hostname, char *path, rio_t *client_rio) {
    char buf[MAXLINE];
    char request_header[MAXLINE], host_header[MAXLINE], etc_header[MAXLINE];

    sprintf(request_header, "GET %s HTTP/1.0\r\n", path);

    while(Rio_readlineb(client_rio, buf, MAXLINE)){
      if (strcmp(buf, "\r\n") == 0){
        break;
      }
      if (!strncasecmp(buf, "Host", strlen("Host"))){
        strcpy(host_header, buf);
        continue;
      }

      if (strncasecmp(buf, "USer-Agent", strlen("User-Agent")) 
        && strncasecmp(buf, "Connection", strlen("Connection"))
        && strncasecmp(buf, "Proxy-Connection", strlen("Proxy-Connection"))){
          strcat(etc_header, buf);
        }
    }
    if(strlen(host_header) == 0) {
      sprintf(host_header, "Host: %s\r\n", hostname);
    }
    sprintf(final_header, "%s%s%s%s%s%s%s",
      request_header,
      host_header,
      "Connection: close\r\n",
      "Proxy-Connection: close\r\n",
      user_agent_hdr,
      etc_header,
      "\r\n");
  }

inline int server_connection(char *hostname, int port){
  char portStr[100];

  sprintf(portStr, "%d", port);
  return Open_clientfd(hostname, portStr);
}

void parse_uri(char *uri, char *hostname, int *port, char *path) {
  char *first = strstr(uri, "//");

  first = first != NULL ? first+2 :uri;

  char *next = strstr(first, ":");

  *port = 80;
  if (next) {
    *next = '\0';
    sscanf(first, "%s", hostname);

    sscanf(next+1, "%d%s", port, path);
  } else {
    next = strstr(first, "/");

    if (next) {
      *next = '\0';
      sscanf(first, "%s", hostname);

      *next = '/';
      sscanf(next, "%s", path);

    } else {
      sscanf(first, "%s", hostname);
    }
  }
}