/****************************************************************************
NAME: ias_geo_convert_dms2deg

PURPOSE:  Convert angles to total degrees.

RETURN VALUE:   Type = int
Value    Description
-----    -----------
SUCCESS  Successful completion
ERROR    Operation failed

ALGORITHM DESCRIPTION:
        Receive an angle in DMS
        Convert the angle to total degrees (in place).
        The angle is then checked to be sure it is within the limits of
          its use (LAT, LON, or DEGREES).

*****************************************************************************/
#include "ias_const.h"
#include "ias_logging.h"
#include "ias_geo.h"

int ias_geo_convert_dms2deg 
(
    double angle_dms,     /* I: Angle in DMS (DDDMMMSSS) format */
    double *angle_degrees,/* O: Angle in decimal degrees */
    const char *type      /* I: Angle usage type (LAT, LON, or DEGREES) */
)
{
    float second;     /* Second of DMS angle value */
    float upper;      /* Upper bound of the angle value for its use */
    float lower;      /* Lower bound of the angle value for its use */

    int  degree;      /* Degree of DMS angle value */
    int  minute;      /* Minute of DMS angle value */
    short sign;       /* Sign of the angle */

    /* Find the upper and lower bound limits for the angle based on its
       usage type.  */
    if (strncmp (type,"LAT",3) == 0) 
    { /* LAT */
        upper = 90.0;
        lower = -90.0;
    } /* LAT */
    else if (strncmp (type,"LON",3) == 0)
    { /* LON */
        upper = 180.0;
        lower = -180.0;
    } /* LON */
    else
    { /* DEGREES */
        upper = 360.0;
        lower = 0.0;
    }

    /* Convert the angle to total degrees based on DMS.  */
    if (angle_dms < 0)
    {
        sign = -1;
        angle_dms = angle_dms * -1;
    }
    else
        sign = 1;

    degree = (int) (angle_dms / 1000000);
    angle_dms = angle_dms - (degree * 1000000);
    minute = (int) (angle_dms / 1000);
    second = angle_dms - (minute * 1000);
    *angle_degrees = sign * (degree + (minute/60.0) + (second/3600.0));

    /* Check to make sure the angle is within the limits of its use (LAT, LON,
       or DEGREES) */
    if ((*angle_degrees > upper) || (*angle_degrees < lower))
    {
        IAS_LOG_ERROR("Illegal coordinate value in decdeg");
        IAS_LOG_ERROR("Calculated angle of %f outside bounds of %f to %f",
            *angle_degrees, lower, upper);
        return ERROR;
    }

    return SUCCESS;
}
