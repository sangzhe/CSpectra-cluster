//
// Created by SangZhe on 16/8/24.
//
#include "string"
#include "stdlib.h"
#include "ICluster.h"
#include "sstream"
#include "Properties.h"
#include "exception"
#include "Spectrum.h"
#include "Defaults.h"
#include "KnownProperties.h"
#include "Functions.h"
#include "Peak.h"
#include "ComparisonMatch.h"
#include "SpectralClusrer.h"

#ifndef CSPECTRA_CLUSTER_PARSERUTILITIES_H
#define CSPECTRA_CLUSTER_PARSERUTILITIES_H

using namespace std;
class ParserUtilities {
    public:
        ParserUtilities();

        static const int MAX_NUMBER_BAD_MGF_LINES;


        static list<ICluster*>* readSpectralCluster(string& clusterString);

        static ICluster* readSpectralCluster( stringstream& ss, string& line);

        static ISpectrum* readMGFScan( stringstream& ss,  string& line);

        static string convertClusterToString(ICluster& cluster);

        static double parsePepMassLine(string& pLine);

    private:
        static const string BEGIN_IONS;
        static const string END_IONS;
        static const string BEGIN_CLUSTER;
        static const string END_CLUSTER;

        static const string titleTile;
        static const string sequenceTitle;
        static const string titleAndId;
        static const string pepmassTitle;
        static const string chargeTitle;
        static const string rtinsecondsTitle;
        static const string taxonomyTitle;
        static const string taxonTitle;
        static const string user02Title;
        static const string user03Title;
        static string buildMGFTitle(string line);

        static const string NOT_HANDLED_MGF_TAGS[18];
        static int gNumberBadMGFLines;

        static Properties* parseProperties(string& line);

//        static stringstream skipLines(stringstream& ss,int& num);

        static IConsensusSpectrumBuilder* parseConsensusSpectrumBuilder(stringstream& ss, string& line);





    protected:
        static void handleTitleLine(ISpectrum& spectrum, string& titleLine);
        static bool storesPeakListFromClusterLine(string& line);
        static void handleBadMGFData(string& line);
        static string idFromClusterLine(string& line);
        static list<ComparisonMatch>* parseComparisonMatches(string line);





};


#endif //CSPECTRA_CLUSTER_PARSERUTILITIES_H
