#!/bin/bash
insmod /winston/cinema/trigger.ko
mknod /dev/trigger c 234 0

cd /winston/cinema
sudo ./shutApp
#sudo ./triggerApp  
