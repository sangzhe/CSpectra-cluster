//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_KNOWPROPERTIES_H
#define CSPECTRA_CLUSTER_KNOWPROPERTIES_H
#include "string"
#include "map"
#include "Properties.h"

using namespace std;
class KnowProperties {
public:
    static const string IDENTIFIED_PEPTIDE_KEY;
    static const string ANNOTATION_KEY;
    static const string TAXONOMY_KEY;
    static const string PROTEIN_KEY; // database: protein
    static const string MODIFICATION_KEY; // database: protein
    static const string INSTRUMENT_KEY;
    static const string SPECTRUM_TITLE;
    static const string IDENTIFIED_PEPTIDE_MGF_KEY;
    static const string ANNOTATION_MGF_KEY;
    static const string TAXONOMY_MGF_KEY;
    static const string PROTEIN_MGF_KEY;
    static const string MODIFICATION_MGF_KEY;
    static const string INSTRUMENT_MGF_KEY;
    static const string SPECTRUM_MGF_TITLE;
    static const string UNKNOWN_MGF_KEY;
    static const string MOST_COMMON_PEPTIDE_KEY;
    static const string N_HIGHEST_PEAKS; // highest peaks to use for comparison
    // future uses
    static const string PEPTIDES_LIST_COMMA_DELIMITED;
    static const string PEPTIDE_PURITY_STRING;

private:
    static map<string, string> INTERNAL_KEY_TO_MGF_KEY;
    static map<string, string> INTERNAL_MGF_KEY_TO_KEY;

    static bool handleKnownProperty(Properties props,string key, string value);

};


#endif //CSPECTRA_CLUSTER_KNOWPROPERTIES_H
