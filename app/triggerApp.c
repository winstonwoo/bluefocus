#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <sys/wait.h>

#define CMD_BEGIN 0xab
#define LENGTH 30

int psdata = 0xff;

int*  change(char *strg)
{
      int n,i,j;
      int *number;
      n = i = j = 0;
      while (strg[i])
      {
            for (; strg[i] && strg[i] == ' '; ++i);
            for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
            i = j;
            ++n;
      }

      number = (int*) malloc( sizeof(int) * (n+1) );
      psdata = 0xff ;
      n = i = j = 0;
      while (strg[i])
      {
            for (; strg[i] && strg[i] == ' '; ++i);
            for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
            number[++n] = atoi(strg + i);
            psdata = number[n] ;
            printf("!!!!!0x%x ####%d\n", number[n], n );
            i = j;
      }
      number[0] = n;
      return number;
}
int system_data(void){
    int result, len;
    int fd ;
    char str[LENGTH] ;
    int num[5] ;
    int *p ;
    p = num ;

    fd = open("system.dat", O_RDWR) ;

    if(fd){
      len = read(fd, str, LENGTH) ;
    }
    str[len] = '\0' ;
    printf("!%s\n", str) ;

    p = change(str);
    printf("num1 = %d, %d, %d\n", num[0], num[1], num[2]) ;
    close(fd) ;
    return len ;
}

int sub_main()
{
    pid_t          status;
    int            errno = 0;
    char           cmdStr[20] ;

    status = system("pgrep omxplayer > system.dat");

    system_data() ;
    printf("wifexited(status):%d/n", WIFEXITED(status));
    printf("WEXITSTATUS(status):%d/n", WEXITSTATUS(status));
    printf("status = 0x%x \n", status) ;
    if (status == -1) {
        printf("system error!");
    }


    if (WIFEXITED(status)) {
        printf("cp exit normal![%d]/n", errno);
        printf("exit staus = [%X]/n", WEXITSTATUS(status));
    } else
        printf("cp exit illegal![%d]/n", errno);

    if(psdata != 0xff)
	{
    sprintf(cmdStr, "kill %d", psdata+1) ;

    printf("$$$ths cmd %s\n", cmdStr) ;
    system(cmdStr) ;
     }
}



int main(){
  
  int fd ;
  
  char val = 0 ;
  int status ;
  fd = open("/dev/trigger", O_RDWR);

  if(fd<0){
    perror("fail to open");
    exit(-1) ;
  }

  ioctl(fd, CMD_BEGIN, 0xaa);

  printf("send command CMD_BEGIN with 0xaa parameter\n") ;
 
  while(1){

    read(fd, &val, sizeof(char));
    printf("val = %x\n", val) ;

    switch(val){
    case 0x17:
         sub_main() ;
	 if(fork() == 0){
         printf("I'm the child process. \n") ;
         status = system("omxplayer -o hdmi mus.mp4") ;  
         exit(0) ;
         }
         //status = system("omxplayer -o hdmi mus.mp4") ;  
	break ;
    case 0x18:
        sub_main() ;
    	if(fork() == 0){
         printf("I'm the child process. \n") ;
         status = system("omxplayer -o hdmi pho.mp4") ;  
         exit(0) ;
         }       
        // status = system("omxplayer -o hdmi pho.mp4") ;  
	break ;

    case 0x22:
        sub_main() ;
  	if(fork() == 0){
         printf("I'm the child process. \n") ;
         status = system("omxplayer -o hdmi sig.mp4") ;  
         exit(0) ;
         } 
         //status = system("omxplayer -o hdmi sig.mp4") ;  
	break ;

    case 0x23:
        sub_main() ;	
	if(fork() == 0){
         printf("I'm the child process. \n") ;
         status = system("omxplayer -o hdmi xos.mp4") ;  
         exit(0) ;
         }
         //status = system("omxplayer -o hdmi xos.mp4") ;  
        break ;
    default:
        printf("No trigger occur!\n") ;

    }
#if 0
    if(val == 0x17){
        if(fork() == 0){
         printf("I'm the child process. \n") ;
         status = system("omxplayer -o hdmi iphone.mp4") ;  
         close(fd) ;
         exit(0) ;
         }else{
           printf("I'm the parent. \n") ;
         }
    }
#endif
    ioctl(fd, CMD_BEGIN, 0xaa);
    sleep(2) ;
  }
  
  close(fd) ;

  return 0 ;
}
