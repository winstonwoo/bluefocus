1. add below row sentenses in /etc/rc.local
   
   insmod /winston/demosys/2014-08-08/module/trigger.ko
   mknod /dev/trigger c 234 0

   cd /winston/demosys/2014-08-10/app
   ./triggerApp

2. the module folder depends on the raspberry resouce folder on PC

   CCPREFIX=/winston/raspberry/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
   KERNEL_SRC=/winston/raspberry/linux-rpi-3.2.27

3. the app can be complied on raspberry with below command
   
   gcc -g triggerApp.c -o triggerApp
4. mask the message from shell command of kill
   kill -13  $PID 	

5. Add the front ground picture display to mask the imformation of command line
   fbi -noverbose -T 2 mypic.jpg 

6. port to Raspberry B+ board with linux 3.12.22+ kernel
   key point : the linux 3.12.y should be recompiled.

7. For the B+ linux3.12.22+ the oxmplayer pid isn't followed the program pid.
   for all pid should be find and kill the oxmplayer pid correctly.
   others remain.  

8. Add the gpio_get_value in IRQ function to confirm the gpio trigger source for debounce

9. Modify the App
   pgrep omxplayer.bin
   kill omxplayer.bin
   kill getpid()

10. add GPIO25 for manual trigger

11. rebuild the app without fork function 
    Add pthread function into new app
	in each thread while(1) there should be sleep for reschedule

12. The kernel will kill user's pid if there isn't sleep() in the while(1) loop.
 
13. Add sleep(1) in app/triggerApp.c for overcoming pid killed issue.

14. Modified app_thread but there is still problems if many trigger occured at same time 
    no feedback will response for new trigger.

15. 2014-09-04 
    Now release new version for exhenbition in ZhengZhou with app
