

// message typede
#define MSG_TYPE_RECV  0x20  // control4 -> device
#define MSG_VER_CONTROL4    0x01  // control4
#define MSG_DO_CONTROL4     0x20  // DO control4

// DO command
#define DO_CMD_OFF     0x00
#define DO_CMD_ON      0x01
#define DO_CMD_TOGGLE  0x02
#define DO_CMD_DEFAULT 0xFF

// DO command
typedef struct 
{
	unsigned char chn;    // channel number
	unsigned char cmd;    // command
	int           flag;   // toggle valid flag
	unsigned short loopnum;  // delay Cycle(ms)
}S_DO_CMD;

