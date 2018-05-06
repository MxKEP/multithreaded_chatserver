#include        <sys/types.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <errno.h>
#include        <string.h>
#include        <sys/socket.h>
#include        <netdb.h>
#include        <pthread.h>
#include         "server.h"


int claim_port( const char * port )
{
  struct addrinfo         addrinfo;
  struct addrinfo *       result;
  int    sd,on = 1;
  char   message[256];

  addrinfo.ai_flags = AI_PASSIVE;
  addrinfo.ai_family = AF_INET;
  addrinfo.ai_socktype = SOCK_STREAM;
  addrinfo.ai_protocol = 0;
  addrinfo.ai_canonname = NULL;
  addrinfo.ai_addrlen = 0;
  addrinfo.ai_addr = NULL;
  addrinfo.ai_canonname = NULL;
  addrinfo.ai_next = NULL;
  if ( getaddrinfo( 0, port, &addrinfo, &result ) != 0 )          // want port 3000
  {
    fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed errno is %s.  File %s line %d.\x1b[0m\n", port, strerror( errno ), __FILE__, __LINE__ );
    return -1;
  }else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )  {
        write( 1, message, sprintf( message, "socket() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
        freeaddrinfo( result );
        return -1;
      }else if ( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) == -1 ){
          write( 1, message, sprintf( message, "setsockopt() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
          freeaddrinfo( result );
          close( sd );
          return -1;
        }else if ( bind( sd, result->ai_addr, result->ai_addrlen ) == -1 ){
            freeaddrinfo( result );
            close( sd );
            write( 1, message, sprintf( message, "\x1b[2;33mBinding to port %s ...\x1b[0m\n", port ) );
            return -1;
        }else if ( listen( sd, 100 ) == -1 ){
             printf( "listen() failed in file %s line %d\n", __FILE__, __LINE__ );
             close( sd );
             return 0;
          }else{
              write( 1, message, sprintf( message,  "\x1b[1;32mSUCCESS : Bind to port %s\x1b[0m\n", port ) );
              freeaddrinfo( result );
              return sd;                      // bind() succeeded;
        }
}

void * clientSession( void * arg ){

  int  sd, i;
  char request[2048];
  int x = 0;
  int nameflag = 0;

  User * arg1 = arg;
  sd = sockd;
  pthread_detach( pthread_self() );

  while ( read( sd, request, sizeof(request) ) > 0 ){

  char username[300];
  char usernamenc[300];
  char checkname[] = "@name";
  char checkprivate[] = "@private";
  char checkend[] = "@end";
  char checkwho[] = "@who";
  char checkexit[] ="@exit";
  char command[100];
  char operand[2048];
  char namemess[] = "\nYou already created a username, Exit and restart to create a new username\n";
  char noendmess[] = "\nYou are not in a private session, cannot execute end command\n";
  char nopriv[] = "\nYou are already in a private session, cannot execute private command\n";

  sscanf(request, "%s %s", command, operand);
  if(x!= 0){
    if(strcmp(command, checkend) == 0){
          int privsd = x;
          x = endprivate(sd, request, arg1, privsd);
          continue;
    }else if(strcmp(command, checkprivate) == 0)  {
            write(sd, nopriv, strlen(nopriv)+1 );
            continue;
          }else if(strcmp(command, checkwho) == 0){
                  pthread_mutex_lock(&whois);
                  findusers(arg1);
                  write(sd, command, strlen(command) +1);
                  pthread_mutex_unlock(&whois);
                  continue;
                }else if(strcmp(command, checkexit) == 0){
                      write(sd, command, strlen(command) +1);
                      deleteuser(usernamenc, arg1);
                    }else{
                        char outputmess[2048];
                        strcpy(outputmess, username);
                        strcat(outputmess, ": ");
                        strcat(outputmess, request);
                        write(x, outputmess, strlen(outputmess) + 1);
                        memset(&outputmess[0], 0, sizeof(outputmess));
                    }
            }
            if(x == 0){
                if(strcmp(command , checkname) == 0){
                   if(nameflag == 1){
                        write(sd, namemess, strlen(namemess)+1);
                        continue;
                  }else{
                      pthread_mutex_lock(&list);
                      int y = userCreate(sd, request, arg1 );
                      if(y == 1){
                          strcpy(usernamenc, operand); // username without color
                          nameflag = 1;
                          int k = 0;
                          int m = 0;
                          while(m != 1){
                            if(strcmp(arg1[k].name, usernamenc) == 0){
                              if(arg1[k].color == 0){
                                strcpy(username, GRN);
                                    strcat(username, usernamenc);
                                    strcat(username, RESET);
                                    m = 1;
                                  }else if(arg1[k].color == 1){
                                    strcpy(username, BLU);
                                    strcat(username, usernamenc);
                                    strcat(username, RESET);
                                    m = 1;
                                    }else if(arg1[k].color == 2){
                                      strcpy(username, YEL);
                                      strcat(username, usernamenc);
                                      strcat(username, RESET);
                                      m = 1;
                                      }else if(arg1[k].color == 3){
                                        strcpy(username, RED);
                                        strcat(username, usernamenc);
                                        strcat(username, RESET);
                                        m = 1;
                                        }else if(arg1[k].color == 4){
                                          strcpy(username, MAG);
                                          strcat(username, usernamenc);
                                          strcat(username, RESET);
                                          m = 1;
                                        }else if(arg1[k].color == 5){
                                           strcpy(username, CYN);
                                           strcat(username, usernamenc);
                                           strcat(username, RESET);
                                           m = 1;
                                         }else if(arg1[k].color == 6){
                                           strcpy(username, WHT);
                                           strcat(username, usernamenc);
                                           strcat(username, RESET);
                                           m = 1;
                                          }else if(arg1[k].color == 7){
                                            strcpy(username, BRED);
                                            strcat(username, usernamenc);
                                            strcat(username, RESET);
                                            m = 1;
                                            }else if(arg1[k].color == 8){
                                              strcpy(username, BGRN);
                                              strcat(username, usernamenc);
                                              strcat(username, RESET);
                                              m = 1;
                                              }else if(arg1[k].color == 9){
                                                strcpy(username, BYEL);
                                                strcat(username, usernamenc);
                                                strcat(username, RESET);
                                                m = 1;
                                                }else if(arg1[k].color == 10){
                                                  strcpy(username, BWHT);
                                                  strcat(username, usernamenc);
                                                  strcat(username, RESET);
                                                  m = 1;
                                                  }else if(arg1[k].color == 11){
                                                    strcpy(username, BBLU);
                                                    strcat(username, usernamenc);
                                                    strcat(username, RESET);
                                                    m = 1;
                                                    }else if(arg1[k].color == 12){
                                                      strcpy(username, BMAG);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }
                                                    else if(arg1[k].color == 13){
                                                      strcpy(username, BCYN);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 14){
                                                      strcpy(username, REDBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 15){
                                                      strcpy(username, GRNBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 16)  {
                                                      strcpy(username, YELBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 17){
                                                      strcpy(username, BLUBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 18){
                                                      strcpy(username, MAGBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }else if(arg1[k].color == 19){
                                                      strcpy(username, CYNBL);
                                                      strcat(username, usernamenc);
                                                      strcat(username, RESET);
                                                      m = 1;
                                                    }
                                                  }else
                                                    k++;

                              }
                          }

                                        pthread_mutex_unlock(&list);
                                        continue;
                                }
                        }
                        else if(strcmp(command, checkprivate) == 0)
                        {
                                x = createprivate(sd, request, arg1);
                                continue;
                        }
                        else if(strcmp(command, checkend)==0)
                        {
                                write(sd, noendmess, strlen(noendmess)+1);
                                continue;
                        }
                        else if(strcmp(command, checkwho) == 0)
                        {
                                  pthread_mutex_lock(&whois);
                                  findusers(arg1);
                                  write(sd, command, strlen(command) +1);
                                  pthread_mutex_unlock(&whois);
                                  continue;
                        }
                        else if(strcmp(command, checkexit) == 0)
                        {
                                write(sd, command, strlen(command) +1);
                                deleteuser(usernamenc, arg1);
                        }
                        else
                        {
                                if(nameflag == 1)
                                {
                                        int j;
                                        pthread_mutex_lock(&file);
                                        FILE * fp;
                                        fp = fopen("history.txt", "a+");
                                        char output[2048];
                                        strcpy(output, username);
                                        strcat(output, ": ");
                                        strcat(output, request);
                                        fputs(output, fp);
                                        fclose(fp);
                                        pthread_mutex_unlock(&file);
                                        for(j = 0; j<20; j++)
                                        {
                                                if(arg1[j].flag == 1)
                                                {
                                                  write(arg1[j].sd, output, strlen(output)+1);
                                                }
                                        }
                                        memset(&output[0], 0, sizeof(output)); // reset
                                }else
                                    continue;

                        }
                }
        }
        close( 0 );
}

int userCreate(int sd, char request[], User * arg )
{
        char command[20], name[100];
        char notused[] = "avail";
        int j, i;
        char success[] = "\nUsername created\nEnter text: ";
        char max_message[] = "\nMax number of users reached. Try again later\n";
        char accountuser[] = "\nUsername already in use. Try again\n";
        sscanf(request, "%s %s", command , name);

        for(j = 0; j<20; j++)
        {
                if(strcmp(arg[j].name, name) == 0)
                {
                        write(sd, accountuser, strlen(accountuser)+1);
                        return 0;
                }
        }
        for(i = 0; i<20; i++)
        {
                if(strcmp(arg[i].name, notused) == 0)
                {
                        strcpy(arg[i].name, name);
                        arg[i].sd = sd;
                        arg[i].flag = 1;
                        write(sd, success, strlen(success)+1 );
                        return 1;
                }
                if(arg[19].flag == 1)
                {
                        write(sd, max_message, strlen(max_message) + 1);
                        return 0;
                }
        }


}

int createprivate(int sd, char request[], User *arg){

    int i;
    char command[32], privname[100];
    int privsd = 0;
    char privmess[] = "\nPrivate message session created\n";
    char noprivmess[] = "\nCould not find user\n";

    sscanf(request, "%s %s", command , privname);
    for(i = 0; i<20; i++){
        if(strcmp(arg[i].name, privname) == 0){
            privsd = arg[i].sd;
            arg[i].privID = 1;
            write(sd, privmess, strlen(privmess)+1);
            return privsd;
        }else if(i == 19){
            write(sd, noprivmess, strlen(noprivmess) +1);
                return 0;
            }
        }
}

int endprivate(int sd, char request[], User * arg, int privsd)
{
      char command[32], privname[64];;
      int j, x;
      x = privsd;
      char endmess[] = "\nPrivate session ended, all messages now public\n";
      char noendmess[] = "\nCould not find private session, cannot execute end command\n";
      sscanf(request, "%s %s", command , privname);

      for(j = 0; j<20; j++)  {
          if(strcmp(arg[j].name, privname) == 0){
                write(sd, endmess, strlen(endmess)+1);
                arg[j].privID = 0;
                return 0;
                }else if(j == 19){
                   write(sd, noendmess, strlen(noendmess)+1);
                   return x;
                }
        }

}

void deleteuser(char username[], User *arg)
{
        int j;
        for(j = 0; j<20; j++)
        {
                if(strcmp(arg[j].name, username) == 0)
                {
                        strcpy(arg[j].name, "avail");
                        arg[j].sd = 0;
                        arg[j].flag = 0;
                        return;
                }
        }
}

void findusers(User * arg)
{
  char available[] = "avail";
  FILE * fp;
  fp = fopen("ActiveUsers.txt", "w");
  int j;
  for (j = 0; j<20; j++)
  {
    if(strcmp(arg[j].name, available) != 0)
    {
      fputs(arg[j].name, fp);
      if(arg[j].privID == 1)
      {
        fputs(" ", fp);
        fputs("private", fp);
      }
      fputs("\n", fp);

    }
  }
  fclose(fp);
}
