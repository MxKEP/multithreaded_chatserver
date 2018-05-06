#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<netdb.h>
#include  <pthread.h>
#include	<sys/socket.h>
#include	<unistd.h>
#include	<string.h>
#include  "client.h"


int main( int argc, char ** argv ){

	int		sd, len, count;
	char	message[512], string[512], buffer[512], output[512];
	addrinfo.ai_family = AF_INET;		// IPv4
	addrinfo.ai_socktype = SOCK_STREAM;	//TCP
	addrinfo.ai_flags = 0;
	addrinfo.ai_protocol = 0;
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_next = NULL;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;

	if ( argc < 2 ){
		fprintf( stderr, "\x1b[1;31m./client <server domain> arguments needed : File %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		exit( 1 );
	}else if ( (sd = servConnect( argv[1], "54321" )) == -1 ){
		write( 1, message, sprintf( message,  "\x1b[1;31mConnection to server failed. %s errno %s\x1b[0m\n", argv[1], strerror( errno ) ) );
		return 1;
	}else{
		fd = sd;
		printf( "Connected to server : %s\n", argv[1] );
		pthread_t thread_id;
	    if ( pthread_create( &thread_id, 0, servPrompt, 0 ) != 0 ){
						printf( "\x1b[2;31mpthread_create() failed file %s, line %d.\x1b[0m\n",__FILE__, __LINE__ );
						return 0;
	}if ( pthread_create( &thread_id, 0, servRead, 0 ) != 0 ){
		printf( "\x1b[2;31mpthread_create() failed file %s, line %d.\x1b[0m\n",
			__FILE__, __LINE__ );
		return 0;
	}else{
		pthread_exit( 0 );
	}
		close( sd );
		return 0;
	}
}
