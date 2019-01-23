# AD9851 - DDS Module Controller Library for Arduino (written in C)
This library utilizes any Arduino pins to be used as AD9851 control pins. This will gives you flexibility to use or to reserve any Arduino pins for other functions. The drawback, the frequency update rate will be somewhat limited to the `digitalWrite()` function's clock cycle. 
This library ONLY supports the AD9851 DDS modules which have **30MHz clock**.

## Instalation
* Download as .zip
* Open Arduino IDE.
* From **Sketch**, **Include Library --> Add .ZIP Library...**
* Choose the downloaded .zip. (when downloaded, more likely be named **MyGPS-master.zip**)
* Restart the Arduino IDE

## Usage
### GPIO Definitions and Initialization
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
### Available Functions
```C
// This function will construct dds_pin structure and initialize the GPIO
// The input arguments should be the dds pins GPIO definition
// With the exact sequence: RST_pin, DATA_pin, FQ_pin, and CLK_pin respectively
// Call this function first at your setup() and store the return value into your DDS struct variable
// example: see Example senction below
struct DDS dds_init(int rst_pin, int data_pin, int fq_pin, int clk_pin);

// This function will reset the AD9851 DDS module
// The input argument should be the DDS struct variable
// You should call this function after the initialization
// and before doing any frequency writing process
void dds_reset(DDS dds);

// This function will write the desired frequency into AD9851 and generate SINE WAVE
// The 6 x Clock multiplier is always enabled (assume that you are using 30 MHz clock)
// Frequency range: DC - 90 MHz (DC - 60 MHz in practice)
// Input arguments are DDS struct variable and the desired frequency in Hertz (unsigned long)
void writeFreq(DDS dds, unsigned long freq);

// This function will generate Frequency Sweep signal
// Input arguments: - DDS struct variable
//                  - center frequency in Hertz (unsigned long)
//                  - sweep deviation in Hertz (unsigned long)
//                  - sweep step or frequency resolution in Hertz (long)
//                  - sweep delay in microseconds (unsigned int)
//
// REQUIREMENTS: - center frequency must be between DC - 90 MHz
//               - sweep deviation must be less than the twice of the center frequency value
//               - sweep step must be less than the sweep deviation divided by 65535
//               - delay in microseconds must be less than 65535 
//
// FAIL TO MEET THE REQUIREMENTS ABOVE MAY RESULTS IN ABNORMAL OR UNPREDICTABLE BEHAVIOR
void sweepTone(DDS dds, unsigned long c_freq, unsigned long s_dev, long s_step, unsigned int delay_us);
```

## Example

## Contribution
