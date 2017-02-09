#include <stdio.h>
#include <sysexits.h>
#include <sys/stat.h>
#include "zergmap_functions.h"
#include "zerg_functions.h"

int
main(int argc, char *argv[])
{

    FILE *decodeFile;

    if (argc < 2)
    {
        printf("Please provide a file to be decoded\n");
        return EX_USAGE;
    }
    else
    {
        decodeFile = fopen(argv[1], "rb");
        if (!decodeFile)
        {
            return EX_USAGE;
        }
    }

    fseek(decodeFile, 0, SEEK_END);
    int lastPos = ftell(decodeFile);

    rewind(decodeFile);
    int start = ftell(decodeFile);

    if (lastPos == start)
    {
        printf("Your file is empty, please give me a file with pcaps\n");
        return EX_USAGE;
    }

    int fCheck;
    struct FileHeader *fh;

    fCheck = fread(&fh, sizeof(fh), 1, decodeFile);
    if (fCheck != 1)
    {
        fprintf(decodeFile,
                "Your file header was not read. Program terminated");
        return EX_USAGE;
    }

    uint32_t magicNumber = 0xa1b2c3d4;

    if (fh->fileType != magicNumber)
    {
        printf("Wrong Endianess or no pcap\n");
        return EX_USAGE;
    }


    uint32_t linkCheck = 0x1;

    if (fh->linkLayer != linkCheck)
    {
        printf("Wrong link layer\n");
        return EX_USAGE;
    }

    int padding;
    int nextPos;
    int ipVer = 0;
    bool ipv4 = false; //if false means it is ipv6;
    size_t lengthCheck;
    puts("");
    do
    {

        struct PcapHeader *ph = NULL;

        fCheck = fread(ph, sizeof(ph), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }


        struct EthernetFrame *eh = NULL;

        fCheck = fread(eh, sizeof(eh), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        struct Ipv4Header *ip = NULL;
        struct Ipv6Header *ip6 = NULL;
        ipVer = eh->type;
        if(ipVer == 8)
        {

            fCheck = fread(ip, sizeof(ip), 1, decodeFile);
             if (fCheck != 1)
             {
                break;
             }
            ipv4 = true;
        }
        else
        {
            
            fCheck = fread(ip6, sizeof(ip6), 1, decodeFile);
            if (fCheck !=1)
            {
                break;
            }
        }
        struct UdpHeader *udp = NULL;

        fCheck = fread(&udp, sizeof(udp), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        struct ZergHeader *zh = NULL;

        fCheck = fread(&zh, sizeof(zh), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        int total = ntohl(zh->version) & 0xffffff;

        if (total < 12)
        {
            printf("Packet Corrupt: Bad Zerg Size\n");
            return EX_USAGE;
        }

        //Error Checking. 
        if(ipv4)
        {
            lengthCheck = etherIpUdp + total;
        }
        else
        {
            lengthCheck = etherIp6Udp + total;
        }

        if (ph->captureLength < lengthCheck)
        {
            printf("Your file is corrupt: packet length is too short");
            return EX_USAGE;
        }

        if(ipv4)
        {
            padding = (ph->captureLength - etherIpUdp) - total;
        }
        else 
        {
            padding = (ph->captureLength - etherIp6Udp) - total;
        }

        union PayloadStructs *zerged;

        int type = zh->version & 0x0f;

        ip->version = ip->version >> 4;

        print_zerg_header(zh);

        zerged = struct_init(total, decodeFile);

        if (zerged == NULL)
        {
            return EX_USAGE;
        }

        int zerg_header = type;

        switch (zerg_header)
        {
        case (0):
            messFunction(zerged);
            break;
        case (1):
            statFunction(zerged);
            break;
        case (2):
            commFunction(zerged);
            break;
        case (3):
            gpsFunction(zerged);
            break;
        }

        fseek(decodeFile, padding, SEEK_CUR);
        nextPos = ftell(decodeFile);
        puts("");
    } while (nextPos != lastPos);

    fclose(decodeFile);
}
