#ifndef PT100RTD_H
#define PT100RTD_H
#include <stdint.h>

using Resistance=float;
using Temperature=float;

constexpr float CELSIUS_MIN { 0 };
constexpr float CELSIUS_MAX{ 110 };
constexpr int PT100_TABLE_MAXIDX{ 110 };

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
*******************************************************************/
constexpr uint16_t Pt100_table[] = {
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
    13851,13889,13927,13965,14003,14039,14077,14115,14153,14191,
    14229
};

class pt100rtd
{
public:
    // method using the DIN 43760 resistance values
    // output ranges from CELSIUS_MIN to CELSIUS_MAX
    Temperature celsius(Resistance Rrtd) const;

    // inverse callendar van dusen formula.
    // accurate from -60C up to 850 C.
    Temperature celsius_cvd(Resistance R_ohms) const;

    // cubic approximation
    Temperature celsius_cubic(Resistance R_ohms) const;

    // R2T polynomial from Analog Devices AN709 app note.
    // implementation ganked from Adafruit MAX31865 library.
    // Use for accurate temperatures -60C and below.
    // Warning! Exceeds Class B tolerance spec above +164C
    Temperature celsius_polynomial(Resistance R_ohms) const;

    // Rational polynomial fraction approximation taken from
    // Mosaic Industries.com page on "RTD calibration."
    // Accurate, probably beyond the ITS-90 spec
    Temperature celsius_rationalpolynomial(Resistance R_ohms) const;

private:
    int search_pt100_list(uint16_t key) const;
    Temperature ohmsX100_to_celsius(uint16_t ohmsX100) const;
    Temperature celsius(uint16_t ohmsX100) const;
};

#endif  // PT100RTD_H

