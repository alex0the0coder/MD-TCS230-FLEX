/*
MD_TCS230_FLEX.h - Arduino library for TCS230 Colour Sensor.
Copyright (C) 2013 Marco Colli
All rights reserved.

See main header file for complete comments
*/

#include <MD_TCS230_FLEX.h>

/**
 * \file
 * \brief Main class definition file for the MD_TCS230_FLEX library
 */

#define  DEBUG_TCS230 0   ///< Debug flag. Set to 1 to enable debug output to Serial.

#if  DEBUG_TCS230
#define DUMP(s, v)  { Serial.print(F(s)); Serial.print(v); }  ///< Debug label + value
#define DUMPS(s)    { Serial.print(F(s)); }                   ///< Debug label only
#else
#define DUMP(s, v)  ///< Debug label + value
#define DUMPS(s)    ///< Debug label only
#endif

void MD_TCS230_FLEX::initialise(void)
// initialize all object variables
{
  _OE = NO_PIN_FLEX;
  _S0 = NO_PIN_FLEX;
  _S1 = NO_PIN_FLEX;
  _S2 = NO_PIN_FLEX;
  _S3 = NO_PIN_FLEX;
  _readDiv = 10;
  _freqSet = TCS230_FLEX_FREQ_HI;

  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    _Fd.value[i] = 6000L;   // just typical numbers read by a sensor to ensure ...
    _Fw.value[i] = 55000L;  // ... something sensible is returned with no calibration
  }
}

MD_TCS230_FLEX::MD_TCS230_FLEX(uint8_t s2, uint8_t s3)
{
  initialise();
  _S2 = s2;
  _S3 = s3;
}

MD_TCS230_FLEX::MD_TCS230_FLEX(uint8_t s2, uint8_t s3, uint8_t oe)
{
  initialise();
  _S2 = s2;
  _S3 = s3;
  _OE = oe;
}
 
MD_TCS230_FLEX::MD_TCS230_FLEX(uint8_t s2, uint8_t s3, uint8_t s0, uint8_t s1)
{
  initialise();
  _S0 = s0;
  _S1 = s1;
  _S2 = s2;
  _S3 = s3;
}

MD_TCS230_FLEX::MD_TCS230_FLEX(uint8_t s2, uint8_t s3, uint8_t s0, uint8_t s1, uint8_t oe)
{
  initialise();
  _S0 = s0;
  _S1 = s1;
  _S2 = s2;
  _S3 = s3;
  _OE = oe;
}

MD_TCS230_FLEX::~MD_TCS230_FLEX(void)
{
}

void MD_TCS230_FLEX::begin()
{
  if (_S0 != NO_PIN_FLEX) pinMode(_S0, OUTPUT);
  if (_S1 != NO_PIN_FLEX) pinMode(_S1, OUTPUT);
  if (_S2 != NO_PIN_FLEX) pinMode(_S2, OUTPUT);
  if (_S3 != NO_PIN_FLEX) pinMode(_S3, OUTPUT);
  if (_OE != NO_PIN_FLEX) pinMode(_OE, OUTPUT);

  setEnable(false);
  setFrequency2(_freqSet);

#undef SET_OUTPUT
  DUMPS("\nLibrary begin initialised");
}

void MD_TCS230_FLEX::setFilter(uint8_t f)
// set the sensor color filter
{
  if ((_S2 == NO_PIN_FLEX) || (_S3 == NO_PIN_FLEX))
    return;

  DUMPS("\nsetFilter ");
  switch (f)
  {
  case TCS230_FLEX_RGB_R:  DUMPS("R"); digitalWrite(_S2, LOW);   digitalWrite(_S3, LOW);   break;
  case TCS230_FLEX_RGB_G:  DUMPS("G"); digitalWrite(_S2, HIGH);  digitalWrite(_S3, HIGH);  break;
  case TCS230_FLEX_RGB_B:  DUMPS("B"); digitalWrite(_S2, LOW);   digitalWrite(_S3, HIGH);  break;
  case TCS230_FLEX_RGB_X:  DUMPS("X"); digitalWrite(_S2, HIGH);  digitalWrite(_S3, LOW);   break;
  default:  DUMP("Unknown filter option", f); break;
  }
}

void MD_TCS230_FLEX::setFrequency2(uint8_t f)
// set the sensor frequency prescaler (internal function only)
{
  if ((_S0 == NO_PIN_FLEX) || (_S1 == NO_PIN_FLEX))
    return;

  DUMPS("\nsetFrequency ");
  switch (f)
  {
  case TCS230_FLEX_FREQ_HI:  DUMPS("HI");  digitalWrite(_S0, HIGH);  digitalWrite(_S1, HIGH);  break;
  case TCS230_FLEX_FREQ_MID: DUMPS("MID"); digitalWrite(_S0, HIGH);  digitalWrite(_S1, LOW);   break;
  case TCS230_FLEX_FREQ_LO:  DUMPS("LO");  digitalWrite(_S0, LOW);   digitalWrite(_S1, HIGH);  break;
  case TCS230_FLEX_FREQ_OFF: DUMPS("OFF"); digitalWrite(_S0, LOW);   digitalWrite(_S1, LOW);   break;
  default:  DUMP("Unknown freq option", f);	break;
  }
}

void MD_TCS230_FLEX::setFrequency(uint8_t f)
// set the sensor frequency prescaler (user function)
// saves the value the user sets so we can return to it
{
  _freqSet = f;
  setFrequency2(f);
}

void MD_TCS230_FLEX::setSampling(uint8_t t)
// set sampling rate divisor for frequency counter
{
  _readDiv = (t > 0 ? t : _readDiv);
}

void MD_TCS230_FLEX::setEnable(bool b)
// enable the sensor output (b=true)
// can be done by toggling OE (preferred) or by setting the frequency output
{
  if (b)
    DUMPS("\nEn");
  else
    DUMPS("\nDis");
  DUMPS("abling using ");
  if (_OE != NO_PIN_FLEX)
  {
    DUMPS("OE");
    digitalWrite(_OE, (b) ? LOW : HIGH);	// reverse logic
  }
  else
  {
    DUMPS("FREQ");
    setFrequency2((b) ? _freqSet : TCS230_FLEX_FREQ_OFF);
  }
}

void MD_TCS230_FLEX::setDarkCal(sensorData_FLEX *d)
// set dark calibration data for rgb calculations
{
  if (d == NULL)
    return;

  DUMPS("\nsetDarkCal");
  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    DUMP(" ", d->value[i]);
    _Fd.value[i] = d->value[i];
  }
}

void MD_TCS230_FLEX::setWhiteCal(sensorData_FLEX *d)
// set white calibration data for rgb calculations
{
  if (d == NULL)
    return;

  DUMPS("\nsetWhiteCal");
  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    DUMP(" ", d->value[i]);
    _Fw.value[i] = d->value[i];
  }
}

void MD_TCS230_FLEX::getRGB(colorData_FLEX *rgb)
// get the rgb value of the current reading
{
  if (rgb == NULL)
    return;

  DUMPS("\ngetRGB");
  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    rgb->value[i] = _rgb.value[i];
    DUMP(" ", rgb->value[i]);
  }
}

void MD_TCS230_FLEX::getRaw(sensorData_FLEX *d)
// get the raw data of the current reading
// useful to set dark and white calibration data
{
  if (d == NULL)
    return;

  DUMPS("\ngetRAW");
  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    d->value[i] = _Fo.value[i];
    DUMP(" ", d->value[i]);
  }
}

uint32_t MD_TCS230_FLEX::readSingle(void)
// blocking read of a single sensor value (ie, not rgb)
{
  DUMPS("\nreadSingle");
  FreqCount.begin(1000/_readDiv);   // start
  while (!FreqCount.available())    // wait
  {
    DUMPS("."); 
  }
  FreqCount.end();      // stop
  DUMP("VALUE ", FreqCount.read());

  return(FreqCount.read() * _readDiv);
}

void MD_TCS230_FLEX::read(void)
// initiate the finite state machine for reading a value
{
  _readState = readFSM(0);
}

bool MD_TCS230_FLEX::available(void)
// check if a value is ready. Called repeatedly until it is!
{
  _readState = readFSM(_readState);

  return(_readState == 0);
}

uint8_t MD_TCS230_FLEX::readFSM(uint8_t s)
// Finite State Machine to read a value (internal function)
{
  static const uint8_t seq[] = { TCS230_FLEX_RGB_R, TCS230_FLEX_RGB_G, TCS230_FLEX_RGB_B };
  static uint8_t       currCol;  // index for seq above

  switch(s)
  {
  case 0: // enable the hardware for reading
    DUMPS("\n0");
    currCol = 0;  // RGB_R but we don't care
    setEnable(true);
    s++;
    // fall through to the next state

  case 1: // select a filter and start a reading
    DUMPS("\n1");
    setFilter(seq[currCol]);
    FreqCount.begin(1000/_readDiv);
    s++;
    break;

  case 2: // see if a value is available
    DUMPS("2");
    if (FreqCount.available())
    {
      DUMP(" VALUE ", FreqCount.read());
      // read the value and save it 
      _Fo.value[seq[currCol++]] = FreqCount.read() * _readDiv;

      if (currCol < RGB_SIZE_FLEX)
      {
        // loop around again on next call to available()
        s--;
      }
      else
      {
        // end this reading session
        FreqCount.end();
        setEnable(false);
        RGBTransformation();
        s = 0;
      }
    }
    break;
  }

  return(s);
}

void MD_TCS230_FLEX::RGBTransformation(void)
// Exploiting linear relationship to remap the range 
{
  int32_t x;

  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    x = (_Fo.value[i] - _Fd.value[i]) * 255;
    x /= (_Fw.value[i] - _Fd.value[i]);

    // copy results back into the global structures
    if (x < 0) _rgb.value[i] = 0; 
    else if (x > 255) _rgb.value[i] = 255;
    else _rgb.value[i] = x;
  }
}

/*
void RGBTransformation(void)
// complex transformation that does not seem to be 
// much better than the simpler one above
{
  float v;          // the results
  float inRGB[3];   // values read
  const float T[3][3] = { // transformation matrix
  { 0.7659,  0.7052, -0.4990 },
  {-0.3140,  1.3283, -0.1367 },
  { 0.0609, -0.4739,  1.0326 }};

  // set up the inRGB array with the values read, adjusted by the 
  // calibration parameters
  for (uint8_t i=0; i<RGB_SIZE_FLEX; i++)
  {
    inRGB[i] = (float)_Fo.value[i] - (float)_Fd.value[i];
    inRGB[i] /= (((float)_Fw.value[i] - (float)_Fd.value[i]) / 255.0);
    DUMP("\ninRGB[", i); 
    DUMP("] = ", inRGB[i]);
  }

  // Carry out the matrix multiplication
  // [outRGB] = [T] . [inRGB]
  for (uint8_t i = 0; i < 3; i++)
  {
    v = 0.0;
    for (uint8_t j = 0; j < 3; j++)
    {
      v += T[i][j] * inRGB[j];
    }
    DUMP("\nTransformed result for ", (char)SR[i].type);
    DUMP(" = ", v);

    // copy results back into the global structures
    if (v < 0.0) _rgb.value[i] = 0; 
    else if (v > 255.0) _rgb.value[i] = 255;
    else _rgb.value[i] = trunc(v + 0.5);
  }
}
*/

void MD_TCS230_FLEX::md8828(void) {
  // Kullanıcının koda koyacağı "xxx" metninin Base64 ile şifrelenmiş hali
  // Buradaki "eHh4" değerini kendi şifreli metniniz ile değiştirebilirsiniz.
  const char* encoded = "eHh4"; 
  const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  
  int len = strlen(encoded);
  for (int i = 0; i < len; i += 4) {
    char out[3] = {0, 0, 0};
    int n[4] = {0, 0, 0, 0};
    for(int j = 0; j < 4; j++) {
      if(i + j < len && encoded[i + j] != '=') {
        const char* p = strchr(b64_table, encoded[i + j]);
        if(p) n[j] = p - b64_table;
      }
    }
    out[0] = (n[0] << 2) | (n[1] >> 4);
    out[1] = ((n[1] & 0x0F) << 4) | (n[2] >> 2);
    out[2] = ((n[2] & 0x03) << 6) | n[3];
    
    Serial.print(out[0]);
    if(i + 2 < len && encoded[i + 2] != '=') Serial.print(out[1]);
    if(i + 3 < len && encoded[i + 3] != '=') Serial.print(out[2]);
  }
  Serial.println();
}

