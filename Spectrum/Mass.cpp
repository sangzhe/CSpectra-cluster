//
// Created by SangZhe on 16/9/7.
//

#include "Mass.h"
// Elements
float Mass::PROTON = 1.007276F;
float Mass::NEUTRON = 1.008665F;
float Mass::ELECTRON = 0.000549F;

// AAs
float Mass::CARBON_MONO        = 12.00000F;
float Mass::CARBON_AVG         = 12.01078F;
float Mass::HYDROGEN_MONO      = 01.00783F;
float Mass::HYDROGEN_AVG       = 01.00794F;
float Mass::OXYGEN_MONO        = 15.99491F;
float Mass::OXYGEN_AVG         = 15.99943F;
float Mass::NITROGEN_MONO      = 14.00304F;
float Mass::NITROGEN_AVG       = 14.00672F;
float Mass::SULFUR_MONO        = 31.97207F;
float Mass::SULFUR_AVG         = 32.06550F;
float Mass::PHOSPHORUS_MONO    = 30.97376F;
float Mass::PHOSPHORUS_AVG     = 30.97376F;

float Mass::WATER_MONO     = 2*HYDROGEN_MONO + OXYGEN_MONO;
float Mass::WATER_AVG      = 2*HYDROGEN_AVG + OXYGEN_AVG;

float Mass::AMMONIA_MONO   = 3*HYDROGEN_MONO + NITROGEN_MONO;
float Mass::AMMONIA_AVG    = 3*HYDROGEN_AVG + NITROGEN_AVG;

float Mass::getMonoisotopicMass(float precursorMz, int charge) {
    float floatCharge = (float) charge;
    float mass = (precursorMz * floatCharge) - (floatCharge * PROTON);

    return mass;
}

float Mass::getMz(float mass, int charge) {
    float floatCharge = (float) charge;
    float mz = (mass + (floatCharge * PROTON)) / floatCharge;

    return mz;
}

Mass::Mass() {}