# experimental I2S development for Teensy 3.1

This is a fork of the [original library](https://github.com/hughpyle/teensy-i2s) by Hugh Pyle.
In this experimental version, basic support for 24 and 32 bit I2S half-frames (in contrast to the default 16bit) is evaluated.
**Be warned:** several existing features were deliberately broken during the development process.
Do not rely on this library.

**approach**:
* currently developing **RX part only**, **interrupt-based only** (no DMA!)
* I2S in 48kHz master mode for teensy 3.1 running 96Mhz
* reading stereo (2 half-channel) RX data 
* usage of pins 11, 12 and 13
* proof of concept: streaming the audio data continuously over USB serial interface without data loss - **works!**

Most notably, RX support for the InvenSense INMP441 I2S microphone is achieved.
This microphone requires 32bit samples to be taken. (Real audio content is 24bit)

When this USB "serial" stream is recorded from the first byte and saved to a file, it can be reformatted to a "real" audio file with e.g. audacity or the sox command line program.

---
Previous library introduction:

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
