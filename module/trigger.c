#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/delay.h>

#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/time.h>

#include <asm/uaccess.h>

#define DRIVER_AUTHOR "Igor <hardware.coder@gmail.com>"
#define DRIVER_DESC   "Tnterrupt Test"

// we want GPIO_17 (pin 11 on P5 pinout raspberry pi rev. 2 board)
// to generate interrupt
#define GPIO_ANY_GPIO                17   //trigger 1
#define GPIO_18_GPIO                 18   //trigger 3 
#define GPIO_22_GPIO                 22   //trigger 2 
#define GPIO_23_GPIO                 23   //trigger 4
#define GPIO_24_GPIO                 24   //trigger 5
#define GPIO_25_GPIO                 25   //trigger 6
#define GPIO_21_GPIO                 27   //Reboot /*this pin is defined as 27 in ver2board and 21 in version1 board*/
#define GPIO_4_GPIO                  4    //backup


// text below will be seen in 'cat /proc/interrupt' command
#define GPIO_ANY_GPIO_DESC           "Some gpio pin description"

// below is optional, used in more complex code, in our case, this could be
// NULL
#define GPIO_ANY_GPIO_DEVICE_DESC    "some_device"

/****************************************************************************/
#define CMD_BEGIN 0xAB

static int trigger_major = 234 ;
static int trigger_minor = 0 ;

static dev_t devno = 0 ;


static char trigger_status = 0 ; 

static struct cdev trigger_cdev ;
/****************************************************************************/
/* Interrupts variables block                                               */
/****************************************************************************/
short int irq_any_gpio    = 0;
short int irq_18_gpio     = 0 ;
short int irq_22_gpio     = 0 ;
short int irq_23_gpio    = 0;
short int irq_24_gpio    =0 ;
short int irq_25_gpio    =0 ;
short int irq_21_gpio    =0 ;
short int irq_4_gpio    =0 ;


unsigned int last_interrupt_time = 0;
unsigned int last_interrupt18_time = 0;
unsigned int last_interrupt22_time = 0;
unsigned int last_interrupt23_time = 0;
unsigned int last_interrupt24_time = 0 ;
unsigned int last_interrupt25_time = 0 ;
unsigned int last_interrupt21_time = 0 ;
unsigned int last_interrupt4_time = 0 ;


static uint64_t epochMilli;

unsigned int millis (void)
{
  struct timeval tv ;
  uint64_t now ;

  do_gettimeofday(&tv) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

  return (uint32_t)(now - epochMilli) ;
}

/****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;

   if (interrupt_time - last_interrupt_time < 500) 
  {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);
   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_ANY_GPIO) ;
   printk(KERN_NOTICE"gpio 17 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x17 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 

  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}
///****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq18_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;

   if (interrupt_time - last_interrupt18_time < 500) 
  {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt18_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);

   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_18_GPIO) ;
   printk(KERN_NOTICE"gpio 18 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x18 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq22_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;

   if (interrupt_time - last_interrupt22_time < 500) 
  {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt22_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);

   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_22_GPIO) ;
   printk(KERN_NOTICE"gpio 22 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x22 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 


   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}
//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq23_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;
   
   if (interrupt_time - last_interrupt23_time < 500) 
   {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt23_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);


   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_23_GPIO) ;
   printk(KERN_NOTICE"gpio 23 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x23 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq24_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;
   
   if (interrupt_time - last_interrupt24_time < 500) 
   {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt24_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);


   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_24_GPIO) ;
   printk(KERN_NOTICE"gpio 24 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x24 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq25_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;
   
   if (interrupt_time - last_interrupt25_time < 500) 
   {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt25_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);


   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_25_GPIO) ;
   printk(KERN_NOTICE"gpio 25 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x25 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq21_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;
   
   if (interrupt_time - last_interrupt21_time < 500) 
   {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt21_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);


   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_21_GPIO) ;
   printk(KERN_NOTICE"gpio 21 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x21 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}
//****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/***************************************************************************/
static irqreturn_t r_irq4_handler(int irq, void *dev_id, struct pt_regs *regs) {
   unsigned long flags;
   unsigned int interrupt_time = millis();
   int gpioLevel = 0xff ;
   
   if (interrupt_time - last_interrupt4_time < 500) 
   {
     printk(KERN_NOTICE "Ignored Interrupt!!!!! [%d]%s \n",
          irq, (char *) dev_id);
     return IRQ_HANDLED;
   }
   last_interrupt4_time = interrupt_time;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);


   mdelay(100) ;
   gpioLevel = gpio_get_value(GPIO_4_GPIO) ;
   printk(KERN_NOTICE"gpio 4 !!!!level = 0x%x \n", gpioLevel) ; 
   if(gpioLevel == 0x1) 
   {	   trigger_status = 0x4 ;
	   printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered & trigger_status is %x !.\n",
			   irq, (char *) dev_id, trigger_status);
   } 
  
   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

static int trigger_open(struct inode *inode, struct file* flip){

  printk(KERN_INFO "trigger_open\n") ;
  return 0 ;
}

static int trigger_release(struct inode *inode, struct file* flip){
  
  printk(KERN_INFO "trigger_release\n") ;
  return 0 ;
}

static int trigger_read(struct file* filp, char __user *buf, size_t size, loff_t *off){

  if (copy_to_user(buf, &trigger_status, sizeof(char)))
    return -EFAULT ;

  return sizeof(int) ;
  
}

static int trigger_ioctl(struct file* filp, unsigned int cmd, unsigned long arg){

  switch(cmd){
  case CMD_BEGIN:
    trigger_status = 0xff ;
    break ;
  default:
    return -EINVAL;
  }

  return 0 ;
}

/****************************************************************************/
static const struct file_operations trigger_fops ={
  .owner = THIS_MODULE ,
  .open = trigger_open ,
  .unlocked_ioctl = trigger_ioctl,
  .read = trigger_read,
  .release = trigger_release ,
};



/****************************************************************************/
/* This function configures interrupts.                                     */
/****************************************************************************/
void r_int_config(void) {

  struct timeval tv ;

  do_gettimeofday(&tv) ;
  epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000) ;


   //gpio_request functions
   if (gpio_request(GPIO_ANY_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 17 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_ANY_GPIO) ;
  

   if (gpio_request(GPIO_18_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 18 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_18_GPIO) ;

   if (gpio_request(GPIO_22_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 22 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_22_GPIO) ;

   if (gpio_request(GPIO_23_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 23 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_23_GPIO) ;
  
   if (gpio_request(GPIO_24_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 24 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_24_GPIO) ;
   
if (gpio_request(GPIO_25_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 25 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_25_GPIO) ;
   
if (gpio_request(GPIO_21_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 21 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_21_GPIO) ;
 
 if (gpio_request(GPIO_4_GPIO, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO 4 request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
   gpio_direction_input(GPIO_4_GPIO) ;
 
   //gpio_to_irq functions
   if ( (irq_any_gpio = gpio_to_irq(GPIO_ANY_GPIO)) < 0 ) {
      printk("GPIO 17 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }

   if ( (irq_18_gpio = gpio_to_irq(GPIO_18_GPIO)) < 0 ) {
      printk("GPIO 18 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }

   if ( (irq_22_gpio = gpio_to_irq(GPIO_22_GPIO)) < 0 ) {
      printk("GPIO 22  to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }

   if ( (irq_23_gpio = gpio_to_irq(GPIO_23_GPIO)) < 0 ) {
      printk("GPIO 23 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }

   if ( (irq_24_gpio = gpio_to_irq(GPIO_24_GPIO)) < 0 ) {
      printk("GPIO 24 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }

   if ( (irq_25_gpio = gpio_to_irq(GPIO_25_GPIO)) < 0 ) {
	   printk("GPIO 25 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
	   return;
   }

   if ( (irq_21_gpio = gpio_to_irq(GPIO_21_GPIO)) < 0 ) {
	   printk("GPIO 21 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
	   return;
   }

   if ( (irq_4_gpio = gpio_to_irq(GPIO_4_GPIO)) < 0 ) {
	   printk("GPIO 4 to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
	   return;
   }

   printk(KERN_NOTICE "Mapped int %d\n", irq_any_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_18_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_23_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_22_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_24_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_25_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_21_gpio);
   printk(KERN_NOTICE "Mapped int %d\n", irq_4_gpio);
   
   //request_irq
   if (request_irq(irq_any_gpio,
                   (irq_handler_t ) r_irq_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 17 Request failure\n");
      return;
   }

   if (request_irq(irq_18_gpio,
                   (irq_handler_t ) r_irq18_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 18 Request failure\n");
      return;
   }

   if (request_irq(irq_22_gpio,
                   (irq_handler_t ) r_irq22_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 22 Request failure\n");
      return;
   }

   if (request_irq(irq_23_gpio,
                   (irq_handler_t ) r_irq23_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 23 Request failure\n");
      return;
   }

   if (request_irq(irq_24_gpio,
                   (irq_handler_t ) r_irq24_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 24 Request failure\n");
      return;
   }

   if (request_irq(irq_25_gpio,
                   (irq_handler_t ) r_irq25_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 25 Request failure\n");
      return;
   }

   if (request_irq(irq_21_gpio,
                   (irq_handler_t ) r_irq21_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 21 Request failure\n");
      return;
   }

   if (request_irq(irq_4_gpio,
                   (irq_handler_t ) r_irq4_handler,
                   IRQF_TRIGGER_RISING,
                   GPIO_ANY_GPIO_DESC,
                   GPIO_ANY_GPIO_DEVICE_DESC)) {
      printk("Irq 4 Request failure\n");
      return;
   }

   return;
}


/****************************************************************************/
/* This function releases interrupts.                                       */
/****************************************************************************/
void r_int_release(void) {

   free_irq(irq_any_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_ANY_GPIO);
   
   free_irq(irq_18_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_18_GPIO);

  free_irq(irq_22_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_22_GPIO);

  free_irq(irq_23_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_23_GPIO);

  free_irq(irq_24_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_24_GPIO);

  free_irq(irq_25_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_25_GPIO);

   free_irq(irq_4_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_4_GPIO);

   free_irq(irq_21_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
   gpio_free(GPIO_21_GPIO);


  return;
}

static int char_reg_setup_cdev(void){
  int error ;
  
  devno = MKDEV(trigger_major, trigger_minor) ;
  cdev_init(&trigger_cdev, &trigger_fops) ;
  error = cdev_add(&trigger_cdev, devno, 1) ;

  if(error){
    printk(KERN_NOTICE "Error %d adding char_reg_setup_cdev", error) ;
    return error ;
  }
  
  return 0 ;
}

/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
static int __init r_init(void) {

  int result ;

  devno = MKDEV(trigger_major, trigger_minor) ;
  result = register_chrdev_region(devno, 1, "TRIGGER IRQ") ;

  if(result){
    printk(KERN_WARNING "TRIGGER IRQ FAIL TO GET MAJOR NUMBER %D\N", trigger_major) ;\
    return result;
  }
  
  char_reg_setup_cdev();
   
   printk(KERN_NOTICE "HELLO !\N");
   r_int_config();

   return 0;
}

static void __exit r_cleanup(void) {
   printk(KERN_NOTICE "GOODBYE\N");
   r_int_release();

   cdev_del(&trigger_cdev) ;
   unregister_chrdev_region(devno, 1) ;

   return;
}


module_init(r_init);
module_exit(r_cleanup);


/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
