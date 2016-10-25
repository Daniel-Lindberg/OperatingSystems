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
	{ 0x6b269ee7, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x7cf9b83e, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x215b60b8, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x83b32430, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x4bd8d7ca, __VMLINUX_SYMBOL_STR(alloc_etherdev_mqs) },
	{ 0x31650526, __VMLINUX_SYMBOL_STR(__netif_schedule) },
	{ 0x7f924b32, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0xf31bebb5, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x87953dd9, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xd44adc55, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x59f674af, __VMLINUX_SYMBOL_STR(consume_skb) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x7c11f175, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "11624252DA67B1A0F04BBDB");
