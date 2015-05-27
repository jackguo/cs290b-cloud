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

int do_listen(const int port, const int bTcp)
{
 int s = 0, r = 0, o = 1;
 struct sockaddr_in h; 
 memset(&h, 0, sizeof(h));
 h.sin_family = AF_INET; h.sin_port = htons(port);
 h.sin_addr.s_addr = INADDR_ANY;
 s = socket(AF_INET, bTcp?SOCK_STREAM:SOCK_DGRAM, 0);
 if (s < 1) { perror("socket(listen)"); return 0;}
 r = setsockopt(s, SOL_SOCKET,SO_REUSEADDR, (char *)&o, sizeof(int));
 if (r == -1) { perror("setsockopt(listen)"); return 0;}
 r = bind(s, (struct sockaddr *)&h, sizeof(h));
 if (r == -1) { perror("bind(listen)"); return 0;}
 if (bTcp) {
  r = listen(s, SOMAXCONN);
  if (r == -1) { perror("listen()"); return 0;}
 }/*end if*/
 /*signal(SIGPIPE, SIG_IGN);*/
 return s;
}/*end do_listen*/

void response(int sck, struct sockaddr_in * host)
{
 FILE * f = 0; char cmd[szSTR]; time_t now;
 struct tm * t = 0;
 f = fdopen(sck, "w+");
 while(!feof(f)) {
  memset(cmd, 0, szSTR);
  fgets(cmd, szSTR -1, f);
  time(&now);
  t = localtime(&now);
  if(strstr(cmd, "DATE")) { 
   fprintf(stderr, "Query Type: Date\n");
   fprintf(f, "Date: %d %d, %d\n", t->tm_mon + 1, t->tm_mday, t->tm_year + 1900); 
   continue; 
  }/*end if*/
  if(strstr(cmd, "TIME")) { 
   fprintf(stderr, "Query Type: Time\n");
   fprintf(f, "Time: %02d::%02d::%02d\n", t->tm_hour, t->tm_min, t->tm_sec); 
   continue; 
  }/*end if*/
  if(strstr(cmd, "EXIT")) break;
  fprintf(f, "commands: DATE, TIME, EXIT.\n");
 }/*end while*/
 shutdown(sck, SHUT_RDWR);
 close(sck);
 fclose(f);
 return ;
}/*end response*/

int main(void)
{
 socklen_t sklen = 0;int sck = 0, i = 0, listener = 0; 
 struct sockaddr_in client; pid_t proc = 0;
 system("ifconfig");
 listener = do_listen(SERVERPORT, 1); 
 if(listener < 1) { perror("listen()"); return 0; }
 for(i=0;i<2;i++) {
  memset(&client, 0, sizeof(client));
  sklen = sizeof(client);
  sck = accept(listener, (struct sockaddr *)&client, &sklen);
  if(sck < 1) break;
  proc = fork();
  if (proc == -1) { perror("fork()"); break ; }
  if(proc) continue;
  response(sck, &client);
  break;
 }/*next*/
 close(listener);
 return 0;
}/*end main*/
