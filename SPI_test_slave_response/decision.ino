
extern volatile byte command[11];

unsigned char response[11];

float timeInterval;
unsigned char int0;
unsigned char int1;

#define WAKE_REQ    0
#define WAKE_RESP   1
#define SLEEP_REQ   2
#define SLEEP_RESP  3
#define CONFIG_REQ  4
#define CONFIG_RESP 5
#define PING_REQ    6
#define PING_RESP   7
#define JUNK        0xFF

void parsePacket()
{
  switch(command[0])
  {
    case WAKE_REQ: //wake req
                   composeResponse(1);
                   break;

    case SLEEP_REQ: //sleep_req
                    composeResponse(3);
                    break;

    case CONFIG_REQ: //config req
                     composeResponse(5);
                     break;

    case PING_REQ: //ping req
                     composeResponse(7);
                     break;

    case JUNK:  //junk
                break;

    default: Serial.print("corrupt opcode");
  }
}

void composeResponse(unsigned char responseNumber)
{
  switch(responseNumber)
  {
    case WAKE_RESP: //wake resp
                    response[0] = 0x01;
                    memmove(&response[1], &timeInterval, 4);
                    response[5] = int0;
                    response[6] = int1;
                    memmove(&response[7], 0, 4);
                    break;

    case SLEEP_RESP: //sleep resp
                     response[0] = 0x03;
                     memmove(&response[1], 0, 10);
                     break;

    case CONFIG_RESP: //config resp
                      response[0] = 0x04;
                      memmove(&response[1], 0, 10);
                      break;

    case PING_RESP: //ping response
                    response[0] = 0x07;
                    memmove(&response[1], 0, 10);
                    break;

    default: break;
  } 
}

