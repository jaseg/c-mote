c-mote
======

Mote_ is hipster slang for small, low-power, long-lifetime sensor nodes. We want to build a sensor network consisting of about 40 motes at c-base and AfRA (both hackerspaces in Berlin). Traditionally, the term applies to wireless sensors, but since we already have enough rays bouncing around at both places we opted for less expensive, more reliable wired communication via RS-485_ instead.

Hardware
--------

We are planning the base board to be about 5*5cm. The heart of the system is an Atmel ATmega8_ microcontroller. Most of the components on the board will be surface-mounted, though we aim to not use anything smaller than 0805 packages so we can still solder it by hand. The board is supplied via 5V routed with the RS-485 bus.

All of the sensors are optional to allow greater flexibility.

Bus
---
Most likely we will end up using regular CAT5e or better ethernet cable since that stuff is cheap and will probably do the job. Contrary to what one might expect after having a glance on the standard, RS-485 (TIA-485-A) actually is kind of picky concerning bus topologies. We plan on routing a long chain, avoiding any branches, by using two of the ethernet cable's twisted pairs for both directions into dead ends if necessary.

Protocol
--------

The first iteration of the network will be based on cerebrum_, since basically everything we need is already implemented there. This network will consist of one dedicated, internet-connected bus master (some kind of linux system with an USB/RS-485 converter) running a cerebrum daemon collecting data and forwarding it to MQTT_ and Graphite_.

The medium-term goal is to get the next-gen cerebrum protocol to work which is completely distributed and directly compatible to 6LoWPAN_. At this point, it would be sensible to implement MQTT_ directly on the motes using contiki_ for its 6LoWPAN stack.

.. _Mote:       https://en.wikipedia.org/wiki/Motes
.. _RS-485:     https://en.wikipedia.org/wiki/RS-485
.. _ATmega8:    http://www.atmel.com/Images/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_summary.pdf
.. _cerebrum:   https://github.com/jaseg/cerebrum
.. _MQTT:       https://en.wikipedia.org/wiki/MQ_Telemetry_Transport
.. _Graphite:   https://github.com/graphite-project/graphite-web
.. _6LoWPAN:    https://en.wikipedia.org/wiki/6LoWPAN
.. _contiki:    http://www.contiki-os.org/
