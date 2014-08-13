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
