#ifndef server_h
#define server_h

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define BRED   "\x1B[2;31m"
#define BGRN   "\x1B[2;32m"
#define BYEL   "\x1B[2;33m"
#define BBLU   "\x1B[2;34m"
#define BMAG   "\x1B[2;35m"
#define BCYN   "\x1B[2;36m"
#define BWHT   "\x1B[2;37m"
#define REDBL   "\x1B[31;40m"
#define GRNBL   "\x1B[32;40m"
#define YELBL   "\x1B[33;40m"
#define BLUBL   "\x1B[34;40m"
#define MAGBL   "\x1B[35;40m"
#define CYNBL   "\x1B[36;40m"

int sockd;
pthread_mutex_t list, whois, file; // lock for writing to file

typedef struct user
{
  char name[256];
  int sd;
  int flag;
  int color;
  int privID;
} User;


int claim_port( const char * port );
void* clientSession( void * arg );
int userCreate(int sd, char request[], User * arg );
int createprivate(int sd, char request[], User *arg);
int endprivate(int sd, char request[], User * arg, int privsd);
void findusers(User * arg);
void deleteuser(char username[], User *arg);
#endif
