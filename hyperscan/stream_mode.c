#include <stdio.h>
#include <hs.h>
#include <pcap.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>


static int onMatch(unsigned int id, unsigned long long from,
                                unsigned long long to, unsigned int flags, void *ctx) {
    int *matchcount = (int *)ctx;
    printf("id : %d, ", id);
    printf("from : %llu, ", from);
    printf("to : %llu, ", to);
    printf("ctx : %d\n", (*matchcount));
    printf("--------\n");
    (*matchcount)++;
    return 0;
}

static int onMatchClose(unsigned int id, unsigned long long from,
                                unsigned long long to, unsigned int flags, void *ctx) {
    return 0;
}

int getStreamId(struct ip *iphdr)
{
    int protocol = iphdr->ip_p;
    int srcAddr = iphdr->ip_src.s_addr;
    int dstAddr = iphdr->ip_dst.s_addr;

    const struct udphdr *uh =
        (const struct udphdr *)(((const char *)iphdr) + (iphdr->ip_hl * 4));
    int srcPort = uh->source;
    int dstPort = uh->dest;
}

int fiveTupleHash(const struct ip *iphdr)
{
    int protocol = iphdr->ip_p;
    int srcAddr = iphdr->ip_src.s_addr;
    int dstAddr = iphdr->ip_dst.s_addr;

    const struct udphdr *uh =
        (const struct udphdr *)(((const char *)iphdr) + (iphdr->ip_hl * 4));
    int srcPort = uh->source;
    int dstPort = uh->dest;
    unsigned long long hash1 = srcAddr ^ dstAddr ^ srcPort ^ dstPort ^ protocol;
    unsigned int hash = hash1 % 65535;
    return hash;
}

static int payloadOffset(const unsigned char *pkt_data, unsigned int *offset,
                    unsigned int *length) {
    const struct ip *iph = (const struct ip*)(pkt_data + sizeof(struct ether_header));
    const struct tcphdr *th = NULL;

    if (iph->ip_v != 4) {
        return -1;
    }

    if (iph->ip_off & htons(IP_MF|IP_OFFMASK)) {
        return -1;
    }

    unsigned int ihlen = iph->ip_hl * 4;
    unsigned int thlen = 0;

    switch (iph->ip_p) {
        case IPPROTO_TCP:
            th = (const struct tcphdr *)((const char *)iph + ihlen);
            thlen = th->doff * 4;
            break;
        case IPPROTO_UDP:
            thlen = sizeof(struct udphdr);
            break;
        default:
            return -1;
    }

    *offset = sizeof(struct ether_header) + ihlen + thlen;
    *length = sizeof(struct ether_header) + ntohs(iph->ip_len) - *offset;

    return *length != 0;
}

struct streams {
    int id;
    hs_stream_t *stream;
};

int main(void)
{
    const char *pt[3] = {0};
    pt[0] = (char *)malloc(3);
    memcpy((char *)pt[0], "111", 3);

    pt[1] = (char *)malloc(3);
    memcpy((char *)pt[1], "222", 3);

    pt[2] = (char *)malloc(3);
    memcpy((char *)pt[2], "555", 3);

    unsigned int ids[3] = {110, 123, 890};
    unsigned int flags[3] = {0, 0, 0};


    flags[0] |= HS_FLAG_SOM_LEFTMOST;
    flags[1] |= HS_FLAG_SOM_LEFTMOST;
    flags[2] |= HS_FLAG_SOM_LEFTMOST;

    flags[1] |= HS_FLAG_CASELESS;
    flags[2] |= HS_FLAG_CASELESS;

    
    hs_database_t *database;
    hs_compile_error_t *compile_err;

    //hs_error_t err = hs_compile_multi(pt, flags, ids, 3, HS_MODE_STREAM, NULL, &database, &compile_err);
    hs_error_t err = hs_compile_multi(pt, flags, ids, 3, HS_MODE_STREAM|HS_MODE_SOM_HORIZON_LARGE, NULL, &database, &compile_err);
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: compile %s : %s\n", pt[compile_err->expression], compile_err->message);
        hs_free_compile_error(compile_err);
    }

    free((char *)pt[0]); pt[0] = NULL;
    free((char *)pt[1]); pt[1] = NULL;
    free((char *)pt[2]); pt[2] = NULL;

    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(database);
        return -1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcapHandle = pcap_open_offline("./test.pcap", errbuf);
    if (pcapHandle == NULL) {
        fprintf(stderr, "Error: Unable open pcap file : %s\n", "./test.pcap");
        return -1;
    }

    struct pcap_pkthdr pktHeader;
    const unsigned char *pktData;
    int matchCount = 0;

    struct streams ft[65535] = {0};
    
    while ((pktData = pcap_next(pcapHandle, &pktHeader)) != NULL) {
        unsigned int offset = 0, length = 0;

        if (!payloadOffset(pktData, &offset, &length)) {
            continue;
        }

        const struct ip *iphdr = (const struct ip *)(pktData + sizeof(struct ether_header));
        const char *payload = (const char *)pktData + offset;

        unsigned int index = fiveTupleHash(iphdr);
        if (ft[index].id == 0) {
            hs_stream_t *stream;
            hs_error_t err = hs_open_stream(database, 0, &stream);
            if (err != HS_SUCCESS) {
                fprintf(stderr, "error: open stream\n");
                return -1;
            }
            ft[index].id = 1;
            ft[index].stream = stream;
        }

        err = hs_scan_stream(ft[index].stream, payload, length, 0, scratch, onMatch, &matchCount);
        if (err != HS_SUCCESS) {
            fprintf(stderr, "error: scan stream\n");
            return -1;
        }
    }

    int i = 0;
    for (i = 0; i < 65535; i++) {
        if (ft[i].id != 0) {
            err = hs_close_stream(ft[i].stream, scratch, onMatchClose, NULL);
            if (err != HS_SUCCESS) {
                fprintf(stderr, "error: scan stream\n");
                return -1;
            }
        }
    }
    printf("matchcount : %d\n", matchCount);
    hs_free_scratch(scratch);
    hs_free_database(database);

    return 0;
}
