#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void error(const char *s)
{
    perror(s);
    assert(false);
}

int main(void)
{
    struct sockaddr_in si_me;
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        error("socket");
    }
    
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1)
    {
        error("bind");
    }
    
    for (i=0; i<NPACK; i++)
    {
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)(&si_other), (socklen_t *)&slen)==-1)
        {
            error("recvfrom()");
        }
        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
    }
    
    close(s);
    return 0;
}