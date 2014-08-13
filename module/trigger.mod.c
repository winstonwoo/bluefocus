#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc54e0a01, "module_layout" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x68d7dd81, "cdev_del" },
	{ 0xb38a8029, "cdev_add" },
	{ 0x69c77033, "cdev_init" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xfe990052, "gpio_free" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x1d2e87c6, "do_gettimeofday" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x27e1a049, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D1598B00BE5C3637D1B2DA9");
