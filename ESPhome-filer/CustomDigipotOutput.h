#include "esphome.h"
using namespace esphome;
#include <SPI.h>

int CS= 15;

class CustomDigipotOutput : public Component, public FloatOutput {
 public:
    void setup() override {
       // This will be called by App.setup()
       pinMode(CS, OUTPUT);
       digitalWrite(CS, HIGH);
    }

    //write to Pot
    void digitalPotWrite(byte value) {

       int r;
       SPI.begin();
       delay(50);
       digitalWrite(CS, LOW);
       delay(50);
       SPI.transfer(B00010001);
       SPI.transfer(value);
       delay(50);
       digitalWrite(CS, HIGH);

    }  

    void write_state(float state) override {
       // state is the amount this output should be on, from 0.0 to 1.0
       // we need to convert it to an integer first
       byte value = state * 255;
       ESP_LOGD("CustomDigipotOutput", "write_state %d", (int)value);
       digitalPotWrite(value);
    }
};