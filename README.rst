c-mote
======

Mote_ is hipster slang for small, low-power, long-lifetime sensor nodes. We want to build a sensor network consisting of about 40 motes at c-base and AfRA (both hackerspaces in Berlin). Traditionally, the term applies to wireless sensors, but since we already have enough rays bouncing around at both places we opted for less expensive, more reliable wired communication via RS-485_ instead.

Hardware
--------

We are planning the base board to be about 5*5cm. The heart of the system is an Atmel ATmega8_ microcontroller. Most of the components on the board will be surface-mounted, though we aim to not use anything smaller than 0805 packages so we can still solder it by hand. The board is supplied via 5V routed with the RS-485 bus.

All of the sensors are optional to allow greater flexibility.

Some of the unused microcontroller IOs are routed to a 2*5 pin male 2.54mm pin header which serves as a connector for daughter boards. The first planned daughter board is a frontend for diffuse IR telemetry.

Sensor auto-detection
~~~~~~~~~~~~~~~~~~~~~

The default firmware will include modules for all sensors. The available sensors will be probed on power-on. The following table summarizes the conditions under which each of the sensors will be considered absent.

==================  ==============================  ======================================================
Sensor              Considered absent, when...      Activate µC's internal pull-up for presence detection?
==================  ==============================  ======================================================
Analog temperature  U_in > 4.0V                     yes
Light               U_in > 4.0V                     yes
Oxygen              U_in > 4.0V                     yes
Sound               U_in > 4.0V                     yes
Methane             U_in > 4.0V                     yes
Barometer           Probing via I2C fails
Hygrometer          No response on bus
NRF24L01            Probing via SPI fails
Motion              See text below
==================  ==============================  ======================================================

The motion sensor's presence detection is a bit tricky. This sensor is considered absent when the input is a logical 0 and switches to a logical 1 after the µC's internal pull-up is activated. This procedure is using the fact that the AVR's internal pull-up is something below 50kΩ and the motion sensor's output impedance is something below 10kΩ. When the motion sensor is left unpopulated, a 200kΩ pull-down resistor is added instead.

Bus
---
Most likely we will end up using regular CAT5e or better ethernet cable since that stuff is cheap and will probably do the job. Contrary to what one might expect after having a glance on the standard, RS-485 (TIA-485-A) actually is kind of picky concerning bus topologies. We plan on routing a long chain, avoiding any branches, by using two of the ethernet cable's twisted pairs for both directions into dead ends if necessary.

Protocol
--------

The first iteration of the network will be based on cerebrum_, since basically everything we need is already implemented there. This network will consist of one dedicated, internet-connected bus master (some kind of linux system with an USB/RS-485 converter) running a cerebrum daemon collecting data and forwarding it to MQTT_ and Graphite_.

The medium-term goal is to get the next-gen cerebrum protocol to work which is completely distributed and directly compatible to 6LoWPAN_. At this point, it would be sensible to implement MQTT_ directly on the motes using contiki_ for its 6LoWPAN stack.

MAC address handling
~~~~~~~~~~~~~~~~~~~~
Cerebrum, and probably any successor to it, uses random 64-bit MAC adresses for auto-discovery. The current plan is to use a common firmware binary with sensor auto-detection on all nodes. Device-specific configuration, specifically device name, MAC and spatial coordinates should simply be stored in EEPROM. The chosen approach is to have all devices start with a build-specific, random MAC that is used until a proper, random MAC has been configured along with the device's name and coordinates. Thus, the devices can be added to the bus and configured one-by-one without requiring another compilation step.

.. _Mote:       https://en.wikipedia.org/wiki/Motes
.. _RS-485:     https://en.wikipedia.org/wiki/RS-485
.. _ATmega8:    http://www.atmel.com/Images/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_summary.pdf
.. _cerebrum:   https://github.com/jaseg/cerebrum
.. _MQTT:       https://en.wikipedia.org/wiki/MQ_Telemetry_Transport
.. _Graphite:   https://github.com/graphite-project/graphite-web
.. _6LoWPAN:    https://en.wikipedia.org/wiki/6LoWPAN
.. _contiki:    http://www.contiki-os.org/
