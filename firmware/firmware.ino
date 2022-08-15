// using this lib to solve I2C buffer problem: https://github.com/stephaneAG/zforce-arduino
// Note: uncomment "#define USE_I2C_LIB 1" in Zforce.cpp if using official SAMD21-based NeoNode board

#include <Zforce.h> // R: modded version including I2C lib or modified "MYWire" lib
#define PIN_NN_DR 7 // INT6
#define PIN_NN_RST 5


//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
    #define Serial_print(...)     Serial.print(__VA_ARGS__)
    #define Serial_println(...)   Serial.println(__VA_ARGS__)
#else
    #define Serial_print(...)
    #define Serial_println(...)
#endif


void setup() {
    pinMode(PIN_NN_RST, OUTPUT);            // reset pin
    pinMode(PIN_NN_DR, INPUT);              // data ready
    zforce.Start(PIN_NN_DR);

    digitalWrite(PIN_NN_RST, LOW);          // Reset sensor
    delay(10);
    digitalWrite(PIN_NN_RST, HIGH);

    Serial.begin(115200);
    while(!Serial){};
    Serial_println("zforce start");

    // Init / Boot complete
    Message* msg = zforce.GetMessage();
    do { msg = zforce.GetMessage(); } while (msg == NULL);
    if (msg != NULL){
        Serial_println("Received Boot Complete Notification");
        Serial_print("Message type is: ");
        Serial_println((int)msg->type);
        zforce.DestroyMessage(msg);
    }

    // Send and read Enable - mandatory
    Serial_println("sending enable ..");
    zforce.Enable(true);
    do { msg = zforce.GetMessage(); } while (msg == NULL);
    if (msg->type == MessageType::ENABLETYPE){
        Serial_print("Message type is: ");
        Serial_println((int)msg->type);
        Serial_println("Sensor is now enabled and will report touches.");
    }
    zforce.DestroyMessage(msg);
}

void loop() {

    // receives touch updates via polling
    Message* touch = zforce.GetMessage();
    if (touch != NULL){
        Serial_println("Touch Msg");
        if (touch->type == MessageType::TOUCHTYPE){
            for (uint8_t i = 0; i < ((TouchMessage*)touch)->touchCount; i++){

                Serial_print("ID is: ");
                Serial_println(((TouchMessage*)touch)->touchData[i].id);

                Serial_print("X.Y: ");
                unsigned x = ((TouchMessage*)touch)->touchData[i].x;
                unsigned y = ((TouchMessage*)touch)->touchData[i].y;

                if (x != 0 && y != 0) {
                    // TODO: add id
                    char buf[17];
                    sprintf(buf, "%04u.%04u\n", x, y);
                    Serial.print(buf);
                }

                Serial_print("Event is: ");
                Serial_println(((TouchMessage*)touch)->touchData[i].event);
            }
        }

        zforce.DestroyMessage(touch);
    }
}
