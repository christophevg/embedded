# Support for GPS module via USART

This example implements a GPS receiver using an EM-506 GPS module.

Buying Info: https://www.sparkfun.com/products/12751  
Product Info: http://usglobalsat.com/p-717-em-506-sirf-iv.aspx  
Datasheet: http://cdn.sparkfun.com/datasheets/GPS/EM506_um.pdf  

## Unstable

Due to a problem I can only point to, but don't understand, the code is still
unstable. When removing a typedef and declaration of such an unused variable,
the code crashes the mcu. They are marked in the nmea.{c,h} code.
