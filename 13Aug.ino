#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

int count=0;
int HR=0;
float Spo2=0.0;
uint32_t tsLastReport = 0;
void(* resetFunc) (void) = 0;//declare reset function at address 0
// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);
    while (Serial.available() == 0) ; //wait until user presses a key
      Serial.println(Serial.read());

    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
        Serial.println("Done");
    }
    

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{    
    // Make sure to call update as fast as possible
    pox.update();
    
//    Serial.println("Now tou can place your finger");
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
    {
      HR=pox.getHeartRate();
      Spo2=pox.getSpO2();
      
      if (HR >50 && HR <120 && Spo2> 80 && Spo2 <=100 && count<5)
      { 
        count=count+1;
        Serial.print("HR:");
        Serial.println(HR);
        Serial.print("SpO2:");
        Serial.println(Spo2);
        Serial.println(count);
      }
      else 
      {
        Serial.println("Place your finger properly");
        if (count==5)
      {
     
        Serial.println("resetting arduino");
        for (int i=0 ;i<5; i++)
        {
          Serial.println("Remove your finger");
          delay(1000);
        }
      
        count=0;
        resetFunc();
      }
      }
      tsLastReport = millis();
    }   
}
