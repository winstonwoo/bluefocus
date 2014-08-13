1. add below row sentenses in /etc/rc.local
   
   insmod /winston/demosys/2014-08-08/module/trigger.ko
   mknod /dev/trigger c 234 0

   cd /winston/demosys/2014-08-10/app
   ./triggerApp

