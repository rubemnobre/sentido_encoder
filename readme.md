sentido_encoder
============

Digital circuit to indicate the direction of spin of a quadrature pulse encoder.

###  Motivation

This circuit was developed to reduce the need for specific quadrature pulse encoder peripherals on the Controller Board of a robotics system with a large amount of encoders. It is designed to go inline with the encoder cable, read one of the pulse channels and pass it forward, and use it's relation with the other one to drive an open collector output indicating the direction of spin.

### Architecture

The usage of a small and low cost microcontroller allows for a very simple circuit, with only a LDO to supply the 5V rail, optional pull down resistors on the A and B lines to provide adaptability for different encoder output types, and a BJT with an open collector output for the direction signal.