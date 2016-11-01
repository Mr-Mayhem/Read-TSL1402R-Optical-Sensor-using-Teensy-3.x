# Teensy 3.x Read TSL1402R Optical Sensor
Use a Teensy 3.x board to read the AMS TSL1402R optical sensor and plot pixel values in Processing.

The TSL1402R linear photodiode array optical sensor:
http://ams.com/eng/Products/Light-Sensors/Linear-Array/TSL1402R

TSL1402R PDF Datasheet:
http://ams.com/chi/content/download/250165/975693/file/TSL1402R_DS000147_2-00.pdf

Processing home page (Processing is used to plot the sensor data):
https://processing.org/

This includes a Teensy-centric Arduino class library for reading the sensor.

Tested on Teensy 3.6 OK and fast! In fact, the processing app can't keep up without inserting a few milliseconds delay
in the Teensy 3.6 loop. I am seeing well over 240 frames per second (512 bytes each frame); I will measure more precisely after some experiments in speeding it up, with an eye towards using a C++ display solution in the future.
 
The sensor consists of a linear array of 256 photodiodes. The sensor pixels are clocked out using "parallel mode" circuit of the sensor datasheet, and thus 2 pixels are presented for reading at a time. (After looping 128 times, we are done reading all the pixels.)

We take advantage of the Teensy ADC library to read both pixel values simultaneously using two seperate Teensy hardware ADCs, rather than reading them at seperate times, one right after the other. In theory, this approach is almost twice as fast as other typical "Parallel" mode code examples found on the web for this sensor, and almost 4 times as fast compared with reading each pixel in turn, one at a time ("Serial" mode circuit in the sensor datasheet)

What a perfect use of the "simultaneous read" feature of the Teensy ADC library! Score!
Oh, but it gets better.
We send each pixel value as a byte pair rather than character strings to shrink the bandwidth of the bitstream significantly.

When using Serial, we shift the bits of each 2 byte word to the left 2 places (multiply by 4) to leave room for 255 as a unique prefix sync byte to sync the receiver to sender. 
This shift does not drop bits because the data ADC samples are only 12 bits wide, so we have 2 "spare" bits on each end of the 16
bit pixel value words. 
The shift to the left 2 places (multiply by 4) prevents sending 255 in any byte, except on the sync byte.
On the Processing app, we shift the bits back 2 places (divide by 4) to restore the original byte pair word values.

The library will probably work on any Teensy 3.x board, but you may need to change the pins used to connect to the sensor.





