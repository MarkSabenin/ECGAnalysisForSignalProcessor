/*******************************************************************************
 * ecg_vector.c
 * Implementation of ECG vector analysis
 ******************************************************************************/

#include "ecg_vector.h"
#include <math.h>
#include <string.h>

heart_vector_t compute_heart_vector(const standard_leads_t std,
                                    const augmented_leads_t aug,
                                    const chest_leads_t chest)
{
    heart_vector_t hv;

    hv.x = std.lead_I;
    hv.y = aug.aVF;
    hv.z = (chest.v1 + chest.v2) / 2.0f;  /* Use chest leads */

    hv.magnitude = sqrtf(hv.x * hv.x + hv.y * hv.y + hv.z * hv.z);

    if (hv.x != 0.0f) {
        hv.azimuth = atan2f(hv.y, hv.x) * 180.0f / 3.14159265359f;
    } else {
        hv.azimuth = (hv.y > 0) ? 90.0f : -90.0f;
    }

    hv.elevation = 0.0f;

    return hv;
}

cardiac_axis_t compute_cardiac_axis(const standard_leads_t std)
{
    cardiac_axis_t axis;

    if (std.lead_I != 0.0f) {
        axis.angle_radians = atan2f(std.lead_II, std.lead_I);
        axis.angle_degrees = axis.angle_radians * 180.0f / 3.14159265359f;
    } else {
        axis.angle_degrees = (std.lead_II > 0) ? 90.0f : -90.0f;
        axis.angle_radians = axis.angle_degrees * 3.14159265359f / 180.0f;
    }

    if (axis.angle_degrees >= -30.0f && axis.angle_degrees <= 90.0f) {
        strcpy(axis.axis_description, "normal");
    } else if (axis.angle_degrees < -30.0f) {
        strcpy(axis.axis_description, "left deviation");
    } else {
        strcpy(axis.axis_description, "right deviation");
    }

    return axis;
}
