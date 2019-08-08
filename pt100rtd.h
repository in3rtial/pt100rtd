/*******************************************************************
    PT100RTD
     
    This library uses integer math to convert a Pt100 RTD resistance to a
    celsius temperature of two decimal point precision.
 
    It searches an ordered list of empirical resistances which correspond to
    temperatures ranging -200C to +850C, the complete span for which 100 ohm
    platinum RTDs are specified.  It interpolates temperatures from intermediate
    resistances.
 
    AUTHOR:    drh

    DATE:2/18/2017

*******************************************************************/


#ifndef PT100RTD_H
#define PT100RTD_H

#include <stdint.h>
#include <math.h>

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif


/*******************************************************************
*    pt100rtd list of resistances v temperature
*
*    DIN 43760 resistance values (ohms) were multiplied by 100 and
*    converted to 16 bit unsigned integers with no loss of accuracy.
* 
*    Examples:
*    1852 represents 18.52 ohms and corresponds to a temperature of -200C.
*    10000 ==> 100.00 ohms @   0C
*    13851 ==> 138.51 ohms @ 100C 
* 
*   The list resides in flash since it's too large for SRAM or EEPROM
*******************************************************************/
constexpr float CELSIUS_MIN { 0 };
constexpr float CELSIUS_MAX{ 109 };
constexpr int PT100_TABLE_MAXIDX{ 109 };

const static uint16_t Pt100_table[] = {
    10000,10039,10078,10117,10156,10195,10234,10273,10312,10351,
    10390,10429,10468,10507,10546,10585,10624,10663,10702,10740,
    10779,10818,10857,10896,10935,10973,11012,11051,11090,11128,
    11167,11206,11245,11283,11322,11361,11399,11438,11477,11515,
    11554,11593,11631,11670,11708,11747,11785,11824,11862,11901,
    11940,11978,12016,12055,12093,12132,12170,12209,12247,12286,
    12324,12362,12401,12439,12477,12517,12555,12593,12632,12670,
    12708,12746,12785,12823,12861,12899,12938,12976,13014,13052,
    13090,13128,13167,13205,13243,13281,13319,13357,13395,13433,
    13471,13509,13547,13585,13623,13661,13699,13737,13775,13813,
    13851,13889,13927,13965,14003,14039,14077,14115,14153,14191
};


class pt100rtd
{
private:
    int search_pt100_list(uint16_t key);
    float ohmsX100_to_celsius(uint16_t ohmsX100);

public:
    float celsius(float Rrtd);

    float celsius_cvd(float R_ohms);
    float celsius_cubic(float R_ohms);
    float celsius_polynomial (float R_ohms);
    float celsius_rationalpolynomial (float R_ohms);

private:
    float celsius (uint16_t ohmsX100);
};

#endif  // PT100RTD_H

