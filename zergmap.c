
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#define _GNU_SOURCE
#include <sysexits.h>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>

#include "dijkstra.h"
#include "healthTree.h"
#include "zerg_list.h"
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

	struct stat *buffer = malloc (sizeof (*buffer));
	
  	int i = 1;
  	off_t fileEnd;

	 for (i = 1; i < argc; i++)
    {
      stat (argv[i], buffer);
      fileEnd = buffer->st_size;
      if (fileEnd == 0)
	{
	  printf ("Empty Files not allowed\n");
	  goto cleanup1;
	}
    }

	struct FileHeader *fh = malloc(sizeof(*fh));
	struct PcapHeader *ph = malloc(sizeof(*ph));
    struct EthernetFrame *eh = malloc(sizeof(*eh));
    struct Ipv4Header *ip = malloc(sizeof(*ip)); 
    struct Ipv6Header *ip6 = malloc(sizeof(*ip6)); 
    struct UdpHeader *udp = malloc(sizeof(*udp));
    struct ZergHeader *zh = malloc(sizeof(*zh)); 
	union PayloadStructs *zerged;

	int padding;
    int nextPos;
    int ipVer = 0;
    bool ipv4 = false; //if false means it is ipv6;
    size_t lengthCheck;

 	int files = 1;
    int fCheck;
	uint32_t magicNumber = 0xa1b2c3d4;
	vertex *zergNode = NULL;
	int zergFlag = 0;
	healthTree *tree = NULL;
	int treeFlag = 0;

   while(argv[files] && files <= argc -1)
	{
	stat (argv[files], buffer);
    fileEnd = buffer->st_size;
	decodeFile = fopen(argv[files], "rb");
    if (!decodeFile)
   	{
	  files++;
	  continue;
    }

    fCheck = fread(fh, sizeof(*fh), 1, decodeFile);
    if (fCheck != 1)
    {
        fprintf(decodeFile,
         "Your file header was not read. File ignored.");
		files++;
		fclose(decodeFile);
		continue;
    }
   
    if (fh->fileType != magicNumber)
    {
        printf("Wrong Endianess or no pcap, File ignored\n");
		files++;
		fclose(decodeFile);
        continue;
    }

    if (fh->linkLayer != linkCheck)
    {
        printf("Wrong link layer\n");
		files++;
		fclose(decodeFile);
        continue;
    }

    //puts("");
    do
    {
	    
        fCheck = fread(ph, sizeof(*ph), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        fCheck = fread(eh, sizeof(*eh), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        ipVer = eh->type;
        if(ipVer == 8)
        {
 
            fCheck = fread(ip, sizeof(*ip), 1, decodeFile);
             if (fCheck != 1)
             {
                break;
             }
            if(ip->protocol != udpProtocol)
            {
                break;
            }
            ipv4 = true;
        }
        else
        {
            	    
            fCheck = fread(ip6, sizeof(*ip6), 1, decodeFile);
            if (fCheck !=1)
            {
                break;
            }
            if(ip6->next != udpProtocol)
            {
                break;
            }
        }
         
        fCheck = fread(udp, sizeof(*udp), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }
        if(ntohs(udp->d_port) != eat)
		{
			break;
		}

        fCheck = fread(zh, sizeof(*zh), 1, decodeFile);
        if (fCheck != 1)
        {
            break;
        }

        int total = ntohl(zh->version) & 0xffffff;
		
        if (total < 12)
        {
            printf("Packet Corrupt: Bad Zerg Size\n");
            break;
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

        int type = zh->version & 0x0f;
		
        int zerg_header = type;

        ip->version = ip->version >> 4;

        //print_zerg_header(zh);
      
		if(type == 1 || type == 3)
		{
        	zerged = struct_init(total, decodeFile, type);
		}

        if (zerged == NULL)
        {
            break;
        }

        switch (zerg_header)
        {
        case (1):
            tree = statFunction(tree, zerged, zh->dest);
            break;
        case (3):
            zergNode = gpsFunction(zergNode, zerged, zh->dest);
            break;
        }

        fseek(decodeFile, padding, SEEK_CUR);
        nextPos = ftell(decodeFile);
        //puts("");

    } while (nextPos != fileEnd);
	files++;
    fclose(decodeFile);
	}

	int numNode;
	int maxRem;
	int removals;

	numNode = ListLength(zergNode);
	maxRem = floor(numNode /2);
	removals = removeSingle(zergNode);

/*	
//Still wonky, Handle Edge cases that
	if(removals > maxRem)
	{
		printf("it is not possible\n");
	}
	else if(removals == 0)
	{
		printf("no zergs have been removed\n");
	}
	else
	{
		printRemovals(zergNode);
	}
	*/
//	vertex *test;
	//setCost(zergNode);
	//test = zergNode;
	//vertex *hope = setCost(zergNode);
	//struct queue *node = malloc(sizeof(*node));
	//node = qInit(node, numNode);

	//while(test != NULL)
	//{
	//	if(test->edges == 2)
	//	{
	//		insert(node,test->id,0, 0);
	//	}
	//	test = test->next;
	//}
	//printf("queue size = %d\n", node->index+1);

	//int start;
	//int finish;
	//int next;

	//int twoWay = 2;
	//bool failed = false;

	//start = pop(node);
	//finish = pop(node);
	//vertex *findStart = findPlace(zergNode, start);
	//vertex *dijkstra = zergNode;
	/*

	struct queue *priq = malloc(sizeof(*priq));
	priq = qInit(priq, numNode);
	
	printf("start = %d\n", start);
	printf("finish = %d\n", finish);
	printf("findStart id = %d\n", findStart->id);
	
 	int total = 0;
	
	populate(priq, findStart, total);//updates priq

	findStart = findPlace(zergNode, priq->next[0].id);//takes you to next cheap node

	findStart->from = priq->next[0].from; //updates the  from in the node
	
	findStart->visited = 1; //sets node to visited

	printList(zergNode);
	*/
	if(zergNode == NULL)
	{
		printf("no gps packets given\n");
		zergFlag = 1;
	}
	else
	{
		printAdj(zergNode);
	}
	

	int lowHealth = 10;
	if(tree == NULL)
	{
		printf("no status packets given\n");
		treeFlag = 1;
	}
	else
	{
		print_tree(tree, lowHealth);
	}

	//obliviate(priq);
	//obliviate(node);
	if(treeFlag == 0)
	{
		destroy_healthTree(tree);
	}
	if(zergFlag == 0)
	{	
		destroy(zergNode);
	}
    free(zh);
    free(udp);
    free(ip6);
    free(ip);
    free(eh);
    free(ph);
    free(fh);
cleanup1:
	free(buffer);
}
