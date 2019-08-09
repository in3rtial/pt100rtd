#include "pt100rtd.h"
#include <math.h>

int pt100rtd::search_pt100_list(uint16_t ohmsX100) const
{
    int lower = 0;
    int upper = PT100_TABLE_MAXIDX;
    int mid = (lower + upper) / 2;

    do
    {
        uint16_t pt100val = Pt100_table[mid];

        if (pt100val == ohmsX100)
        {
            break;
        }
        else if (pt100val < ohmsX100)
        {
            lower = mid + 1;
        }
        else
        {
            upper = mid;
        }
                
        mid = (lower + upper) / 2;
                
    } while (lower < upper)    ;
        // falls through on last mismatch

    return(mid);
}

float pt100rtd::ohmsX100_to_celsius (uint16_t ohmsX100) const
{
    uint16_t R_upper, R_lower;
    int hundredths = 0;                 // STFU flag for avr-gcc
    int iTemp = 0;
    float celsius;

    int index = search_pt100_list(ohmsX100);
        
    // The minimum integral temperature
    iTemp = index - 1 + CELSIUS_MIN;
        
    // fetch floor() and ceiling() resistances since
    // key = intermediate value is the most likely case.

    // ACHTUNG!  (index == 0) is forbidden!
    R_lower = Pt100_table[index - 1];
    R_upper = Pt100_table[index];

    // if key == table entry, temp is an integer degree
    if (ohmsX100 == R_upper)
    {
        iTemp++;
        hundredths = 0;
    }
    // an intermediate resistance is the common case
    else if (ohmsX100 < R_upper)
    {
        hundredths = ((100 * (ohmsX100 - R_lower)) / (R_upper - R_lower));
    }
    // two unlikely cases are included for disaster recovery
    else if (ohmsX100 > R_upper) /*NOTREACHED*/  /*...unless list search was dain bramaged */
    {
        iTemp++;
        // risks index+1 out of range
        uint16_t Rnext = Pt100_table[index + 1];
        hundredths = (100 * (ohmsX100 - R_upper)) / (Rnext - R_upper);
    }
    else        /*NOTREACHED*/  /*...except in cases of excessive tweakage at 2:30am */
    {
        hundredths = ((100 * (ohmsX100 - R_lower)) / (R_upper - R_lower));
    }

    celsius  = static_cast<float>(iTemp) + static_cast<float>(hundredths) / 100.0;

    return celsius;
}

// Uses minimally-processed ADC binary output,
// an unsigned 16 bit integer == (ohms * 100).
float pt100rtd::celsius (uint16_t ohmsX100) const
{
    if ((ohmsX100 < Pt100_table[0]) || (ohmsX100 > Pt100_table[PT100_TABLE_MAXIDX]))
    {
        return NAN;
    }
    return pt100rtd::ohmsX100_to_celsius(ohmsX100);
}

float pt100rtd::celsius(float rtd_ohms) const
{
    uint16_t ohmsX100 = (uint16_t) floor(rtd_ohms * 100.0);
    return pt100rtd::celsius(ohmsX100);
}

float pt100rtd::celsius_cvd(float R_ohms) const
{
    constexpr float PT100_NOMINAL { 100.0 };
    constexpr float iCVD_A { 3.9083e-3 };
    constexpr float iCVD_B { -5.775e-7 };

    float Z1, Z2, Z3, Z4, temp;

    Z1 = -iCVD_A;
    Z2 = iCVD_A * iCVD_A - (4 * iCVD_B);
    Z3 = (4 * iCVD_B) / PT100_NOMINAL;
    Z4 = 2 * iCVD_B;

    temp = Z2 + (Z3 * R_ohms);
    temp = (sqrt(temp) + Z1) / Z4;

    return temp;
}


float pt100rtd::celsius_cubic(float R_ohms) const
{
    return -247.29 + ( R_ohms * ( 2.3992 + R_ohms * (0.00063962 + 1.0241E-6 * R_ohms)) );
}

float pt100rtd::celsius_polynomial(float R_ohms) const
{
    float rpoly = R_ohms;
    float temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= R_ohms;                        // square;
    temp += 2.5859e-3 * rpoly;
    rpoly *= R_ohms;                        // ^3;
    temp -= 4.8260e-6 * rpoly;
    rpoly *= R_ohms;                        // ^4;
    temp -= 2.8183e-8 * rpoly;
    rpoly *= R_ohms;                        // ^5;
    temp += 1.5243e-10 * rpoly;

    return temp;
}

float pt100rtd::celsius_rationalpolynomial(float R_ohms) const
{
    constexpr float c0 { -245.19 };
    constexpr float c1 { 2.5293 };
    constexpr float c2 { -0.066046 };
    constexpr float c3 { 4.0422E-3 };
    constexpr float c4 { -2.0697E-6 };
    constexpr float c5 { -0.025422 };
    constexpr float c6 { 1.6883E-3 };
    constexpr float c7 { -1.3601E-6 };

    const float num{ R_ohms * (c1 + R_ohms * (c2 + R_ohms * (c3 + R_ohms * c4)))};
    const float denom{ 1.0 + R_ohms * (c5 + R_ohms * (c6 + R_ohms * c7))};
    const float T{ c0 + (num / denom)};

    return T;
}

/*END*/
