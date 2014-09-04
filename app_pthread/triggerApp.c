#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <syslog.h>

#define CMD_BEGIN 0xab
#define LENGTH	500

//#define MY_LOG
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
char moviename[20] ;

int main(void){

	pthread_t t_control ;
	pthread_t t_play ;
    void *tret ; 

	pthread_mutex_init(&mutex, NULL) ;
	pthread_cond_init(&cond, NULL) ;


	pthread_create(&t_control, NULL, controlThread, (void*)	NULL) ;
    pthread_create(&t_play, NULL, playThread, (void*)NULL) ;


	pthread_join(t_control, &tret) ;

	pthread_mutex_destroy(&mutex) ;
	pthread_cond_destroy(&cond) ;
 
	myprintf("main exit!\n") ;

    return 0 ;
}

void *controlThread(void *junk){
    int fd ;
    int status ; 
    char val = 0 ;

	tBeginTime = times(NULL) ;
    
	status = system("fbi -noverbose -T 2 mypic.jpg") ;

	myprintf("contrl thread entered!\n") ;
    //Open trigger device	
	fd = open ("/dev/trigger", O_RDWR);                                
	if (fd < 0)                                                        
	{                                                                  
		perror ("fail to open");                                       
		exit (-1);                                                     
	}                                                                  
   
#if 1 
	//play beginning movie
	check_kill_omxplayer() ;
	pthread_mutex_lock(&mutex) ;
	strcpy(moviename, "mov2.mp4") ;     
	pthread_cond_signal(&cond) ;
	pthread_mutex_unlock(&mutex) ;
#endif
	ioctl (fd, CMD_BEGIN, 0xaa);    
	while(1){

		read(fd, &val, sizeof(char)) ; 
		switch (val){
			case 0x17:
                syslog(LOG_USER|LOG_INFO,"0x17 trigger\n") ;
				check_kill_omxplayer() ;
				insertMovie("mov4.mp4") ;

				break;
			case 0x18:

                syslog(LOG_USER|LOG_INFO,"0x18 trigger\n") ;
				check_kill_omxplayer() ;
				insertMovie("mov5.mp4") ;

				break;
			case 0x22:

                syslog(LOG_USER|LOG_INFO,"0x22 trigger\n") ;
				check_kill_omxplayer() ;
				insertMovie("mov3.mp4") ;

				break;
			case 0x23:

                syslog(LOG_USER|LOG_INFO,"0x23 trigger\n") ;
				check_kill_omxplayer() ;
				insertMovie("mov7.mp4") ;

				break;
			case 0x24:

                syslog(LOG_USER|LOG_INFO,"0x24 trigger\n") ;
				check_kill_omxplayer() ;
				insertMovie("mov4.mp4") ;

				break;
			case 0x25:
#if 0           
				status = system ("reboot");
#else
				syslog(LOG_USER|LOG_INFO,"0x25 trigger happened !\n") ;
#endif  
				break;

			default:
				//myprintf ("No trigger occur!\n");
				break ;
		}
		
		ioctl(fd, CMD_BEGIN, 0xaa) ;	

		tEndTime = times(NULL) ;
		double fCostTime = (double)(tEndTime - tBeginTime)/sysconf(_SC_CLK_TCK) ;
		if(fCostTime > 50){
			tBeginTime = tEndTime ;
			insertMovie("mov1.mp4") ;         
		} 
	
		usleep(100*1000) ;
	}

    //Close trigger device
	close (fd);                                                                                       
	return 0; 
}

void *playThread(void *junk){

	int status ;
	char pChar[200] ;	
	myprintf("player thread entered!\n") ;

	while(1){	

		pthread_mutex_lock(&mutex) ;
		syslog(LOG_USER|LOG_INFO,"playThread wait before!\n") ; 
		pthread_cond_wait(&cond, &mutex) ;

		usleep(100*1000) ;
		syslog(LOG_USER|LOG_INFO," %s player beginning\n",moviename) ; 
		sprintf(pChar, "omxplayer -o hdmi %s > trash", moviename) ;
		myprintf(pChar) ;
		status = system (pChar);
		syslog(LOG_USER|LOG_INFO," %s player complete\n",moviename) ; 
		
		pthread_mutex_unlock(&mutex) ;
		syslog(LOG_USER|LOG_INFO,"playThread unlock after!\n") ; 
	}
}



int check_kill_omxplayer(){
	int status ;
	int fd ;
	char str[LENGTH];
	int len = 0 ; 
	char killstr[30] ;
    int  pid ;

#if 0	
	myprintf("\ninsertMovie entered!\n") ;
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
#else

	syslog(LOG_USER|LOG_INFO,"check_kill_omxplayer begin!\n") ;
    status = system("killall omxplayer") ;
    status = system("killall omxplayer") ;
    status = system("killall omxplayer.bin") ;
	status = system("killall omxplayer.bin") ;
#endif	
	syslog(LOG_USER|LOG_INFO,"check_kill_omxplayer complete!\n") ;
	return 0 ; 
}

int  check_kill_pid(){

	pid_t status;
	int errno = 0;
	char cmdStr[20];
	int i;

	pidkill = 0x0;
	status = system ("pgrep omxplayer.bin > system.dat");

    analyse_pid() ;	
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


int insertMovie(char* pName){
	int status ;
	int fd ;
	char str[LENGTH];
	int len = 0 ; 
	char killstr[30] ;


	//syslog(LOG_USER|LOG_INFO,"insertMovie entered!\n") ;
	status = system ("pgrep omxplayer.bin > system.dat");
	fd = open ("system.dat", O_RDWR);

	if (fd)
	{   
		len = read (fd, str, LENGTH);
	}   
	close(fd) ;


	syslog(LOG_USER|LOG_INFO,"length is %d \n", len) ;           
	if(!len){
		pthread_mutex_lock(&mutex) ;
		syslog(LOG_USER|LOG_INFO,"insertMovie lock after!\n") ;
   		memset(moviename, 0, sizeof(moviename)) ;
		strcpy(moviename, pName) ;     
		pthread_cond_signal(&cond) ;
		pthread_mutex_unlock(&mutex) ;
        usleep(10*1000) ;     
		syslog(LOG_USER|LOG_INFO,"insertMovie mutex unlock complete!\n") ;
	}  

     
	//syslog(LOG_USER|LOG_INFO,"insertMovie exit!\n") ;
	return 0 ; 
}     
