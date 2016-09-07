//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_MASS_H
#define CSPECTRA_CLUSTER_MASS_H


class Mass {
public:

    // Elements
 static  float PROTON;
 static  float NEUTRON;
 static  float ELECTRON ;

    // AAs
 static  float CARBON_MONO ;
 static  float CARBON_AVG  ;
 static  float HYDROGEN_MONO;
 static  float HYDROGEN_AVG;
 static  float OXYGEN_MONO;
 static  float OXYGEN_AVG ;
 static  float NITROGEN_MONO;
 static  float NITROGEN_AVG ;
 static  float SULFUR_MONO;
 static  float SULFUR_AVG;
 static  float PHOSPHORUS_MONO;
 static  float PHOSPHORUS_AVG ;

 static  float WATER_MONO;
 static  float WATER_AVG;

 static  float AMMONIA_MONO ;
 static  float AMMONIA_AVG ;

 static float getMonoisotopicMass(float precursorMz, int charge);

 static float getMz(float mass, int charge);

private:
    Mass();
};


#endif //CSPECTRA_CLUSTER_MASS_H
