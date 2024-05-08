#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    struct addrinfo *result, *rp;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // IPv4 또는 IPv6를 모두 허용
    hints.ai_socktype = SOCK_STREAM; // TCP 소켓 타입

    int ret = getaddrinfo(argv[1], NULL, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return 1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        void *addr;
        char ip_address[INET6_ADDRSTRLEN];
        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        inet_ntop(rp->ai_family, addr, ip_address, INET6_ADDRSTRLEN);
        printf("IP Address: %s\n", ip_address);
    }

    freeaddrinfo(result);
    return 0;
}
