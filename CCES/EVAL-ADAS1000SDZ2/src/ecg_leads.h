/*******************************************************************************
 * ecg_leads.h
 * ECG lead calculations (based on ADAS1000 documentation)
 ******************************************************************************/

#ifndef ECG_LEADS_H
#define ECG_LEADS_H

#include "adas1000_data.h"

/* Standard leads (I, II, III) */
typedef struct {
    float lead_I;
    float lead_II;
    float lead_III;
} standard_leads_t;

/* Augmented leads (aVR, aVL, aVF) */
typedef struct {
    float aVR;
    float aVL;
    float aVF;
} augmented_leads_t;

/* Chest leads (V1-V6) */
typedef struct {
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
} chest_leads_t;

/* All 12 leads */
typedef struct {
    standard_leads_t standard;
    augmented_leads_t augmented;
    chest_leads_t chest;
} all_12_leads_t;

/* Function prototypes */
standard_leads_t compute_standard_leads(const adas_frame_t *frame);
augmented_leads_t compute_augmented_leads(const standard_leads_t std);
chest_leads_t compute_chest_leads(const adas_frame_t *frame);
all_12_leads_t compute_all_12_leads(const adas_frame_t *frame);

#endif /* ECG_LEADS_H */
