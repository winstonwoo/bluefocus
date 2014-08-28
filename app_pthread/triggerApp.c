#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/times.h>

#define CMD_BEGIN 0xab
#define LENGTH	500

#ifdef MY_LOG
#define myprintf printf
#else
#define myprintf
#endif


clock_t tBeginTime, tEndTime ;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER ;

void *controlThread(void *) ;
void *playThread(void *) ;


int psdata[10] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
int count = 0xff;
int pidkill = 0xff; 

int main(void){

	pthread_t t_control ;
	pthread_t t_play ;

	pthread_create(&t_control, NULL, controlThread, (void*)	NULL) ;
    pthread_create(&t_play, NULL, playThread, (void*)NULL) ;

	pthread_join(t_control) ;

	pthread_mutex_destroy(&mutex) ;
	pthread_cond_destroy(&cond) ;

	exit(0) ;
}

void *controlThread(void *junk){
    int fd ;
  
    char val = 0 ;

    //Open trigger device	
	fd = open ("/dev/trigger", O_RDWR);                                
	if (fd < 0)                                                        
	{                                                                  
		perror ("fail to open");                                       
		exit (-1);                                                     
	}                                                                  

	ioctl (fd, CMD_BEGIN, 0xaa);    
    while(1){
       read(fd, &val, sizeof(char)) ; 
     
       ioctl(fd, CMD_BEGIN, 0xaa) ;	
	   usleep(500) ;


	   tEndTime = times(NULL) ;
       double fCostTime = (double)(tEndTime - tBeginTime)/sysconf(_SC_CLK_TCK) ;
	   if(fCostTime > 20){
	     tBeginTime = tEndTime ;
         insertMovie() ;         
	   } 
	}



    //Close trigger device
	close (fd);                                                                                       
	return 0; 
}

void *playThread(void *junk){


}



void triggerPlay(void){


}



int check_kill_omxplayer(){
	int status ;
	int fd ;
	char str[LENGTH];
	int len = 0 ; 
	char killstr[30] ;
    int  pid ;

	status = system ("pgrep omxplayer.bin > system.dat");
	fd = open ("system.dat", O_RDWR);

	if (fd)
	{   
		len = read (fd, str, LENGTH);
	}   
	close(fd) ;
    
	str[len] = '\0' ;

	printf("length is %d \n", len) ;           
    //Kill the omxplayer.bin if len isn't null	
	if(len){
	  sscanf(str, "%d",pid) ;
	  sprintf(killstr, "kill -13 %d", pid) ;
	  system(killstr) ;
	}   
	
	return 0 ; 
}

int  check_kill_pid(){

	pid_t status;
	int errno = 0;
	char cmdStr[20];
	int i;

	pidkill = 0x0;
	status = system ("pgrep omxplayer.bin > system.dat");

	system_data ();
	if (status == -1)
	{
		myprintf ("system error!");
	}

	if (WIFEXITED (status))
	{
		myprintf ("cp exit normal![%d]/n", errno);
		myprintf ("exit staus = [%X]/n", WEXITSTATUS (status));
	}
	else
		myprintf ("cp exit illegal![%d]/n", errno);

	if(psdata[count] != 0xff){
		sprintf(cmdStr, "kill -13 %d", psdata[count]) ;
		system(cmdStr) ;
	}

	return 0 ;
}

int analyse_pid (void)
{
	int result, len;
	int fd;
	char str[LENGTH];
	int num[5];
	int *p;
	int i;
	p = &num[0];

	fd = open ("system.dat", O_RDWR);

	if (fd)
	{
		len = read (fd, str, LENGTH);
	}
	str[len] = '\0';

	i = 0;
	while (i < LENGTH)
	{
		if ((str[i] == '\n') || (str[i] == '\r'))
			str[i] = ' ';
		i++;
	}

	p = pidstr2num (str);
	close (fd);
	return len;
}


//count the number in one string line and convert them to int 
int pidstr2num (char *strg)

{
	int n, i, j;
	int *number;
	n = i = j = 0;


    //Count the total number amount
	while (strg[i])
	{
		for (; strg[i] && strg[i] == ' '; ++i);
		for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
		i = j;
		++n;
	}

	number = (int *) malloc (sizeof (int) * (n + 1));
	n = i = j = 0;
	count = 0;

    //Convert the number string to int number
	while (strg[i])
	{
		for (; strg[i] && strg[i] == ' '; ++i);
		for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
		number[++n] = atoi (strg + i);
		psdata[n] = number[n];
		i = j;
	}
	
	pidkill = psdata[n - 1];
	number[0] = n;
	count = n;
	return number;
}


int insertMovie(){
	int status ;
	int fd ;
	char str[LENGTH];
	int len = 0 ; 
	char killstr[30] ;

	status = system ("pgrep omxplayer.bin > system.dat");
	fd = open ("system.dat", O_RDWR);

	if (fd)
	{   
		len = read (fd, str, LENGTH);
	}   
	close(fd) ;

	printf("length is %d \n", len) ;           
	if(!len){
			status = system ("omxplayer -o hdmi mov7.mp4 > trash");
	}   
	return 0 ; 
}     
