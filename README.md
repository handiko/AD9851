# AD9851 - DDS Module Controller Library for Arduino (written in C)
This library utilizes any Arduino pins to be used as AD9851 control pins. This will gives you flexibility to use or to reserve any Arduino pins for other functions. The drawback, the frequency update rate will be somewhat limited to the `digitalWrite()` function's clock cycle.

## Instalation
* Download as .zip
* Open Arduino IDE.
* From **Sketch**, **Include Library --> Add .ZIP Library...**
* Choose the downloaded .zip. (when downloaded, more likely be named **MyGPS-master.zip**)
* Restart the Arduino IDE

## Usage
```C
// start off by including the library
#include <AD9851.h>

// define the GPIO pins to be used
// you can use any pins you like
#define RST 8
#define DATA 9
#define FQ  10
#define CLK 11

// Declare a DDS struct variable, globally or locally, depends on the scope
DDS dds;

// somewhere in your setup() function
// call the dds_init() function to construct dds_pin structure 
// and initialize the GPIO for you
dds = dds_init(RST, DATA, FQ, CLK);

// now you are ready to go!!
```

## Example

## Contribution
