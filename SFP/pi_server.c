#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <wiringPi.h>
#include <mysql/mysql.h>

#define BUF_SIZE 50
#define INTER_PIN 18

void error_handling(char *msg);
void wiring_init();

int serv_sock, clnt_sock;

static char *host ="localhost";
static char *user ="root";
static char *pass ="kcci";
static char *dbname ="arduino";

int main(int argc, char *argv[]){
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	int str_len = 0;
	char msg[BUF_SIZE];
	int sock_option  = 1;
	char temp[200];
	int res;
	MYSQL * conn_m;
	MYSQL * conn_t;
	int open_flag=0;
	char *pToken;
	int moisture;
	float temperature;
	char msg_open[]="open\n";
	char msg_close[]="close\n";

char *pArray[3]={0};

	conn_m=mysql_init(NULL);
	conn_t=mysql_init(NULL);
	if(!(mysql_real_connect(conn_m,host,user,pass,dbname,0,NULL,0))){
		 fprintf(stderr,"ERROR : %s[%d]|\n",mysql_error(conn_m),mysql_errno(conn_m));
		 exit(1);
	}
	if(!(mysql_real_connect(conn_t,host,user,pass,dbname,0,NULL,0))){
		 fprintf(stderr,"ERROR : %s[%d]|\n",mysql_error(conn_t),mysql_errno(conn_t));
		 exit(1);
	}

   printf("DB Connection Successful!\n\n");
	
	wiring_init();

	if(argc != 2) {
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	 }
		
	fputs("IoT Server Start!!\n",stdout);

	serv_sock=socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_option, sizeof(sock_option)); 
	
	if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr))==-1)
		 error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		 error_handling("listen() error");
	
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
	
	printf("New connected!\n");
	write(clnt_sock,"New connected\n",14);
	while( ( str_len=read(clnt_sock,msg,BUF_SIZE)) >=0 ){
		msg[str_len]='\0';
		printf("%s\n",msg);
		/*
		pToken=strtok(msg,":");
		moisture=atoi(pToken);
		pToken=strtok(NULL,":");
		strcpy(temp,pToken);
		//temperature=atof(pToken);
		printf("%.1f\n",temp);*/

		int i;
		pToken = strtok(msg,"[:]");
		i = 0; 
		while(pToken != NULL)
		{
			pArray[i] =  pToken;
			if(++i >= 3)
			break;	
			pToken = strtok(NULL,"[:]");
		}
		
		moisture=atoi(pArray[0]);
		temperature=atof(pArray[1]);

		printf("%d\n",moisture);
		printf("%f\n",temperature);

		sprintf(temp,"insert into temperature(id,date,time,temperature) values(null,curDate(),curTime()," "'%.1f')",temperature);
		res = mysql_query(conn_t,temp);
		if(!res)
			printf("inserted %1u rows temperature table\n", (unsigned long)mysql_affected_rows(conn_t));
		else
			fprintf(stderr,"Insert error %d: %s\n",mysql_errno(conn_t),mysql_error(conn_t));

		if(moisture<300){
			sprintf(temp,"insert into moisture(id,date,time,moisture) values(null,curDate(),curTime()," "'%d')",moisture);
			res = mysql_query(conn_m,temp);
			if(!res)
				printf("inserted %1u rows moisture table\n", (unsigned long)mysql_affected_rows(conn_m));
			 else
				fprintf(stderr,"Insert error %d: %s\n",mysql_errno(conn_m),mysql_error(conn_m));

			write(clnt_sock,"low\n",4);
		}
			
	}
	
	close(clnt_sock);
	mysql_close(conn_m);
	mysql_close(conn_t);

	return 0;

}

void error_handling(char *msg)
{
	   fputs(msg, stderr);
		   fputc('\n', stderr);
			   exit(1);
}

void force_open(){
	write(clnt_sock,"low\n",4);

}


void wiring_init(){
	if(wiringPiSetupGpio()==-1){
		 printf("wiring pi setup error");
		 exit(1);
	}
	pinMode(INTER_PIN,INPUT);
	wiringPiISR(INTER_PIN,INT_EDGE_FALLING,force_open);
}





