OnCoreM12-t-DelayLine
=====================

A microprocessor (PIC10F200) controlled delay line (DS1023) for the Motorola OnCore M12+t timing GPS receiver.
The PIC reads the negative sawtooth time error from the GPS and adjusts the delay in the DS1023 delay line.
The Time RAIM Status message must be enabled in the GPS-receiver (@@HnrC<CR><LF>)[1] for the circuit to function.

References
==========
[1]: M12+UsersGuide.pdf, p. 167.


Who ?
=====
Written by Jonas Bo Jalling, <jonas@jalling.dk>


License ?
=========
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <jonas@jalling.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Jonas Bo Jalling
 * ----------------------------------------------------------------------------
 */
