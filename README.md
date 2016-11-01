# Teensy 3.x Read TSL1402R Optical Sensor
Use a Teensy 3.x board to read the AMS TSL1402R optical sensor and plot pixel values in Processing.

Inspired by:
http://playground.arduino.cc/Main/TSL1402R

The TSL1402R linear photodiode array optical sensor:
http://ams.com/eng/Products/Light-Sensors/Linear-Array/TSL1402R

TSL1402R PDF Datasheet:
http://ams.com/chi/content/download/250165/975693/file/TSL1402R_DS000147_2-00.pdf

Mouser Electronics Link:
http://www.mouser.com/ProductDetail/ams/TSL1402R/?qs=sKasJQfA%252bi5t6%2F%2FqMqmpuA%3D%3D

Digikey Link:
https://www.digikey.com/product-detail/en/ams-taos-usa-inc/TSL1402R/TSL1402-R-ND/3095085

Arrow Electronics Link:
https://www.arrow.com/en/products/tsl1402r/ams-ag

Processing home page (Processing is used to plot the sensor data):
https://processing.org/

This includes a Teensy-centric Arduino class library for reading the TSL1402R sensor, an Teensy Arduino example, and a Processing sketch to visualize the pixel data in near-realtime.

Tested on Teensy 3.6 OK and fast! In fact, the processing app can't keep up without inserting a few milliseconds delay
in the Teensy 3.6 loop. I am seeing well over 240 frames per second (512 bytes each frame); I will measure more precisely after some experiments in speeding up the Processing sketch, with an eye towards using a C++ display solution in the future. The Processing sketch is pretty stripped-for-speed as is, but polls serial in the loop for available bytes. I want to retry
redrawing the screen from Serial event instead, last time I tried this approach it was much more sluggish, but I suspect my sync bit was being falsified by the data at the time.
 
The sensor consists of a linear array of 256 photodiodes. The sensor pixels are clocked out using "parallel mode" circuit of the sensor datasheet, and thus 2 pixels are presented for reading at a time. (After looping 128 times, we are done reading all the pixels.)

We take advantage of the Teensy ADC library to read both pixel values simultaneously using two seperate Teensy hardware ADCs, rather than reading them at seperate times, one right after the other. In theory, this approach is almost twice as fast as other typical "Parallel" mode code examples found on the web for this sensor, and almost 4 times as fast compared with reading each pixel in turn, one at a time ("Serial" mode circuit in the sensor datasheet)

What a perfect use of the "simultaneous read" feature of the Teensy ADC library! Score!
Oh, but it gets better.
We send each pixel value as a byte pair rather than character strings to shrink the bandwidth of the bitstream significantly.

Prior to sending the data over Serial, we shift the bits of each 2 byte word to the left 2 places (multiply by 4) to leave room for 255 as a unique prefix sync byte to sync the receiver to sender. 
This shift does not drop bits because the data ADC samples are only 12 bits wide. We have 4 "spare" bits in the 16 bit word. By shifting only 2 places to the left (multiply by 4), both the upper byte and lower byte of each word are prevented from ever equaling 255, so as to not interfere with the 255 sync byte.

The shift to the left 2 places (multiply by 4) prevents sending 255 in any byte, except on the sync byte.
On the Processing app, we shift the bits back 2 places (divide by 4) to restore the original byte pair word values prior to displaying them.

The library will probably work on any Teensy 3.x board, but you may need to change the pins used to connect to the sensor.





