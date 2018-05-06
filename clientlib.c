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

int flag = 0;
int servConnect(const char* server, const char* port)
{
	int		sd;
  char	message[256];

	if ( getaddrinfo( server, port, &addrinfo, &result ) != 0 ){
		fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed.  File %s line %d.\x1b[0m\n", server, __FILE__, __LINE__ );
		return -1;
	}else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 ){
		freeaddrinfo( result );
		return -1;
	}else{
		do {
			if ( errno = 0, connect( sd, result->ai_addr, result->ai_addrlen ) == -1 ){
				sleep( 1 );
				write( 1, message, sprintf( message, "\x1b[2;33mConnecting to server %s ...\x1b[0m\n", server ) );
			}else{
				freeaddrinfo( result );
				return sd;		// connect() succeeded
			}
		} while ( errno == ECONNREFUSED );
		freeaddrinfo( result );
		return -1;
	}
}

void* servRead(void* test){

    char input[2048], command[32];;
     
    while(read(fd, input, sizeof(input)) > 0){
	    char checkexit[] = "@exit";
	    char checkwho[] = "@who";
	    char Users[2048];
	    sscanf(input,"%s", command);

	    if(strcmp(command, checkexit) == 0){
	        flag = 1;
	        return 0;
	    }else if(strcmp(command, checkwho) == 0){
	        FILE * op;
	        op = fopen("ActiveUsers.txt", "r");
	        while(fgets(Users, 100, (FILE*)op) != NULL){
	            printf("%s", Users);
	        }
	        fclose(op);
	    }else{
	        write( 1, input, strlen(input) + 1);
	        memset(&input[0], 0, sizeof(input));
	    }
	}
}

void* servPrompt(void* test){

    pthread_detach( pthread_self() );
    char input[2048];
    char history[2048];
    printf("Chat History:\n");
    FILE *fp;
    fp = fopen("history.txt", "r");

    while(fgets(history, 1000, (FILE*)fp) != NULL)
        printf("%s", history);

    fclose(fp);
    printf("\nEnter username : @name yourname\n");
    while(flag != 1){
        read(0, input, sizeof(input));
        write(fd, input, (strlen(input)+1) );
        memset(&input[0], 0, sizeof(input));
        sleep(1);
    }
    printf("Exiting\n");
    return 0;
}
