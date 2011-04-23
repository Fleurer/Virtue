#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
//
#include "vt/vt.h"

#define LISTEN_SIZ 10
#define PORT 8888

vconn_t *connmap[FD_SETSIZE] = {NULL, }; 


int main(int argc, char **argv){
    int r, tmp, oflag;
    int port;
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        vpanic("bad socket.");
    tmp = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
    // set up non-blocking
    oflag = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, oflag | O_NONBLOCK);

    port = PORT;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    r = bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    if (r < 0) 
        vpanic("bad bind.");

    r = listen(sockfd, LISTEN_SIZ);
    if (r < 0) 
        vpanic("bad listen.");

    vloop(sockfd);
}

int vloop(int sockfd){
    struct sockaddr_in cliaddr;
    size_t salen;
    int connfd, maxfd, fd;
    int oflag, r;
    fd_set rdset, wrset;
    vconn_t *cp;
    vbuf_head_t *bhp;
    vbuf_t *bp;
    //

    FD_ZERO(&rdset);
    FD_ZERO(&wrset);

    FD_SET(sockfd, &rdset);
    maxfd = sockfd;

    for(;;){
        r = select(maxfd+1, &rdset, &wrset, NULL, NULL);
        //printf("select() returns.\n");
        // on error
        if (r < 0) {
            if (errno == EINTR)
                continue;
        }
        // on accept ok
        if (FD_ISSET(sockfd, &rdset)) {
            fd = accept(sockfd, NULL, NULL);
            if (fd < 0) {
                if (errno==EWOULDBLOCK || errno==EINTR)
                    continue;
                else 
                    vpanic("bad accept().");
            }
            // set it non-block
            oflag = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, oflag | O_NONBLOCK);
            //
            FD_SET(fd, &rdset);
            cp = vconn_new(fd);
            connmap[fd] = cp; 
            maxfd = max(fd, maxfd);
            continue;
        }
        // on reading & writing 
        for (fd=0; fd<maxfd+1; fd++) {
            if (fd==sockfd) continue;
            // on read ready
            if (FD_ISSET(fd, &rdset)) {
                cp = connmap[fd];
                if (cp == NULL) {
                    vpanic("bad conn entry");
                }
                bhp = cp->c_buf_read;
                r = vbuf_read(fd, bhp);
                if (r < 0) {
                    if (errno==EWOULDBLOCK || errno==EINTR) 
                        continue;
                    else 
                        vpanic("bad read.");
                }
                // recieve finished
                if (r == 0) {
                    FD_CLR(fd, &rdset);
                    FD_SET(fd, &wrset);
                    on_read_complete(cp);
                }
                continue;
            }
            // on write ready
            if (FD_ISSET(fd, &wrset)) {
                cp = connmap[fd];
                if (cp == NULL) {
                    vpanic("bad conn entry");
                }
                bhp = cp->c_buf_write;
            }
        }
    }
}

int on_read_complete(vconn_t *cp){
    printf("read complete\n");
}
