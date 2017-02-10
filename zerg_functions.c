#include "zerg_functions.h"
#include "zergmap_functions.h"

union PayloadStructs *
struct_init(int total, FILE * decodeFile, int type)
{
    int fCheck;
    union PayloadStructs *memory = calloc(1, sizeof(*memory) );
    if (!memory)
    {
        printf("NO MALLOC");
		return memory;
    }
	if(type == 0)
	{
		memory->mess.message = calloc(1, (total - zerg_header_length) +1);

		fCheck = fread(memory->mess.message, (total - zerg_header_length), 1
						, decodeFile);
	}
	else
	{
   		 fCheck = fread(memory, total - zerg_header_length, 1, decodeFile);
	}
    if (fCheck != 1)
    {
        memory = NULL;
    }
    return memory;
}

//Conceptually Explained by Dow
float
converter(uint32_t * thing)
{
    union converter
    {
        float speed;
        uint32_t test;
    };
    union converter speed_test;

    speed_test.test = ntohl(*thing);
    float speedy = speed_test.speed;

    return speedy;
}

void
print_zerg_header(struct ZergHeader *zerg)
{

    zerg->version = (zerg->version & 0x10) >> 4;

    printf("Version : %d\n", zerg->version);
    printf("Sequence: %d\n", ntohl(zerg->id));
    printf("From    : %d\n", ntohs(zerg->source));
    printf("To      : %d\n", ntohs(zerg->dest));
}


//compliments of DOW 
uint64_t
doub_flip(uint32_t * most, uint32_t * least)
{

    uint64_t flipped_double;

    *most = ntohl(*most);
    *least = ntohl(*least);

    flipped_double = *most;
    flipped_double = flipped_double << 32;
    flipped_double = flipped_double + *least;

    return flipped_double;
}

//Conceptually explained by Dow
double
doub_converter(uint64_t * number)
{
    union doub_converter
    {
        double place_holder;
        uint64_t old_number;
    };
    union doub_converter conversion;

    conversion.old_number = *number;
    double new_number = conversion.place_holder;

    return new_number;
}

const char *
raceId(uint32_t type)
{
    const char *raceId = "No type";

    switch (type)
    {
    case (0):
        raceId = "Overmind";
        break;
    case (1):
        raceId = "Larva";
        break;
    case (2):
        raceId = "Cerebrate";
        break;
    case (3):
        raceId = "Overlord";
        break;
    case (4):
        raceId = "Queen";
        break;
    case (5):
        raceId = "Drone";
        break;
    case (6):
        raceId = "Zergling";
        break;
    case (7):
        raceId = "Lurker";
        break;
    case (8):
        raceId = "Brooding";
        break;
    case (9):
        raceId = "Hydralisk";
        break;
    case (10):
        raceId = "Guardian";
        break;
    case (11):
        raceId = "Scourge";
        break;
    case (12):
        raceId = "Ultralisk";
        break;
    case (13):
        raceId = "Mutalisk";
        break;
    case (14):
        raceId = "Defiler";
        break;
    case (15):
        raceId = "Devourer";
        break;
    default:
        return raceId;
        break;

    }
    return raceId;
}
