#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/times.h>
#include <sys/wait.h>

#define CMD_BEGIN 0xab
#define LENGTH  500

//#define MY_LOG

#ifdef MY_LOG
#define myprintf printf
#else
#define myprintf
#endif

int psdata[10] =
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
int count = 0xff;
int pidkill = 0xff;		//store the pid which should be kill for omxplayer
int pidkill_b = 0x0;

clock_t tBeginTime, tEndTime ;

//count the number in one string line and convert them to int 
	int *
change (char *strg)
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
		myprintf ("!!!!!0x%x ####%d\n", number[n], n);
		i = j;
	}
	pidkill = psdata[n - 1];
	number[0] = n;
	count = n;
	myprintf ("###########pidkill = %d ++++\n", pidkill);
	return number;
}

	int
system_data (void)
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

	myprintf ("wttwtt!@ %s @wttwtt\n", str);
	myprintf ("wttwtt!@ 0x%x @wttwtt\n", str);
#if 0				//work for B+ linux 3.12.22+
	//Replace the '\r' and '\n' with ' ' for number counting. 
	i = 0;
	while (i < LENGTH)
	{
		if ((str[i] == '\n') || (str[i] == '\r'))
			str[i] = ' ';
		i++;

	}
#endif
	p = change (str);
	myprintf ("num1 = %d, %d, %d\n", num[0], num[1], num[2]);
	close (fd);
	return len;
}

	int
sub_main ()
{
	pid_t status;
	int errno = 0;
	char cmdStr[20];
	int i;

	pidkill = 0x0;
	status = system ("pgrep omxplayer.bin > system.dat");

	system_data ();
	myprintf ("wifexited(status):%d/n", WIFEXITED (status));
	myprintf ("WEXITSTATUS(status):%d/n", WEXITSTATUS (status));
	myprintf ("status = 0x%x \n", status);
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

	//only kill the last PID for whole program operating
	myprintf ("!!!!!!!!!!%d!! %d!!!\n", count, pidkill);

	if (psdata[count] != 0xff)	//if(pidkill) 
	{
		//              sprintf (cmdStr, "kill -13  %d ", pidkill);
		sprintf (cmdStr, "kill -13  %d ", psdata[count]);
		myprintf (cmdStr);
		myprintf ("$$$ths cmd %s\n", cmdStr);
		system (cmdStr);
	}
	myprintf ("^^^^^^^^^^^^^^^^^^^\n");
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
		if (fork () == 0){
			status = system ("omxplayer -o hdmi mov1.mp4 > trash");
			sprintf(killstr, "kill %d", getpid()) ;
			system(killstr) ;
			exit (0);
		}	
	}
        return 0 ;
}

	int
main ()
{

	int fd;

	char val = 0;
	int status;

	char killstr[30] ;

	tBeginTime = times(NULL) ;
	
	if (fork () == 0)
	{
		myprintf ("I'm the child process. \n");
		status = system ("fbi -noverbose -T 2 mypic.jpg ");
		printf("fbi parent ID is %d\n", getppid()) ;	
		printf("fbi ID is %d\n", getpid()) ;	
		sprintf(killstr, "kill %d", getpid()) ;
		system(killstr) ; 
		exit (0);
	}

	fd = open ("/dev/trigger", O_RDWR);

	if (fd < 0)
	{
		perror ("fail to open");
		exit (-1);
	}

	ioctl (fd, CMD_BEGIN, 0xaa);

	if (fork () == 0)
	{
		myprintf ("I'm the child process. \n");
		status = system ("omxplayer -o hdmi mov6.mp4 >trash");

		sprintf(killstr, "kill %d", getpid()) ;
		system(killstr) ; 
		printf("omplayer mov6 ID is %d\n", getpid()) ;	
		exit (0);
	}
	myprintf ("send command CMD_BEGIN with 0xaa parameter\n");

	while (1)
	{
		read (fd, &val, sizeof (char));
		myprintf ("val = %x\n", val);

		switch (val)
		{
			
			case 0x17:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm  0x17child process. \n");
					status = system ("omxplayer -o hdmi mov2.mp4 >trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ; 

					printf("omplayer mov2 ID is %d\n", getpid()) ;	
					exit (0);
				}
				break;

			case 0x18:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x18 child process. \n");
					status = system ("omxplayer -o hdmi mov3.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ; 

					printf("omplayer mov3 ID is %d\n", getpid()) ;	
					exit (0);
				}
				break;

			case 0x22:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x22 child process. \n");
					status = system ("omxplayer -o hdmi mov4.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ; 

					printf("omplayer mov4 ID is %d\n", getpid()) ;	
					exit (0);
				}
				break;

			case 0x23:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x23 child process. \n");
					status = system ("omxplayer -o hdmi mov5.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ;

					printf("omplayer mov5 ID is %d\n", getpid()) ;	
					exit (0);
				}
				break;
		
			case 0x24:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x24 child process. \n");
					status = system ("omxplayer -o hdmi mov1.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ;

					printf("omplayer mov7 ID is %d\n", getpid()) ;	
					exit (0);
				}
				break;
			
			case 0x25:
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x25 child process. \n");
					status = system ("omxplayer -o hdmi mov7.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ;

					printf("omplayer mov7 ID is %d\n", getpid()) ;	
					exit (0);
				}
	
				break;
			
			case 0x21:
					status = system ("reboot");
				break;

			case 0x4:
#if 0
				sub_main ();
				if (fork () == 0)
				{
					myprintf ("I'm 0x25 child process. \n");
					status = system ("omxplayer -o hdmi mov7.mp4 > trash");
					sprintf(killstr, "kill %d", getpid()) ;
					system(killstr) ;

					printf("omplayer mov7 ID is %d\n", getpid()) ;	
					exit (0);
				}
#else

				myprintf ("I'm 0x4 trigger process. \n");
#endif	
				break;


			default:
				myprintf ("No trigger occur!\n");

		}

		ioctl (fd, CMD_BEGIN, 0xaa);
		sleep (1);
		
		tEndTime = times(NULL) ;		
                double fCostTime = (double)(tEndTime - tBeginTime)/sysconf(_SC_CLK_TCK) ;	
                if(fCostTime > 20){                
		        tBeginTime = tEndTime ;	
			insertMovie() ;
		}
	}

	close (fd);

	return 0;
}
