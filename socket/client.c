#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define szSTR 256
#define SERVERPORT 21429 /*please modify the port with your id*/
int cnn(const char * ip, const int port)
{
 struct sockaddr_in h; memset(&h, 0, sizeof(h));
 h.sin_family = AF_INET; h.sin_port = htons(port);
 h.sin_addr.s_addr = inet_addr(ip);
 int s = socket(AF_INET, SOCK_STREAM, 0);
 if (s < 1) { perror("socket(tcp)"); return 0;}
 int r = connect(s, (struct sockaddr *)&h, sizeof(h));
 if (r == 0) return s;
 perror("connect()");
 return 0;
}//end cnn

int main(void)
{
 int sck = 0; FILE * f = 0; char ip[szSTR]="127.0.0.1";
 fprintf(stderr, "Please input the calendar server ip:");
 fgets(ip, szSTR - 1, stdin);
 sck = cnn(ip, SERVERPORT);
 if(sck < 1) return 0;
 f = fdopen(sck, "w+");
 fprintf(f, "DATE\r\n");
 fgets(ip, szSTR -1 , f);
 fprintf(stderr, "%s\n", ip);
 fprintf(f, "TIME\r\n");
 fgets(ip, szSTR -1 , f);
 fprintf(stderr, "%s\n", ip);
 fprintf(f, "EXIT\r\n");
 fclose(f);
 close(sck);
 return 0;
}/*end main*/
