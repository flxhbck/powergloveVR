# PowergloveVR

Resources to modify the Powerglove to become compatible with the HTC Vive VR system. Requires a Vive tracker puck and an Arduino Due. 

This project was done at Teague Labs.


## Arduino
This code is written for the Arduino Due! The program reads the analog values of the bending sensors and communicates to the HTC Vive Tracker. The code performs “auto-calibration” on the sensor values and smoothes the values over time. The communication between the Arduino Due and the Vive tracker builds on the USB-Host Library extension by [a matzmann666](https://github.com/matzman666/USBHost). Since only three analog values can be transmitted via the tracker, the middle and ring finger values are alternated over time.

## CAD
Printable and editable files for mounting the Vive tracker and Arduino Due on the Powerglove.

## Eagle
Arduino Due Shield for reading the analog bending sensors of the Powerglove. Includes battery charging module, two indicator LEDs, and a push-button. The folder also includes the image-file we used for etching.
This is a very quick and dirty design, so use it cautiously. When you look at the board design, you’ll see that some extra bridge-wires have to be soldered to the pads. The board works, but there are couple of missteps. Most importantly, the charger footprint is off.

## Unity
Example project with a rigged hand model that listens to the puck.


