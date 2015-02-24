# experimental I2S development for Teensy 3.1

This is a fork of the [original library](https://github.com/hughpyle/teensy-i2s) by Hugh Pyle.
In this experimental version, basic support for 24 and 32 bit I2S half-frames (in contrast to the default 16bit) is evaluated.
**Be warned:** several existing features may be broken in the development process. Do not rely on this library.


---
Previous introduction:

The I2S protocol is designed for high-quality digital audio between devices.

[Teensy3](http://www.pjrc.com/teensy/), programmed with Arduino, uses a Kinetis K20 processor (ARM Cortex M4) with hardware support for one I2S channel.
This library can drive the I2S either directly or using DMA.   DMA is much preferred, because the CPU isn't kept busy dealing with the I/O.
It can transmit and receive simultaneously.  It should be able to drive a multi-channel codec using "DSP"-mode (not yet implemented).

The example sketches use the WM8731 stereo codec with [this library](https://github.com/hughpyle/machinesalem-arduino-libs/tree/master/WM8731).
Examples show configuration for two different boards using this codec: the [MikroE proto board](http://www.mikroe.com/add-on-boards/audio-voice/audio-codec-proto/) (48kHz, master) and for the [Open Music Labs codec shield](http://www.openmusiclabs.com/projects/codec-shield/) (44.1kHz, slave).

Current status:

* Stereo Transmit and Receive implemented with and without DMA.
* Current examples are tested with 16-bit audio and the Open Codec Labs shield.
* Receive is synchronized to Transmit, so may not actually work if Transmit is not running.  Change I2S_RCR2_SYNC(1) to I2S_RCR2_SYNC(0) to work around this.  Later I'll make it switchable.
* Patches and suggestions please!

For more status, see the [forum](http://forum.pjrc.com/threads/15748-Teensy3-I2S-with-DMA).
