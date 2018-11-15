/*
 *
 * File: Measure_rms.ino
 * Purpose: TrueRMS library example project
 * Version: 1.0.0
 * Date: 07-11-2018
 * URL: https://github.com/MartinStokroos/TrueRMS
 * License: MIT License
 *
 *
 * This example illustrates the measurement of the rms value of a signal at the ADC input. To test, apply a sine- or a 
 * square wave of 50Hz/60Hz with 1V amplitude, biased on 2.5Vdc to input ADC0. The dcBias reading should be stable around 
 * the value 512 decimal (the middle of the ADC range) and the rms value of the sine wave should read about 0.71V and for a 
 * square wave about 1.00V.
 * 
 * The number of samples used to capture the input signal, must be a whole number. The sample window, expressed in 
 * number of samples, must have a length equal to at least one cycle of the input signal. If this is not the case, 
 * a slow fluctuation in the rms and power readings will occure.
 * 
*/

#include <TrueRMS.h>

#define LPERIOD 1000    // loop period time in us. In this case 1.0ms
#define ADC_INPUT 0     // define the used ADC input channel
#define RMS_WINDOW 40   // rms window of 40 samples, means 2 periods @50Hz
//#define RMS_WINDOW 50   // rms window of 50 samples, means 3 periods @60Hz

unsigned long nextLoop;
int adcVal;
int cnt=0;
float VoltRange = 5.00; // ADC full scale peak-to-peak is 5.00Volts

Rms readRms ;// create an instance of


void setup() {
  // run once:
  Serial.begin(115200);
  
  readRms.begin(VoltRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  //readRms.begin(VoltRange, RMS_WINDOW, ADC_10BIT, BL_REST_OFF, SGL_SCAN);
  readRms.start();
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
  }



void loop() {
  // run repeatedly:
  adcVal = analogRead(ADC_INPUT); // Read the ADC and remove the DC-offset.
  readRms.update(adcVal);
  //readRms.update(adcVal-512); // for testing without baseline restoration. Substract a fixed DC offset in ADC-units.

  cnt++;
  if(cnt >= 500) { // publish every 0.5s
    readRms.publish();
    Serial.print(readRms.rmsVal,2);
    Serial.print(", ");
    Serial.println(readRms.dcBias);
    cnt=0;
    //readRms.start();  // Restart the acquisition after publishing if the mode is single scan.
  }

  while(nextLoop > micros());  // wait until the end of the time interval
  nextLoop += LPERIOD;  // set next loop time to current time + LOOP_PERIOD
}

// end of Measure_rms.ino
