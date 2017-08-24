// Require vive tracker controller library
#include <ViveTrackerController.h>


#DEFINE DEBUG 0

// Initialize USB Host Controller
USBHost usb;

// Attach vive tracker controller to USB
ViveTrackerController tracker(usb);

int minVal[] = {1023, 1023, 1023, 1023};
int maxVal[] = {0, 0, 0, 0};
int calibratedValue[] = {0, 0, 0, 0};
int oldVal[] = {0, 0, 0, 0};

bool toggle = true;




void setup() {
  // initialize serial communication at 9600 bits per second:
  if(DEBUG) Serial.begin(115200);
  if(DEBUG) Serial.println("Program started");
  delay(200);


  pinMode(7, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);

  //init oldVals
  for (int i = 0; i < 4; i++) {
    oldVal[i] = analogRead(i);
  }

}


void loop() {
  // calibrate on button press
  if ( buttonPressed() ) {
    resetCalibration();
  }

  tracker.Task(); // Process usb and vive tracker controller tasks
  if (tracker.isConnected()) {
    digitalWrite(12, HIGH);

    // read and calibrate values
    for (int i = 0; i < 4; i++) {
      calibratedValue[i] = getCalibratedValue(smoothAnalogValue(i), i);
    }

    for (int i = 0; i < 4; i++) {
      if(DEBUG) Serial.print(calibratedValue[i]);
      if(DEBUG) Serial.print("\t");
    }
    if(DEBUG) Serial.println();

    uint8_t buttons = 0;
    int8_t trigger;
    
    if (toggle) {
      buttons = VIVETRACKER_BUTTON_GRIP;
      trigger = map(calibratedValue[2], 0, 1023,  0, 255); // middle finger
    } else {
      trigger = map(calibratedValue[3], 0, 1023,  0, 255); // ring finger
    }
    toggle = !toggle;

    int16_t padX = map(calibratedValue[1], 0, 1023,  0, 32767); // index finger
    int16_t padY = map(calibratedValue[0], 0, 1023,  0, 32767); //thumb

    uint16_t batteryLevel = 0;
    tracker.setTrackerStatus(buttons, padX, padY, trigger, batteryLevel);

  } else {
    digitalWrite(12, LOW);
  }

  delay(10); // According to HTC's documentation the interval to send data should be at least 10 ms.
}


float getVoltage(int analogValue) {
  return analogValue * (3.3 / 1023.0);
  //return analogValue;
}

bool buttonPressed() {
  return !digitalRead(7);
}

void resetCalibration() {
  digitalWrite(13, HIGH);

  //reset calibration values
  for (int i = 0; i < 4; i++) {
    minVal[i] = 1023;
    maxVal[i] = 0;
  }

  digitalWrite(13, LOW);
}

int getCalibratedValue(int val, int index) {
  if (val < minVal[index]) minVal[index] = val;
  if (val > maxVal[index]) maxVal[index] = val;
  return map(val, minVal[index], maxVal[index], 0, 1023);
  //return ((float)(val - minVal[index])) / ((float)(maxVal[index] - minVal[index]));
}

int smoothAnalogValue(int index) {
  oldVal[index] = round( ((float)oldVal[index]) * 0.5f + ((float)analogRead(index)) * 0.5f);
  return oldVal[index];
}


