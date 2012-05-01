#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include "vt.h"
#include "event.h"

#define PORT 4000
#define LISTEN_SIZ 10

int vt_sockfd_new(){
    int r, opt, oflag;
    int sockfd;
    struct sockaddr_in servaddr;

    // initialize the sock fd
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        vt_log("bad socket.");
        return -1;
    }
    opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(tmp));
    // set up non-blocking
    oflag = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, oflag | O_NONBLOCK);
    // set up sockaddr
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    // 
    r = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (r < 0) {
        vt_log("bad bind.");
        return -1;
    }
    //
    r = listen(sockfd, LISTEN_SIZ);
    if (r < 0) {
        vt_log("bad listen.");
        return -1;
    }
    return sockfd;
}

