/*******************************************************************************
 * ecg_vector.h
 * ECG vector analysis (cardiac axis calculation)
 ******************************************************************************/

#ifndef ECG_VECTOR_H
#define ECG_VECTOR_H

#include "ecg_leads.h"

typedef struct {
    float x;
    float y;
    float z;
    float magnitude;
    float azimuth;
    float elevation;
} heart_vector_t;

typedef struct {
    float angle_degrees;
    float angle_radians;
    char axis_description[32];
} cardiac_axis_t;

/* Function prototypes */
heart_vector_t compute_heart_vector(const standard_leads_t std,
                                    const augmented_leads_t aug,
									const chest_leads_t chest);
cardiac_axis_t compute_cardiac_axis(const standard_leads_t std);

#endif /* ECG_VECTOR_H */
