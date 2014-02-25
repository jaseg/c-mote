BOM
===

**Note:** In the following table, the brutto prices two conrad parts have been converted to USD for summation convenience.

==  ==============================================================  ======================================  =========
#   What                                                            Sources                                 Price/ea.
==  ==============================================================  ======================================  =========
1   RS485 driver SN75176BP                                          `Tayda <tayda1_>`__                         0.39$
1   ATMega8 8-bit µC w/ nice peripheral selection and 5V operation  `Tayda <tayda2_>`__                         1.89$
1   Crystal for good bus timing                                     `Tayda <tayda3_>`__                         0.10$
1   Load caps for crystal oscillator, 22pF 0805 cer.                `Tayda <tayda4_>`__                         0.01$
1   Bootstrap resistor for photo resistor, 10k 0805                 `Tayda <tayda10_>`__                        0.01$
1   Buffer cap for entire circuit, 4.7µ electrolytic 2.54mm leads   `Tayda <tayda11_>`__                        0.02$
1   4pin terminal for bus/VCC connection                            `Conrad <conrad1_>`__                       0.73$
1   5pin terminal for contact connection (GND/VCC/IO1-3)            `Conrad <conrad2_>`__                       0.91$
1   PCB <5*5cm                                                      `Seeedstudio <seeed1_>`__                   1.00$
\   **Total**                                                                                                   5.06$
==  ==============================================================  ======================================  =========

.. _tayda1:  http://www.taydaelectronics.com/ic-integrated-circuits/rs422-rs423-rs485/sn75176-sn75176bp-75176-buffers-line-drivers-ic.html
.. _tayda2:  http://www.taydaelectronics.com/ic-integrated-circuits/microcontrollers/atmel-atmega8-16au-tqfp-32-avr-8-bit-microcontroller-ic.html
.. _tayda3:  http://www.taydaelectronics.com/crystals-resonators-oscilliators/crystals/16-000-mhz-16-mhz-crystal-hc-49-s-low-profile.html
.. _tayda4:  http://www.taydaelectronics.com/capacitors/smd-ceramic-chip-capacitors/0805/22pf-50v-smd-ceramic-chip-capacitor.html
.. _tayda10: http://www.taydaelectronics.com/resistors/smd-chip-resistors/0805/50-x-smd-chip-resistors-10k-ohm-1-8w-1-0805.html
.. _tayda11: http://www.taydaelectronics.com/capacitors/electrolytic-capacitors/4-7uf-16v-105c-radial-electrolytic-capacitor-5x11mm.html
.. _conrad1: http://www.conrad.de/ce/de/product/567658/Loetbare-Schraubklemme-mit-Liftprinzip-AKZ692-V-Rastermass-254-mm-Polzahl-4-Gruen-50692040021E-Inhalt-1-St?ref=list
.. _conrad2: http://www.conrad.de/ce/de/product/567698/Loetbare-Schraubklemme-mit-Liftprinzip-AKZ692-V-Rastermass-254-mm-Polzahl-5-Gruen-50692050021E-Inhalt-1-St?ref=list
.. _seeed1:  http://www.seeedstudio.com/service/index.php?r=site/pcbService

Remember to also throw in a couple of **termination resistors** for the system in accordance to your cable's impedance (e.g. 100Ω for ethernet cable).

Indicator option 1: RGB LED
---------------------------

==  ==============================================================  ======================================  =========
#   What                                                            Sources                                 Price/ea.
==  ==============================================================  ======================================  =========
1   RGB LED as status indicator                                     `Tayda <tayda5_>`__                         0.27$
1   Cur. limit resistor for RGB LED, 0805, 150Ω (R)                 `Tayda <tayda12_>`__                        0.01$
2   Cur. limit resistor for RGB LED, 0805, 100Ω (G/B)               `Tayda <tayda13_>`__                        0.01$
\   **Total**                                                                                                   0.30$
==  ==============================================================  ======================================  =========

.. _tayda5:  http://www.taydaelectronics.com/leds/round-leds/5mm-leds/rgb-leds/rgb-led-5mm-common-cathode.html
.. _tayda12: http://www.taydaelectronics.com/resistors/smd-chip-resistors/0805/50-x-smd-chip-resistors-100-ohm-1-8w-1-0805.html 
.. _tayda13: http://www.taydaelectronics.com/resistors/smd-chip-resistors/0805/50-x-smd-chip-resistors-150-ohm-1-8w-1-0805.html

Indicator option 2: Red 8*8 LED dot matrix (32*32mm)
----------------------------------------------------

==  ==============================================================  ======================================  =========
#   What                                                            Sources                                 Price/ea.
==  ==============================================================  ======================================  =========
1   Red 8*8 32*32mm LED dot matrix                                  `Tayda <tayda14_>`__                        1.35$
1   7-ch NPN darlington array ULN2003APG                            `Tayda <tayda15_>`__                        0.28$
2   8bit shift register w/ output latches                           `Tayda <tayda16_>`__                        0.27$
1   NPN darlington transistor 2N5306                                `Tayda <tayda17_>`__                        0.14$
1   Transistor base resistor 0805 330K                              `Tayda <tayda18_>`__                        0.01$
8   LED current limit resistors 0805 100Ω                           `Tayda <tayda19_>`__                        0.01$
0   Alternative high current shift register                         `Digikey <digikey1_>`__                     1.51$
\   **Total**                                                                                                   2.40$
==  ==============================================================  ======================================  =========

.. _tayda14:  http://www.taydaelectronics.com/led-displays/dot-matrix/8x8-dot-matrix-led-display-red-3mm-common-cathode.html
.. _tayda15:  http://www.taydaelectronics.com/ic-integrated-circuits/transistor-arrays/uln2003apg-uln2003-array-7-npn-darlingtons-ic.html
.. _tayda16:  http://www.taydaelectronics.com/ic-integrated-circuits/74-series/74hc-series/74hc595-8-bit-shift-register-ic.html
.. _tayda17:  http://www.taydaelectronics.com/t-transistors/2n-series/2n5306-npn-darlington-transistor.html
.. _tayda18:  http://www.taydaelectronics.com/resistors/smd-chip-resistors/0805/50-x-smd-chip-resistors-330k-ohm-1-8w-1-0805.html
.. _tayda19:  http://www.taydaelectronics.com/resistors/smd-chip-resistors/0805/50-x-smd-chip-resistors-100-ohm-1-8w-1-0805.html
.. _digikey1: http://www.digikey.com/product-search/en?x=0&y=0&lang=en&site=us&KeyWords=TPIC6B595

Sensors
-------

======  ==============================================================  ======================================  =========
#       What                                                            Sources                                 Price/ea.
======  ==============================================================  ======================================  =========
1       Photoresistor                                                   `Tayda <tayda6_>`__                         0.24$
0       Digital temperature sensor                                      `Tayda <tayda7_>`__                         1.75$
0.5     Alternative analog temperature sensor LM335                     `Tayda <tayda20_>`__                        0.81$
0.5     Alternative analog temperature sensor bias resistor                                                         0.01$
1       PIR (motion sensor) module                                      `Aliexpress <aliex1_>`__                    1.01$
0.3333  Optical particle sensor Sharp GP2Y1010AU0F w/ cable             `Aliexpress <aliex2_>`__                    6.40$
0       Oxygen sensor Alphasense O2-A2 (1~2yr lifetime)                 `Aliexpress <aliex3_>`__                   51.84$
0.3333  Oxygen sensor Envitec OOA101-1 (compatible; 3yr lifetime)       `Aliexpress <aliex4_>`__                    7.07$
0.3333  Passives for oxygen sensor                                                                                  0.05$
0.3333  3.5mm headphone plug for oxygen sensor                          `Tayda <tayda21_>`__                        0.16$
0.5     Humidity sensor DHT22 (**replaces temp. sensor**)               `Aliexpress <aliex5_>`__                    3.95$
\       **Total**                                                                                                   8.28$
======  ==============================================================  ======================================  =========

.. _tayda6:  http://www.taydaelectronics.com/sensors-transducer/optical-sensor/photo-conductive-cell-resistor-ldr-650nm-radial-ke-10720.html 
.. _tayda7:  http://www.taydaelectronics.com/ic-integrated-circuits/temperature-sensors/ds18b20-1-wire-digital-temperature-sensor-ic-dallas.html
.. _tayda20: http://www.taydaelectronics.com/ic-integrated-circuits/temperature-sensors/lm335-precision-temperature-sensor-40c-to-100c-ic.html
.. _tayda21: http://www.taydaelectronics.com/3-5mm-stereo-plug.html
.. _aliex1:  http://www.aliexpress.com/item/Free-Shipping-HC-SR501-Adjust-Infrared-IR-Pyroelectric-Infrared-PIR-module-Motion-Sensor-Detector-Module-We/1564561530.html
.. _aliex2:  http://www.aliexpress.com/item/2pcs-lot-GP2Y1010AU0F-100-NEW-SHARP-Optical-Dust-Sensor-GP2Y1010-free-shipping-add-cable/1670003835.html
.. _aliex3:  http://www.aliexpress.com/item/UK-Alphasense-alpha-O2-A2-oxygen-sensor-O2-A2/1613147048.html
.. _aliex4:  http://www.aliexpress.com/item/Envitec-oxygen-battery-oxygen-probe-ooa101-ooa101-1/1000408054.html
.. _aliex5:  http://www.aliexpress.com/item/10PCS-LOT-DHT22-AM2302-Digital-Temperature-and-Humidity-Sensor-DHT22-Free-shiping/1514942254.html

Loudness sensor subsystem
-------------------------
**Note:** The Op-Amp used in this subsystem is a dual type, so we need only one half of it. The other half is available for other uses.
Loudness sensor? → Microphone+amp+envelope detector

==  ==============================================================  ======================================  =========
#   What                                                            Sources                                 Price/ea.
==  ==============================================================  ======================================  =========
1   Electret microphone                                             `Tayda <tayda8_>`__                         0.25$
1   Dual Op-Amp                                                     `Tayda <tayda9_>`__                         0.11$
\*  Passives (**TODO**)                                                                                         0.05$
\   **Total**                                                                                                   0.41$
==  ==============================================================  ======================================  =========

.. _tayda8:  http://www.taydaelectronics.com/microphones/condenser-microphone-2-2k-ohm-1-5v.html
.. _tayda9:  http://www.taydaelectronics.com/ic-integrated-circuits/audio-amplifier-instrumentation-op-amp/tjm4558-dual-operational-amplifier-wide-band-soic-8-tjm4558cdt.html

Cabling
-------
We will use regular shielded ethernet cable. We will need a bunch of `splice boxes`_ to wire up cables going into dead ends.
.. _`splice boxes`: http://www.amazon.de/Logilink-LogiLink-Junction-box-metallized/dp/B001UG98NI/ref=sr_1_1?ie=UTF8&qid=1393327646&sr=8-1&keywords=cat6+junction+box

Grand total
-----------
14USD

