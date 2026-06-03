/*******************************************************************************
 * ecg_leads.c
 * Implementation of ECG lead calculations
 ******************************************************************************/

#include "ecg_leads.h"
#include <math.h>

standard_leads_t compute_standard_leads(const adas_frame_t *frame)
{
    standard_leads_t leads;

    float la = frame->channels[ADAS_CH_LA].scaled_value;
    float ll = frame->channels[ADAS_CH_LL].scaled_value;
    float ra = frame->channels[ADAS_CH_RA].scaled_value;

    leads.lead_I = la - ra;
    leads.lead_II = ll - ra;
    leads.lead_III = ll - la;

    return leads;
}

augmented_leads_t compute_augmented_leads(const standard_leads_t std)
{
    augmented_leads_t aug;

    aug.aVR = -(std.lead_I + std.lead_II) / 2.0f;
    aug.aVL = std.lead_I - std.lead_II / 2.0f;
    aug.aVF = std.lead_II - std.lead_I / 2.0f;

    return aug;
}

chest_leads_t compute_chest_leads(const adas_frame_t *frame)
{
    chest_leads_t chest;

    chest.v1 = frame->channels[ADAS_CH_V1].scaled_value;
    chest.v2 = frame->channels[ADAS_CH_V2].scaled_value;
    chest.v3 = 0.0f;  /* From slave device */
    chest.v4 = 0.0f;  /* From slave device */
    chest.v5 = 0.0f;  /* From slave device */
    chest.v6 = 0.0f;  /* From slave device */

    return chest;
}

all_12_leads_t compute_all_12_leads(const adas_frame_t *frame)
{
    all_12_leads_t all;

    all.standard = compute_standard_leads(frame);
    all.augmented = compute_augmented_leads(all.standard);
    all.chest = compute_chest_leads(frame);

    return all;
}
