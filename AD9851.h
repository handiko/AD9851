#ifndef AD9851_h
#define AD9851_h

#include <limits.h>

struct DDS {
  int rst;
  int data;
  int fq;
  int clk;
};

struct DDS dds_init(int rst_pin, int data_pin, int fq_pin, int clk_pin);
void pulse(char pin);
void dds_reset(DDS dds);
unsigned long calcDataWord(unsigned long in_freq);
void writeFreq(DDS dds, unsigned long freq);
void sweepUp(DDS dds, unsigned long c_freq, unsigned long s_dev, unsigned long s_step, unsigned int delay_us);
void sweepDn(DDS dds, unsigned long c_freq, unsigned long s_dev, unsigned long s_step, unsigned int delay_us);
void sweepTone(DDS dds, unsigned long c_freq, unsigned long s_dev, long s_step, unsigned int delay_us);

struct DDS dds_init(int rst_pin, int data_pin, int fq_pin, int clk_pin)
{
  DDS dds_pin;

  dds_pin.rst = rst_pin;
  dds_pin.data = data_pin;
  dds_pin.fq = fq_pin;
  dds_pin.clk = clk_pin;

  pinMode(rst_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(fq_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  
  return dds_pin;
}

static void pulse(char pin)
{
	digitalWrite(pin, HIGH); 
	digitalWrite(pin, LOW);
}

void dds_reset(DDS dds)
{
	digitalWrite(dds.data, LOW);
	digitalWrite(dds.fq, LOW);
	digitalWrite(dds.clk, LOW);

	pulse(dds.rst); 
	pulse(dds.clk); 
	pulse(dds.fq); 
}

static unsigned long calcDataWord(unsigned long in_freq)
{
	unsigned long result = 0;

	for(char i=0; i<32; i++)
	{
		result = result << 1;
		in_freq = in_freq << 1;

		if(in_freq >= 180000000UL)
		{
			in_freq = in_freq - 180000000UL;
			result++;
		}
	}

	return result;
}

void writeFreq(DDS dds, unsigned long in_freq)
{
  unsigned long data_word;

  data_word = calcDataWord(in_freq);

  for(int i=0; i<32; i++)
  {
    digitalWrite(dds.data, (data_word & 0x01));
    pulse(dds.clk);

    data_word >>= 1;
  }

  for(int i=0; i<8; i++)
  {
    if(i<1)
      digitalWrite(dds.data, HIGH);
    else
      digitalWrite(dds.data, LOW);

    pulse(dds.clk);
  }

  pulse(dds.fq);
}

static void sweepUp(DDS dds, unsigned long c_freq, unsigned long s_dev, unsigned long s_step, unsigned int delay_us)
{
  unsigned int t = (unsigned int)(s_dev / s_step);
  
  if(s_step > s_dev)
      s_step = s_dev;
    
  for(unsigned int i=0; i<t; i++)
  {
    writeFreq(dds, 1UL*c_freq - s_dev/2 + i*s_step);
    delayMicroseconds(delay_us);
  }
}

static void sweepDn(DDS dds, unsigned long c_freq, unsigned long s_dev, unsigned long s_step, unsigned int delay_us)
{
  unsigned int t = (unsigned int)(s_dev / s_step);
  
  if(s_step > s_dev)
      s_step = s_dev;
    
  for(unsigned int i=0; i<t; i++)
  {
    writeFreq(dds, 1UL*c_freq + s_dev/2 - i*s_step);
    delayMicroseconds(delay_us);
  }
}

void sweepTone(DDS dds, unsigned long c_freq, unsigned long s_dev, long s_step, unsigned int delay_us)
{
  if(s_dev > (2*c_freq))
  	s_dev = 2*c_freq;
  
  if(s_step > 0)
  {
    if((s_dev / s_step) > (UINT_MAX - 1))
    	s_step = s_dev / (UINT_MAX - 1);

    sweepUp(dds, c_freq, s_dev, s_step, delay_us);
  }

  else if(s_step < 0)
  {
    s_step = -1 * s_step;

    if((s_dev / s_step) > (UINT_MAX - 1))
    	s_step = s_dev / (UINT_MAX - 1);

    sweepDn(dds, c_freq, s_dev, s_step, delay_us);
  }
  
  else
  {
    writeFreq(dds, 1UL*c_freq);
  }
}

#endif