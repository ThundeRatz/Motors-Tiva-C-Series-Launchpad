# Motors-Tiva-C-Series-Launchpad
Tiva C Series board controlling motors with a serial connection to the computer.

----------------------------------------------------------------------

##About
Our latest code version uses a Tiva to control the motors board. The I2C connection
used on the ATmega was a bit unreliable.

##Features
This branch has all of the Tiva code:

- Serial listener
- ADC to check battery voltage and set the RGB LED (voltwatch)
- Motor controller
- Watchdog (failsafe)

##TivaWare
TivaWare should be placed at this directory or a link should be created. Name the
directory/link TivaWare or edit the Makefile to point to the proper location. More
information at https://github.com/ThundeRatz/trekking-magellan-docs/blob/master/Tiva.md.

#Docs
Doxygen is used to document the code. Run
```
doxygen Doxyfile
```
to generate the documentation or check 

#This is an incomplete version!
First draft. To be tested and debugged. Will be updated later this week with
discovered bugs fixed. ADC needs calibration for battery voltage divider output
(currently outputs from green to red when voltage varies from 3.3V to 0V, but
the voltage divider on the board outputs a little less than 3.3V at 25V battery
and much more than 0V at 21V). PWM frequency is a bit low and will be adjusted
to match motor's recommended frequency.

##Lost?
https://github.com/ThundeRatz/trekking-magellan-docs

##Licence
[<img src="https://i.creativecommons.org/l/by/4.0/88x31.png">](http://creativecommons.org/licenses/by/4.0/)

Except where otherwise noted, ThunderTrekking by [ThundeRatz](http://www.thunderatz.org) is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).
