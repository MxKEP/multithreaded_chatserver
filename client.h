#ifndef client_h
#define client_h

int fd;
struct addrinfo	addrinfo;
struct addrinfo *	result;

int servConnect(const char* server, const char* port );
void* servPrompt(void* test);
void* servRead(void* test);

#endif
