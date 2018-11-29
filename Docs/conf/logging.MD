# Logging

## Logging level
The amount of logging that is done by the system is determined by a single byte, represented as an integer. This byte is called the level. Every bit determines whether a function will be logged. The level is built up as follows: 0|1|2|3|4|5|6|7, where 0 is the most significant bit. The bits correspond to the following functions:

* 0 | Global code flow
* 1 | Internal file switching
* 2 | Function calls
* 3 | Function calls including data
* 4 | Request header deconstruction process
* 5 | Header data
* 6 | Response header construction
* 7 | Reserved for later use

If we take the following level: 01100010, this will be represented as 98<sub>10</sub>
