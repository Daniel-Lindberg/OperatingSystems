#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x53a8e63d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x93c31886, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0xa23e16b9, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x8af08e8a, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x71e3cecb, __VMLINUX_SYMBOL_STR(up) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0xf22449ae, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EBE82A32B7B2F748928B251");
