#include        <sys/types.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <errno.h>
#include        <string.h>
#include        <sys/socket.h>
#include        <netdb.h>
#include        <pthread.h>
#include        "server.h"

int main( int argc, char ** argv ){

        int   sd, fd, i;
        char  message[256];
        struct sockaddr_in  clientAddr;
        socklen_t           addrlen;
        pthread_t           thread_id;
        User * users;

        users = malloc(20 * sizeof(User));

        for(i= 0; i < 20; i++){
          strcpy(users[i].name, "avail");
          users[i].flag = 0;
          users[i].color = i;
          users[i].privID = 0;
        }if ( (sd = claim_port( "54321" )) == -1 ){
              write( 1, message, sprintf( message,  "\x1b[1;31mCould not bind to port %s errno %s\x1b[0m\n", "3000", strerror( errno ) ) );
              return 1;
        }else{
                addrlen = sizeof(clientAddr);
                while ( (fd = accept( sd, (struct sockaddr *)&clientAddr, &addrlen )) != -1 ){
                        sockd = fd;
                        if ( pthread_create( &thread_id, 0, clientSession, users ) != 0 ){
                                printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
                                return 0;
                        } else
                            continue;
                }
                printf( "Not accepting connections\n");
                close( sd );
                return 0;
        }
}
