#include <Zforce.h>
#define DATA_READY 2

//#define DEBUG_PRINT


void setup()
{
  Serial.begin(115200);
  while (!Serial) {};
  zforce.Start(DATA_READY);
  init_sensor();
}

void loop()
{
  Message* touch = zforce.GetMessage();
  if (touch != NULL)
  {
    if (touch->type == MessageType::TOUCHTYPE)
    {
      //for (uint8_t i = 0; i < 1; i++)
      for (uint8_t i = 0; i < ((TouchMessage*)touch)->touchCount; i++)
      {

#ifdef DEBUG_PRINT
        Serial.print(((TouchMessage*)touch)->touchData[i].id);
        Serial.print("\t");
#endif

        unsigned x = ((TouchMessage*)touch)->touchData[i].x;
        unsigned y = ((TouchMessage*)touch)->touchData[i].y;

        if (x != 0 && y != 0) {
          // TODO: add id
          char buf[17];
          sprintf(buf, "%04u.%04u\n", x, y);
          Serial.print(buf);
        }
      }
    }
    else if (touch->type == MessageType::BOOTCOMPLETETYPE)
    {
      /*If we for some reason we would receive a boot
        complete message, the sensor needs to be reinitiated.*/
      init_sensor();
    }

    zforce.DestroyMessage(touch);
  }
}


void init_sensor()
{
  Message *msg = NULL;

  // Send and read ReverseX
  zforce.ReverseX(false);

  do
  {
    msg = zforce.GetMessage();
  } while (msg == NULL);

#ifdef DEBUG_PRINT
  if (msg->type == MessageType::REVERSEXTYPE)
  {
    Serial.println("Received ReverseX Response");
    Serial.print("Message type is: ");
    Serial.println((int)msg->type);
  }
#endif

  zforce.DestroyMessage(msg);

  // Send and read ReverseY
  zforce.ReverseY(false);

  do
  {
    msg = zforce.GetMessage();
  } while (msg == NULL);

#ifdef DEBUG_PRINT
  if (msg->type == MessageType::REVERSEYTYPE)
  {
    Serial.println("Received ReverseY Response");
    Serial.print("Message type is: ");
    Serial.println((int)msg->type);
  }
#endif

  zforce.DestroyMessage(msg);

  // Send and read Touch Active Area
  zforce.TouchActiveArea(0, 0, 4000, 4000);

  do
  {
    msg = zforce.GetMessage();
  } while (msg == NULL);

#ifdef DEBUG_PRINT
  if (msg->type == MessageType::TOUCHACTIVEAREATYPE)
  {
    Serial.print("minX is: ");
    Serial.println(((TouchActiveAreaMessage *)msg)->minX);
    Serial.print("minY is: ");
    Serial.println(((TouchActiveAreaMessage *)msg)->minY);
    Serial.print("maxX is: ");
    Serial.println(((TouchActiveAreaMessage *)msg)->maxX);
    Serial.print("maxY is: ");
    Serial.println(((TouchActiveAreaMessage *)msg)->maxY);
  }
#endif

  zforce.DestroyMessage(msg);

  // Send and read Enable

  zforce.Enable(true);

  do
  {
    msg = zforce.GetMessage();
  } while (msg == NULL);

#ifdef DEBUG_PRINT
  if (msg->type == MessageType::ENABLETYPE)
  {
    Serial.print("Message type is: ");
    Serial.println((int)msg->type);
    Serial.println("Sensor is now enabled and will report touches.");
  }
#endif

  zforce.DestroyMessage(msg);
}
