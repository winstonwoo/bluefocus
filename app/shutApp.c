#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <syslog.h>


#define CMD_BEGIN 0xab

int main(void){

	int fd = 0xff  ;
    char val = 0xff  ;
    int work = 0x1 ;

	char killstr[30] ;
	
	fd = open("/dev/trigger", O_RDWR) ;

	if(!fd){
     printf("failed open!\n") ;
	}
	
	if(fork()== 0){
		system("cd /winston/cinema") ;
		system("./triggerApp") ;	 
		sprintf(killstr, "kill $de", getpid()) ;
		system(killstr) ;
		exit(0) ;	  
	}

	while(1){
      read(fd, &val, sizeof(char)) ;

//	  syslog(LOG_USER|LOG_INFO,"shutApp %d trigger\n", val) ;
	  switch (val){
		  case 0x4:
			  ioctl(fd, CMD_BEGIN, 0xaa) ;		
			  syslog(LOG_USER|LOG_INFO,"!!!!shutApp 0x4 trigger entere!\n") ;
			  if(work){
				  system("killall triggerApp");
				  system("killall omxplayer") ;
				  system("killall omxplayer.bin") ;
				  printf("0x4 triggered!\n") ;
			      syslog(LOG_USER|LOG_INFO,"!!!!!0x4 trigger out\n") ;
				  work = 0x0 ;
                  sleep(3) ;
			  }
			  else{
				  work = 0x1 ;	 
				  if(fork()== 0){
					  system("cd /winston/cinema") ;
					  system("./triggerApp") ;	 
	                  sprintf(killstr, "kill $de", getpid()) ;
				      system(killstr) ;
				      exit(0) ;	  
				  } 
			  }
			  break ;
		  default:
			 // syslog(LOG_USER|LOG_INFO,"shutApp\n") ;

			  break ;

	  }
	  //ioctl(fd, CMD_BEGIN, 0xaa) ;

      sleep(1) ;	
	}

	close(fd) ;

	return 0 ;

}
