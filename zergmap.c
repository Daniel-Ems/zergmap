#define _XOPEN_SOURCE

#include <unistd.h>
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
main (int argc, char *argv[])
{

  FILE *decodeFile;

  if (argc < 2)
    {
      printf ("Please provide a file to be decoded\n");
      return EX_USAGE;
    }

  int lowHealth;

  if (getopt (argc, argv, "h:") == 'h')
    {
      char *err;
      if (!(lowHealth = strtol (optarg, &err, 10)))
	{
	  lowHealth = 10;
	}
      if (*err)
	{
	  fprintf (stdout, "-h [numbers only]\n");
	  return EX_USAGE;
	}
    }
  else
    {
      lowHealth = 10;
    }


  struct stat *buffer = calloc (1, sizeof (*buffer));
  int i;
  off_t fileEnd = 0;

  //This loop will check to see if any of the files passed on the command
  //line are bad files
  for (i = optind; i < argc; i++)
    {
      stat (argv[i], buffer);
      if (buffer)
	{
	  fileEnd = buffer->st_size;

	  if (fileEnd == 0)
	    {
	      printf ("Empty Files not allowed\n");
	      goto cleanup1;
	    }
	}
      else
	{
	  printf ("bad file\n");
	  goto cleanup1;
	}
    }

  struct FileHeader *fh = calloc (1, sizeof (*fh));
  struct PcapHeader *ph = calloc (1, sizeof (*ph));
  struct EthernetFrame *eh = calloc (1, sizeof (*eh));
  struct Ipv4Header *ip = calloc (1, sizeof (*ip));
  struct Ipv6Header *ip6 = calloc (1, sizeof (*ip6));
  struct UdpHeader *udp = calloc (1, sizeof (*udp));
  struct ZergHeader *zh = calloc (1, sizeof (*zh));
  union PayloadStructs *zerged;

  int padding;
  int nextPos;
  int ipVer = 0;
  bool ipv4 = false;		//if false means it is ipv6;
  size_t lengthCheck;

  int files = optind;
  int fCheck;
  uint32_t magicNumber = 0xa1b2c3d4;
  vertex *zergNode = NULL;
  int zergFlag = 0;
  healthTree *tree = NULL;
  int treeFlag = 0;

  while (argv[files] && files <= argc - 1)
    {
      stat (argv[files], buffer);
      if (buffer)
	{
	  fileEnd = buffer->st_size;
	}
      decodeFile = fopen (argv[files], "rb");
      if (!decodeFile)
	{
	  files++;

	}

      fCheck = fread (fh, sizeof (*fh), 1, decodeFile);
      if (fCheck != 1)
	{
	  fprintf (decodeFile,
		   "Your file header was not read. File ignored.");
	  files++;
	  fclose (decodeFile);
	  continue;
	}

      if (fh->fileType != magicNumber)
	{
	  printf ("Wrong Endianess or no pcap, File ignored\n");
	  files++;
	  fclose (decodeFile);
	  continue;
	}

      if (fh->linkLayer != linkCheck)
	{
	  printf ("Wrong link layer\n");
	  files++;
	  fclose (decodeFile);
	  continue;
	}

      //puts("");
      do
	{

	  fCheck = fread (ph, sizeof (*ph), 1, decodeFile);
	  if (fCheck != 1)
	    {
	      break;
	    }

	  fCheck = fread (eh, sizeof (*eh), 1, decodeFile);
	  if (fCheck != 1)
	    {
	      break;
	    }

	  ipVer = eh->type;
	  if (ipVer == 8)
	    {

	      fCheck = fread (ip, sizeof (*ip), 1, decodeFile);
	      if (fCheck != 1)
		{
		  break;
		}
	      if (ip->protocol != udpProtocol)
		{
		  break;
		}
	      ipv4 = true;
	    }
	  else
	    {

	      fCheck = fread (ip6, sizeof (*ip6), 1, decodeFile);
	      if (fCheck != 1)
		{
		  break;
		}
	      if (ip6->next != udpProtocol)
		{
		  break;
		}
	    }

	  fCheck = fread (udp, sizeof (*udp), 1, decodeFile);
	  if (fCheck != 1)
	    {
	      break;
	    }
	  if (ntohs (udp->d_port) != eat)
	    {
	      break;
	    }

	  fCheck = fread (zh, sizeof (*zh), 1, decodeFile);
	  if (fCheck != 1)
	    {
	      break;
	    }

	  int total = ntohl (zh->version) & 0xffffff;

	  if (total < 12)
	    {
	      printf ("Packet Corrupt: Bad Zerg Size\n");
	      break;
	    }

	  //Error Checking. 
	  if (ipv4)
	    {
	      lengthCheck = etherIpUdp + total;
	    }
	  else
	    {
	      lengthCheck = etherIp6Udp + total;
	    }

	  if (ph->captureLength < lengthCheck)
	    {
	      printf ("Your file is corrupt: packet length is too short");
	      return EX_USAGE;
	    }

	  if (ipv4)
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

	  if (type == 1 || type == 3)
	    {
	      zerged = struct_init (total, decodeFile, type);
	    }

	  if (zerged == NULL)
	    {
	      break;
	    }

	  switch (zerg_header)
	    {
	    case (1):
	      tree = statFunction (tree, zerged, zh->dest);
	      break;
	    case (3):
	      zergNode = gpsFunction (zergNode, zerged, zh->dest);
	      break;
	    }

	  fseek (decodeFile, padding, SEEK_CUR);
	  nextPos = ftell (decodeFile);
	  //puts("");

	}
      while (nextPos != fileEnd);
      files++;
      fclose (decodeFile);
    }

  int byebye;
  int numNode;
  int maxRem;
  int removals;
  bool goodGraph = true;;
  vertex *place = zergNode;
  numNode = ListLength (zergNode);

  int graphFlag = 0;

  if (numNode == 0)
    {
      goodGraph = false;
    }
  if (goodGraph)
    {
      while (place)
	{
	  tree = insertTree (tree, place->id, MAX_HEALTH);
	  place = place->next;
	}
      maxRem = floor (numNode / 2);
      removals = removeSingle (zergNode);

      // If the # of removals exceeds the maximum number of allowed removals.
      // Then there is a straight line of connections.
      if (removals > maxRem)
	{
	  graphFlag = 1;
	  if (numNode == 3)
	    {
	      printf ("Remove Zerg #%d\n", zergNode->id);
	    }
	  else if (numNode == 4)
	    {
	      printf ("Remove Zerg #%d\n", zergNode->id);
	      printf ("Remove Zerg #%d\n", zergNode->next->id);
	    }
	  else
	    {
	      printf ("IT IS NOT POSSIBLE\n");
	      //printf("Total Removals %d\n",removals);
	    }

	}
    }

  if (graphFlag == 0)
    {
      goodGraph = sameConnections (zergNode);
      if (goodGraph)
	{
	  byebye = printRemovals (zergNode);
	  if (byebye == 0)
	    {
	      printf ("ALL ZERG ARE IN POSITION\n");
	    }
	}
      else
	{
	  printf ("UNKOWN SOLUTION\n");
	}
    }



  printf ("Low Health (%d%%):\n", lowHealth);
  if (tree == NULL)
    {
      treeFlag = 1;
    }
  else
    {
      print_tree (tree, lowHealth);
    }

  if (treeFlag == 0)
    {
      destroy_healthTree (tree);
    }
  if (zergFlag == 0)
    {
      destroy (zergNode);
    }

  free (zh);
  free (udp);
  free (ip6);
  free (ip);
  free (eh);
  free (ph);
  free (fh);
cleanup1:
  free (buffer);
}
