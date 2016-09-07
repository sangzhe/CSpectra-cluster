//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_KNOWPROPERTIES_H
#define CSPECTRA_CLUSTER_KNOWPROPERTIES_H
#include "string"
#include "map"
#include "../Property/Properties.h"

using namespace std;
class KnownProperties {
public:
    static  string IDENTIFIED_PEPTIDE_KEY;
    static  string ANNOTATION_KEY;
    static  string TAXONOMY_KEY;
    static  string PROTEIN_KEY; // database: protein
    static  string MODIFICATION_KEY; // database: protein
    static  string INSTRUMENT_KEY;
    static  string SPECTRUM_TITLE;
    static  string IDENTIFIED_PEPTIDE_MGF_KEY;
    static  string ANNOTATION_MGF_KEY;
    static  string TAXONOMY_MGF_KEY;
    static  string PROTEIN_MGF_KEY;
    static  string MODIFICATION_MGF_KEY;
    static  string INSTRUMENT_MGF_KEY;
    static  string SPECTRUM_MGF_TITLE;
    static  string UNKNOWN_MGF_KEY;
    static  string MOST_COMMON_PEPTIDE_KEY;
    static  string N_HIGHEST_PEAKS; // highest peaks to use for comparison
    // future uses
    static  string PEPTIDES_LIST_COMMA_DELIMITED;
    static  string PEPTIDE_PURITY_STRING;

//private:
//    static map<string, string> INTERNAL_KEY_TO_MGF_KEY;
//    static map<string, string> INTERNAL_MGF_KEY_TO_KEY;
//
//    static bool handleKnownProperty(Properties props,string key, string value);

};


#endif //CSPECTRA_CLUSTER_KNOWPROPERTIES_H
