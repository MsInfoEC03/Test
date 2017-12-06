#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 4096
#define DEFAULT_PORT 6666 

pid_t pid;
int sockfd;
char buff[20]={0};
char buf[MAXLINE];
struct sockaddr_in broad;
struct sockaddr_in servaddr;
void message();
void display();
static int num=1;

struct msg              //******   structure
{
        char ip[20];
        char name[20];
};

struct msg people[20]={0};

void message()
{
        int i=0,len=0,date=0;
        //static int num=1;
        //len=sizeof(buf);
        if(((buf[0]=='o')&&(buf[1]=='n')&&(buf[6]==':'))||((buf[0]=='u')&&(buf[1]=='p')&&(buf[2]=='d')))
        {
          date=num;
            for(i=0;i<num;i++)
           {
                if(people[i].name[0] == '\0')
                {  date=i; break;  }
           }
          if(date == num) num++;

             for(i=7;buf[i]!='\0';i++)
                {
                    people[date].name[len]=buf[i];
                    len++;
                }

        people[date].name[len]='\0';    //**********************
        len=0;
        for(i=0;i<13;i++)
        {
                people[date].ip[len]=buff[i];
                len++;
        }
        people[date].ip[len]='\0';      //*************************

        }


        if((buf[0]=='o')&&(buf[1]=='f')&&(buf[2]=='f')&&(buf[7]==':'))
        {
            for(i=0;i<num;i++)
            {
                if((people[i].ip[10]==buff[10])&&(people[i].ip[11]==buff[11]))
                {
                memset(people[i].name,'\0',sizeof(people[i].name));
                memset(people[i].ip,'\0',sizeof(people[i].ip));
                }
            }
        }

        if((buf[0]==':')&&(buf[1]=='l')&&(buf[2]=='i')&&(buf[3]=='s'))
        {
	printf("******online people******\n");
        for(i=1;i<num;i++)
            {
            if(people[i].name[0] != '\0')
                printf("people = %s : %s\n",people[i].name,people[i].ip);
            }
	printf("******online people******\n");
        }

}
void display()
{
        int i=0;
        for(i=0;i<num;i++)
        {
                if(people[i].name[0] != '\0')
                        printf("online people %s : %s\n",people[i].name,people[i].ip);
        }
}

static void sig(int signo)
{
        char bye[14]={"offline: Peter"};
        servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
        sendto(sockfd,bye,14,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
        close(sockfd);
        exit(0);
}


int main(int argc,char* argv[])
{
//      char buff[20]={0};
//      struct sockaddr_in servaddr;
        int addr_len = sizeof(struct sockaddr_in);
        int n,i=0;
        int on=1,flagall=1;
//      char buf[MAXLINE];
        char line[13]={"online: Peter"};
        char cop[14]={0};
        char update[13]={"update: Peter"};
        broad.sin_family = AF_INET;
        broad.sin_port = htons(6666);
        broad.sin_addr.s_addr = inet_addr("255.255.255.255");

        if(signal(SIGINT,sig) == SIG_ERR)
                printf("signal eror.\n");
        if((pid = fork()) < 0)
                printf("fork error\n");
        if(pid > 0)
        {
                if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 )
                {
                        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
                        exit(0);
                }
                setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));

                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//              servaddr.sin_addr.s_addr = htonl("255.255.255.255");

                servaddr.sin_port = htons(DEFAULT_PORT);

                if( bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr) ) == -1 )
                {
                        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
                        exit(0);
                }

                printf("------waiting for receve message------\n");

                sendto(sockfd,line,13,0,(struct sockaddr *)&broad,sizeof(broad));

                while(1)
                {
                        if((n = recvfrom(sockfd, buf, sizeof(buf), 0,(struct sockaddr*)&servaddr, &addr_len)) > 0)
                        {
                                inet_ntop(AF_INET,&servaddr.sin_addr,buff,sizeof(buff));
                                message();
                                if((buf[0]=='o')&&(buf[1]=='n')&&(buf[2]=='l'))
                                {
                                //      printf("update.name\n");
				
        			servaddr.sin_port = htons(6666);
                                sendto(sockfd,update,13,0,(struct sockaddr *)&servaddr,sizeof(servaddr));

                                }
				for(i=0;i<num;i++)
				{
                		   if((people[i].ip[10]==buff[10])&&(people[i].ip[11]==buff[11]))
				   break;
				}
                                printf("Receve message from  %s : %s\n",people[i].name,buf);
                        }
                                memset(buf, 0, sizeof(buf));
                                memset(buff, 0, sizeof(buff));
                }
        }
        else
        {
/*              if( argc != 2)
                {
                        printf("usage: ./client <IP address>\n");
                        exit(0);
                }
*/
                if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
                {
                        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
                        exit(0);
                }

                setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));

                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_port = htons(DEFAULT_PORT);
                servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
/*              if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
                {
                        printf("inet_pton error for %s\n",argv[1]);
                        exit(0);
                }
*/
                while(1)
                {
                        printf("Please write something: \n");
                        memset(buf, 0, sizeof(buf));
                        n = read(STDIN_FILENO, buf, MAXLINE);
                        if(buf[0] == ':')
                        {
                                if(buf[1] == 'a')
                                {
                                        flagall=0;
                                        servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");

                                }
                                else if (buf[1] == '1')
                                {
                                        flagall=0;
                                        for(i=0;i<13;i++)
                                        {
                                                cop[i]=buf[i+1];
                                        }
                                        cop[13]='\0';
                //                      printf("cop= %s.\n",cop);
                                        servaddr.sin_addr.s_addr = inet_addr(cop);              //      特定用户 IP
                                }
                        }
                if(flagall)
                {
                        if( sendto(sockfd, buf,4096, 0, (struct sockaddr*)&servaddr, addr_len) == -1)
                        {
                                printf("sendto msg error: %s(errno: %d)\n", strerror(errno), errno);
                                exit(0);
                        }
                }
                flagall=1;
                }
                close(sockfd);
        }
        return 0;
}
  
