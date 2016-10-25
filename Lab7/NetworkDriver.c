//Daniel Lindberg
//In Collaboration with Alex Davis
//11/30/2015

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/etherdevice.h>

struct os_priv {
    struct net_device_stats stats;
    struct sk_buff *skb;
    struct os_packet *pkt;
    struct net_device *dev;
};

struct os_packet {
    struct net_device *dev;
    int datalen;
    u8 data[ETH_DATA_LEN];
};

MODULE_LICENSE("GPL");

struct net_device *os0;
struct net_device *os1;

struct os_priv *priv0;
struct os_priv *priv1;


int os_open(struct net_device *dev) {
    netif_start_queue(dev);
    return 0;
}

int os_stop(struct net_device *dev) {
    netif_stop_queue(dev);
    return 0;
}

int os_rx_i_handler(struct net_device *dev) { return 0; }
int os_tx_i_handler(struct net_device *dev) { return 0; }

int os_start_xmit(struct sk_buff *skb, struct net_device *dev) {
    struct os_priv *priv;
    priv = (dev == os0) ? priv0 : priv1;

    char *data = skb->data;
    int len = skb->len;
    priv->skb = skb;

    // swappabadadooo
    struct iphdr *ih = (struct iphdr *)(data + sizeof(struct ethhdr));
    u32 *saddr = &ih->saddr;
    u32 *daddr = &ih->daddr;
    ((u8*)saddr)[2] ^= 1;
    ((u8*)daddr)[2] ^= 1;
    ih->check = 0;
    ih->check = ip_fast_csum((unsigned char *)ih, ih->ihl);
    priv->pkt->datalen = len;
    memcpy(priv->pkt->data, data, len);
    os_rx_i_handler(dev);
    os_tx_i_handler(dev);
    priv = netdev_priv(skb->dev);
    dev_kfree_skb(priv->skb);
    skb = dev_alloc_skb(len);
    memcpy(skb_put(skb, len), data, len);
    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);
    netif_rx(skb);
    if (netif_queue_stopped(priv->pkt->dev)) netif_wake_queue(priv->pkt->dev);
    priv = netdev_priv(dev);
    if (netif_queue_stopped(priv->pkt->dev)) netif_wake_queue(priv->pkt->dev);

    return 0;
}

struct net_device_stats *os_stats(struct net_device *dev) {
    return &(((struct os_priv*)netdev_priv(dev))->stats);
}


int os_header(struct sk_buff *skb, struct net_device *dev,
              unsigned short type, const void *daddr, const void *saddr,
              unsigned int len) {
    struct ethhdr *eth = (struct ethhdr*)skb_push(skb, ETH_HLEN);

    memcpy(eth->h_source, dev->dev_addr, dev->addr_len);
    memcpy(eth->h_dest, eth->h_source, dev->addr_len);
    eth->h_dest[ETH_ALEN-1] = (eth->h_dest[ETH_ALEN-1] == 5) ? 6 : 5;

    eth->h_proto = htons(type);

    return dev->hard_header_len;
}


static const struct net_device_ops os_device_ops = {
    .ndo_open = os_open,
    .ndo_stop = os_stop,
    .ndo_start_xmit = os_start_xmit,
    .ndo_get_stats = os_stats
};

static const struct header_ops os_header_ops = {
    .create = os_header
};

int init_module(void) {
    int i;

    os0 = alloc_etherdev(sizeof(struct os_priv));
    os1 = alloc_etherdev(sizeof(struct os_priv));

    for(i = 0; i < 6; i++) {
        os0->dev_addr[i] = (unsigned char)i;   
        os1->dev_addr[i] = (unsigned char)i;  
        os0->broadcast[i] = (unsigned char)15; 
        os1->broadcast[i] = (unsigned char)15; 
    }
    os0->hard_header_len = 14;
    os1->hard_header_len = 14;
    os1->dev_addr[5]++; 

    memcpy(os0->name, "os0\0", 4);
    memcpy(os1->name, "os1\0", 4);

    os0->netdev_ops = &os_device_ops;
    os1->netdev_ops = &os_device_ops;

    os0->header_ops = &os_header_ops;
    os1->header_ops = &os_header_ops;

    os0->flags |= IFF_NOARP;
    os1->flags |= IFF_NOARP;

    priv0 = netdev_priv(os0);
    priv1 = netdev_priv(os1);
    memset(priv0, 0, sizeof(struct os_priv));
    memset(priv1, 0, sizeof(struct os_priv));
    priv0->pkt = kmalloc(sizeof(struct os_packet), GFP_KERNEL);
    priv1->pkt = kmalloc(sizeof(struct os_packet), GFP_KERNEL);
    priv0->pkt->dev = os0;
    priv1->pkt->dev = os1;

    register_netdev(os0);
    register_netdev(os1);

    return 0;
}

void cleanup_module(void) {
    if(os0) { 
        kfree(priv0->pkt);
        unregister_netdev(os0);
    }

    if(os1) {
        kfree(priv1->pkt);
        unregister_netdev(os1);
    }
    return;
}
