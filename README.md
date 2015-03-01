# Motors-Tiva-C-Series-Launchpad
Tiva C Series board controlling motors with a serial connection to the computer.

----------------------------------------------------------------------

##About
Our latest code version uses a Tiva to control the motors board. The I2C connection
used on the ATmega was a bit unreliable.

##Features
This branch has all of the Tiva code:

- Serial listener
- ADC to check battery voltage and set the RGB LED

##TivaWare
TivaWare should be placed at this directory or a link should be created. Name the
directory/link TivaWare or edit the Makefile to point to the proper location. More
information at https://github.com/ThundeRatz/trekking-magellan-docs/blob/master/Tiva.md.

#This is an incomplete version!
Will be updated in the near future. Currently serial commands set the blue LED
instead of changing the PWM output. Documentation will be added, too.

##Lost?
https://github.com/ThundeRatz/trekking-magellan-docs

##Licence
[<img src="https://i.creativecommons.org/l/by/4.0/88x31.png">](http://creativecommons.org/licenses/by/4.0/)

Except where otherwise noted, ThunderTrekking by [ThundeRatz](http://www.thunderatz.org) is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).
