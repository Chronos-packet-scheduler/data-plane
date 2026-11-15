#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

SEC("xdp")
int xdp_quic_inspect(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data     = (void *)(long)ctx->data;

    // ethernet
    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    // ipv4
    if (eth->h_proto != bpf_htons(ETH_P_IP))
        return XDP_PASS;

    // layer 3
    struct iphdr *ip = (void *)(eth + 1);
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    // udp
    if (ip->protocol != IPPROTO_UDP)
        return XDP_PASS;

    // variable length
    int ip_hdr_len = ip->ihl * 4;
    if (ip_hdr_len < sizeof(struct iphdr))
        return XDP_PASS;

    // layer 4
    struct udphdr *udp = (void *)ip + ip_hdr_len;
    if ((void *)(udp + 1) > data_end)
        return XDP_PASS;

    if (udp->dest != bpf_htons(443) && udp->source != bpf_htons(443))
        return XDP_PASS;

    bpf_printk("UDP packet: src_port=%u dst_port=%u",
               bpf_ntohs(udp->source), bpf_ntohs(udp->dest));

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";