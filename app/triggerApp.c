#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <sys/wait.h>

#define CMD_BEGIN 0xab
#define LENGTH 500

#define MY_LOG
#ifdef MY_LOG
#define myprintf printf
#else
#define myprintf
#endif

int psdata = 0xff;

int *
change (char *strg)
{
  int n, i, j;
  int *number;
  n = i = j = 0;

  while (strg[i])
    {
      for (; strg[i] && strg[i] == ' '; ++i);
      for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
      i = j;
      ++n;
    }

  number = (int *) malloc (sizeof (int) * (n + 1));
  psdata = 0xff;
  n = i = j = 0;
  while (strg[i])
    {
      for (; strg[i] && strg[i] == ' '; ++i);
      for (j = i + 1; strg[j] && strg[j] != ' '; ++j);
      number[++n] = atoi (strg + i);
      psdata = number[n];
      myprintf ("!!!!!0x%x ####%d\n", number[n], n);
      i = j;
    }
  number[0] = n;
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
  p = num;

  fd = open ("system.dat", O_RDWR);

  if (fd)
    {
      len = read (fd, str, LENGTH);
    }
  str[len] = '\0';
  myprintf ("!%s\n", str);

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

  status = system ("pgrep omxplayer > system.dat");

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

  if (psdata != 0xff)
    {
      sprintf (cmdStr, "kill -13  %d ", psdata + 1);
      myprintf (cmdStr);
      myprintf ("$$$ths cmd %s\n", cmdStr);
      system (cmdStr);
    }
}



int
main ()
{

  int fd;

  char val = 0;
  int status;

  if (fork () == 0)
    {
      myprintf ("I'm the child process. \n");
      status = system ("fbi -noverbose -T 2 mypic.jpg ");
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
      status = system ("omxplayer -o hdmi iphone.mp4 >trash");
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
	      myprintf ("I'm the child process. \n");
	      status = system ("omxplayer -o hdmi mus.mp4 >trash");
	      exit (0);
	    }
	  //status = system("omxplayer -o hdmi mus.mp4") ;  
	  break;
	case 0x18:
	  sub_main ();
	  if (fork () == 0)
	    {
	      myprintf ("I'm the child process. \n");
	      status = system ("omxplayer -o hdmi pho.mp4 > trash");
	      exit (0);
	    }
	  // status = system("omxplayer -o hdmi pho.mp4") ;  
	  break;

	case 0x22:
	  sub_main ();
	  if (fork () == 0)
	    {
	      myprintf ("I'm the child process. \n");
	      status = system ("omxplayer -o hdmi sig.mp4 > trash");
	      exit (0);
	    }
	  //status = system("omxplayer -o hdmi sig.mp4") ;  
	  break;

	case 0x23:
	  sub_main ();
	  if (fork () == 0)
	    {
	      myprintf ("I'm the child process. \n");
	      status = system ("omxplayer -o hdmi xos.mp4 > trash");
	      exit (0);
	    }
	  //status = system("omxplayer -o hdmi xos.mp4") ;  
	  break;
	default:
	  myprintf ("No trigger occur!\n");

	}
#if 0
      if (val == 0x17)
	{
	  if (fork () == 0)
	    {
	      myprintf ("I'm the child process. \n");
	      status = system ("omxplayer -o hdmi iphone.mp4");
	      close (fd);
	      exit (0);
	    }
	  else
	    {
	      myprintf ("I'm the parent. \n");
	    }
	}
#endif
      ioctl (fd, CMD_BEGIN, 0xaa);
      usleep (500);
    }

  close (fd);

  return 0;
}
