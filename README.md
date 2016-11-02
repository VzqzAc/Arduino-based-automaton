# Binary to ABC.. Arduino-based automaton
A simple arduino-based automaton that converts binary input to ascii alphabet (letters and space), showing the current input on a display where also is shown if an error on the input occurs.

This project was originally made for the 'Automatons I' subject
on the Autonomous University of Aguascalientes.

The purpose of this is to convert an 8-bit digital (binary) input into ABC letters from the ascii code
and the ascii space by following the rules shown below:

##1. Only capital letters, lowercase letters, and the space are accepted by the automaton
##2. There will be accepted only one capital letter by run

![Automaton layout](/Automaton.jpg)

The first rule represent 3 special cases, the 'a', 'A', 'z', and 'Z' in the ascii table, they're meant
to represent the top and bottom of the binaries accepted by the automaton, independently of the first
three digits of the binary number, the next five digits must be between '00001' and '11010'. Also, the
space is represented by the binary number '00100000', which is also a special case.

The second rule will be verified by a flag, you can notice there are many flags in this code, but remember
that was made for academic purposes only, anyway there might be a next version more clean-coded

At the moment the code is not properly commented and it might be confused to understand.
