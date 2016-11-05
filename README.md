# Read-TSL1402R-Optical-Sensor-using-Teensy-3.x

Use a Teensy 3.x module to read the AMS TSL1402R optical sensor and plot pixel values in Processing.

Inspired by:
http://playground.arduino.cc/Main/TSL1402R

The TSL1402R linear photodiode array optical sensor (main page), also note the significant Techical Docs section:
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

This includes a Teensy-centric Arduino class library for reading the TSL1402R sensor, a Teensy 3.6 Arduino example sketch, and a Processing sketch to visualize the pixel data in near-realtime. 

===============================================================================================================================
Future improvements:
===============================================================================================================================

A lot of the code for extra features on the Processing sketch is commented out, to make it run faster. 
I will add a more full featured Processing sketch shortly, with all the bells and whistles turned on.

I will attempt writing a C++ visualizer at some point, so it can keep up with the data stream at higher speeds.

Also in the works is a Teensy 3.x SPI Master to ESP8266 SPI Slave connection, combined with ESP8266 Wifi to send the sensor data over WiFi to a Processing sketch.

===============================================================================================================================
Installation:
===============================================================================================================================
Wire the sensor to a Teensy on a breadboard, according to http://playground.arduino.cc/Main/TSL1402R and/or consult the schematics in the TSL1402R datasheet, link above. Use the parallel circuit.

Change each pin assignment in the code carefully to match your wiring and model of Teensy. Teensy 3.6 is what I set the pins to originally and thus needs no changes in the code, but the wires to the sensor chip still need to connect to the sensor correctly, of course.

In Arduino IDE, clicking File/Preferences brings up a dialog showing settings and Arduino Sketchbook Location.

Copy the library folder TSL1402R and its files directly into your Arduino Sketchbook Location/libraries folder, and restart the Arduino IDE.

Then copy the Arduino and Processing sketches into the usual places, for example:

Your Arduino Sketchbook Location/Teensy_36_TSL1402R_To_Serial/Teensy_36_TSL1402R_To_Serial.ino 
--------------------------------------------------------------------------------------------------
Your Processing Sketchbook Location/Teensy_36_TSL1402R_To_Serial/Teensy_36_TSL1402R_To_Serial.pde
--------------------------------------------------------------------------------------------------

Open the Arduino example sketch and upload it to teensy 3.x 

With the usb cable from the Teensy hooked up to your pc, Open the Processing sketch and run it. 

If it raises an error on the serial, make sure the com port is set right. It may be off by 1 or something like that. It should match the com port set in Arduino IDE under Tools >> Port when your Teensy board is selcted under Tools >> Board:.

You should see the frames counting up and the number of bytes in the serial buffer changing, and a plot of white dots that respond to changing light on the sensor.

===============================================================================================================================
Sensor Notes:
===============================================================================================================================
This is a simple and fun sensor to play with. Tested on Teensy 3.6, it works fine, and runs way, way faster than on 16 Mhz Arduino! I am seeing well over 240 frames per second (512 bytes each frame, with each frame being one complete sensor readout of all 256 pixels); I will measure more precisely after some more attempts to speed up the Processing data visualization sketch, with an eye towards using a C++ based display app in the future.

The sensor consists of a linear array of 256 photodiodes. The sensor pixels are clocked out using "parallel mode" circuit of the sensor datasheet, and thus 2 pixels are presented for reading at a time. (After looping 128 times, we are done reading all the pixels.)

You may notice 60 or 120 hertz flicker from certain light sources. 
The AMS website page for the part has a white paper that shows some ways to filter such noise, but often you can use a steady light source to eliminate the problem.

I use an overhead lamp and insulated jumper wire or rod for casting shadows upon the sensor, but be careful not to short out your circuit with bare metal objects.

Processing XY Display of Data:

Shadows cast upon the sensor window by a narrow wire show up as dips in the plot, and light from a laser line projector, like the kind used to project a laser line to align picture frames on a wall, shows up as an upwards spike in the plot. Move your desklamp around over the sensor, and watch the plot's waves roll and morph like a science fiction display! 

Some Applications:

It is used often as a filament width sensor for 3d printers. I am trying to use it to measure small displacements on a probe tip for cnc blank workpiece height correction, like for milling PCBs. They are sometimes used for reading bar codes. I have seen the TSL202R (128 pixel) used in a 3D motion tracking system, see you tube video: https://www.youtube.com/watch?v=htYAWXXoZa0

One can envision many other useful applications, like an amateur seismometer, a micrometer, a wire thickness measurement gadget, a quick drill bit size identifier, scanning line camera sensor, sensitive micro weighing or torsion balance scale sensor, robot line follower, software adjustable (instead of mechanical) cnc axis homing sensor, 3d printer bed leveling sensor etc. 

Spectrometers:

Spectrometers commonly employ this species of sensor or a linear CCD (a close cousin) to capture a light spectrum of an item by shining a diffraction grating "rainbow" over the sensor pixels, so red is at one end of the line of pixels and purple shines at the other. 

I'd use a sensor with many more pixels for building a spectrometer, because the main point of a spectrometer is to resolve the different colors as finely as possible. AMS sells a range of these linear diode photo sensors, including sensors with much higher pixel counts, that are easy to use with small changes to this code. 

More professional instruments seem to prefer sensors like a Toshiba linear CCD sensor with over 3,000 pixels. Note that these devices are significantly more challenging and complex to drive compared to the AMS sensors.

I used a low pixel count part because I seek fast-as-possible updates for my application, and the more pixels the slower the frame rate, and vice-versa.

The pixel clock goes up to 5 mhz.
Divide that by 256 pixels = 19,531.25 frames per second max, but add 18 clock cycles plus exposure delay to the math for closer estimate, which is slower. See the data sheet for the nitty gritty details on how to calculate framerate.

I am somewhere around 300ish frames per sec, limited by the Processing sketch.
There's always room for improvement. 

The fastest strategy would probably be 2 external ADCs, one per sensor analog out, with dedicated logic like a cheap CPLD for driving the sensor chip at a steady 5 Mhz pixel clock. AMS, who makes the sensor, sells a demo board that uses dedicated logic chips, but only up to 2 Mhz, see:
http://ams.com/eng/Support/Demoboards/Light-Sensors/Linear-Array/PC404A-Eval-Kit

Bit-banging timing:

I unwrapped the Arduino sensor pin bit banging code to make it faster, but using DigitalWriteFast() is too fast, apparently, because it stops working. Maybe one could use some tiny no-op type of delays with DigitalWriteFast() or use multiple calls to DigitalWriteFast(), for determining the narrowest possible driving pulse widths before it quits. The minimum pulse width is specified in the data sheet, but I have not yet formally examined the actual pulsewidths on my oscilloscope.

Exposure time:

You can set the exposure time by adjusting Sensor.ExposureMicroseconds() in the setup() of the Arduino example. 

Note that if you see the middle and far right sensor pixels lower than the rest, you may be saturating the sensor with too much exposure time, and should try turning it down. 

I witnessed this artifact in the original Arduino code cited above, and it may also be caused by the timing of the ADC read being sequential not simultaneous for the pixel pairs, or some other subtle timing issue. I also noted the original code from the Arduino example was not following the data sheet timing diagram in the sense that clock and SI pulse must be staggered in a specific sequence. The timing in the original Arduino code example worked, but the timing phase relationship between clock and SI was out of spec. I altered the clock and SI timing phase relationship to strictly follow the datasheet, and in this final version for Teensy, this artifact has all but disappeared.

I find 500 to 750 milliseconds ok for casual use under a magnifier clamping desk lamp which uses a big ring of white LEDs, a few feet over the sensor. If doing shadow casting from an led, you might turn it down even further because of the higher brightness involved. See the datasheet for recommended upper and lower exposure limits. The shorter the delay, the lower the readout for given brightness, and the faster the sensor framerate.

===============================================================================================================================
Teensy Notes:
===============================================================================================================================

We take advantage of the Teensy ADC library to read both pixel values simultaneously using two seperate Teensy hardware ADCs, rather than reading them at seperate times, one right after the other. In theory, this approach is almost twice as fast as other typical "Parallel" mode code examples found on the web for this sensor, and almost 4 times as fast compared with reading each pixel in turn, one at a time ("Serial" mode circuit in the sensor datasheet)

What a perfect use of the "simultaneous read" feature of the Teensy ADC library! Score!

Oh, but it gets better.
We send each pixel value as a byte pair rather than character strings to shrink the bandwidth of the bitstream significantly. This has the side benefit of faster frame rate and lower latency.

Prior to sending the data over Serial, we shift the bits of each 12 bit pixel value to the left 2 places (multiply by 4) to avoid value 255 in either the upper or lower byte of the sensor value- this value is reserved as the unique prefix & sync byte. 

This shifting does not drop bits because the data ADC samples are only 12 bits wide, so we have 4 "spare" bits. By shifting only 2 places to the left (multiply by 4), both the upper byte and lower byte of each word are prevented from ever equaling 255, so as to not interfere with the 255 sync byte. Yet, the final result still is 12 bits of information. Each shifted pixel value is then split into 2 bytes and sent over serial in 512 byte chunks.

===============================================================================================================================
Processing Notes:
===============================================================================================================================

Processing is used to receive the serial data from the USB, and display it. I use 

myPort = new Serial(this, "COM5", 12500000);

Note the high bits-per-second (baud) serial speed setting @ 12.5 megabits per sec.

On the Processing app, after parsing the data stream into frames using the sync byte as a delimiter, each pair of bytes is recombined into an unsigned integer identical to the original sensor pixel value, except still shifted. So we shift the bits to the right 2 places (divide by 4), to restore the original sensor values and finally display them.

The processing app can't keep up without inserting a few milliseconds delay in the Arduino main loop. Teensy 3.6 USB serial is always 12.5 megabits per second, regardless of the speed setting. 

(Plus it has a 480 megabit alternate USB connection that could be wired up, hah, but I did not try this yet.)

The serial connection seems blasing fast to me compared to Arduino, of course. The frame number is a blur.

The Processing sketch is pretty stripped-for-speed as it is, but the design polls the serial object in the loop for available bytes; not the most efficient thing one would think. 

I want to retry using noLoop() in Setup(), and redraw() the screen in serialEvent() instead, using the sync byte for the readUntil trigger.

The last time I tried this approach it was sluggish, but I suspect that during my last attempt to do it this way, my sync bit was being falsified by data bytes having the same value. Maybe it will work nice and fast this time. 

Perhaps someone out there has a more definitive answer on which drawing approach in Processing is faster and more efficient, assuming both methods are programmed properly, and given a fair comparison?

===============================================================================================================================
Processing Subpixel Resolution Notes:
===============================================================================================================================
Note: The subpixel code is experimental and temperamental. I am exploring ways to replace the current subpixel code with something better. 

I will announce here when I have better sub-pixel code working. It may be awhile, but this remains my focus 
so I can learn me some DSP.

The Processing sketch contains code to obtain subpixel resolution, borrowed from thingiverse.com filament width sensor projects:
See https://www.thingiverse.com/thing:454584 an original thingiverse project,
and https://www.thingiverse.com/thing:704897 which is a "remix" of the original thingiverse project, from which I got the subpixel code for Processing.

Subpixel Formula?

I do not fully understand the filament width subpixel code, but I do understand it finds the steepest slope on the left and right of the "notch" visible in the data plot, and then uses a formula to estimate the width using some flavor of interpolation. The original referred to it as quadradic interpolation. I am not a math wiz, so I can't tell. In response to small movements of the shadow, it moves between the original data points smoothly for a bit, then it jerks as the peak finder selects new points to anchor to, so I don't think it's working all that great, but maybe I goofed it up. 

Update: With a litle more knowlege under my belt, I am thinking that interpolation should be done first (adding well-estimated points between the original points), then look for the steepest slopes. Then the jerkeyness would be less because the steepest slope finder would be snapping between the new, more closely spaced points. 

See a brief page containing interpolation code examples here: 
http://paulbourke.net/miscellaneous/interpolation/
If you click his name at the top, it goes to his miscellaneous page, containing funny jokes scattered alongside a career's worth of interesting graphics programming tidbits. One example:

Question. Why did the computer programmer die in the shower?
Answer. The instructions on the shampoo bottle read: "Lather, Rinse, Repeat" 
lol.

The original subpixel code, from the filament width sensor projects, estimated the width of the notch, not the center as my mod attempts to do- I just divide the width by half and add that to the left side "steepest slope" position. Kind of a guess on my part there.

I draw the 2 steepest sides of the shadow's notch left and right slopes with red and green circles respectively, and the subpixel center location is marked with a white circle. Note that these additional graphical objects do not display, unless a significant, uniform, and narrow shadow is projected onto the sensor's face, AND the related code is uncommented so it runs. 

The original samples must be spread out on the screen for subpixel location to be visible, but the screen is only so big so we are rather limited without a zoom feature.

WIDTH_PER_PT sets the horizontal spacing of sensor data points on the screen. With this higher than 2, we start to see the subpixel center location resolve better. A future version could narrow the displayed region of interest, so one can zoom in on the subpixels with more screen resolution without the entire Processing display window ending up being wider than the screen. 

I commented the subpixel code out because it was slowing the framerate alot and seems to have significant jitter. Maybe I am using it wrong, or need to add the zoom-in feature to give it more screen pixels to snap to, in-between the original data points.

I wonder if someone knows of alternative subpixel resolution examples I could study, for finding the center of uniform shadows mainly, but laser line gaussian subpixel code from laser scanners would also be useful. I am thinking this species of code is most interesting and challenging to put into effect.

Side Note on Laser Vs Shadow casting:

The advantage of laser over shadow when used for a measuring device, is that a laser can amplify motion like a lever, to increase sensitivity to motion. It can be bounced a few times between mirrors prior to striking the sensor to get a longer virtual baseline thus more amplification of motion, yet still be contained in a relatively small case.

===============================================================================================================================
Final thoughts:
===============================================================================================================================

The library will probably work on any Teensy 3.x board, but you will need to change the pins used to connect to the sensor if you don't use a Teensy 3.6 like I did.

Since this is my first github posting, any feedback is welcomed, and tell me about your experiments! I'd love to see articles or even just photos with a description. Contact me through github or the Teensy forum thread on this:
https://forum.pjrc.com/threads/39058-New-library-and-example-Read-TSL1402R-Optical-Sensor-using-Teensy-3-x?p=121782#post121782

 


