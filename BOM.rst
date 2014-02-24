BOM
===

==  ==============================================================  =====================
#   What                                                            Sources
==  ==============================================================  =====================
1   RS485 driver                                                    Tayda__
__  http://www.taydaelectronics.com/ic-integrated-circuits/rs422-rs423-rs485/sn75176-sn75176bp-75176-buffers-line-drivers-ic.html
1   ATMega8 8-bit µC w/ nice peripheral selection and 5V operation  Tayda__
__  http://www.taydaelectronics.com/ic-integrated-circuits/microcontrollers/atmel-atmega8-16au-tqfp-32-avr-8-bit-microcontroller-ic.html
1   Crystal for good bus timing                                     Tayda__
__  http://www.taydaelectronics.com/crystals-resonators-oscilliators/crystals/16-000-mhz-16-mhz-crystal-hc-49-s-low-profile.html
1   Load caps for crystal oscillator, 22pF 0805 cer.                Tayda__
__  http://www.taydaelectronics.com/capacitors/smd-ceramic-chip-capacitors/0805/22pf-50v-smd-ceramic-chip-capacitor.html
1   RGB LED as status indicator                                     Tayda__
__  http://www.taydaelectronics.com/leds/round-leds/5mm-leds/rgb-leds/rgb-led-5mm-common-cathode.html
1   Bootstrap resistor for photo resistor
1   Buffer cap for entire circuit
1   4pin terminal for bus/VCC connection
1   5pin terminal for contact connection (GND/VCC/IO1-3)
3   Current limiting resistors for RGB LED
1   PCB <5*5cm                  Seeedstudio__
__  http://www.seeedstudio.com/service/index.php?r=site/pcbService
==  ==========================  ========================================

Sensors
-------
==  ==========================  ========================================
#   What                        Sources
==  ==========================  ========================================
1   Photoresistor               Tayda__
__  http://www.taydaelectronics.com/sensors-transducer/optical-sensor/photo-conductive-cell-resistor-ldr-650nm-radial-ke-10720.html Photoresistor
1   Digital temperature sensor  Tayda__
__  http://www.taydaelectronics.com/ic-integrated-circuits/temperature-sensors/ds18b20-1-wire-digital-temperature-sensor-ic-dallas.html
1   PIR (motion sensor) module  Aliexpress__
__  http://www.aliexpress.com/item/Free-Shipping-HC-SR501-Adjust-Infrared-IR-Pyroelectric-Infrared-PIR-module-Motion-Sensor-Detector-Module-We/1564561530.html
==  ==========================  ========================================

Loudness sensor subsystem
-------------------------
*Note:* The Op-Amp used in this subsystem is a dual type, so we need only one half of it. The other half is available for other uses.
Loudness sensor? → Microphone+amp+envelope detector
==  ==========================  ========================================
#   What                        Sources
==  ==========================  ========================================
1   Electret microphone         Tayda__
__  http://www.taydaelectronics.com/microphones/condenser-microphone-2-2k-ohm-1-5v.html
1   Dual Op-Amp                 Tayda__
__  http://www.taydaelectronics.com/ic-integrated-circuits/audio-amplifier-instrumentation-op-amp/tjm4558-dual-operational-amplifier-wide-band-soic-8-tjm4558cdt.html
==  ==========================  ========================================

