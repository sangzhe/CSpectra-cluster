//
// Created by SangZhe on 16/8/24.
//

#include "ParserUtilities.h"
#include "ConsensusSpectrum.h"

const string BEGIN_IONS = "BEGIN IONS";
const string END_IONS = "END IONS";
const string BEGIN_CLUSTER = "BEGIN CLUSTER";
const string END_CLUSTER = "END CLUSTER";

const string titleTile = "TITLE=";
const string sequenceTitle = ",sequence=";
const string titleAndId = "TITLE=id=";
const string pepmassTitle = "PEPMASS=";
const string chargeTitle = "CHARGE=";
const string rtinsecondsTitle = "RTINSECONDS=";
const string taxonomyTitle = "TAXONOMY=";
const string taxonTitle = "TAXON=";
const string user02Title = "USER02=";
const string user03Title = "USER03=";

const string NOT_HANDLED_MGF_TAGS[18] = {
                                 "TOLU=",
                                 "TOL=",
                                 "USER00",
                                 "USER01",
                                 "USER04",
                                 "USER05",
                                 "USER06",
                                 "USER08",
                                 "USER09",
                                 "USER10",
                                 "USER11",
        //   "TAXONOMY=",
        //      "SEQ=",
                                 "COMP=",
                                 "TAG=",
                                 "ETAG=",
                                 "SCANS=",
                                 "IT_MODS=",
                                 "CLUSTER_SIZE=",
                                 "PRECURSOR_INTENSITY="};

//stringstream* ParserUtilities::skipLines(stringstream ss,int num) {
//    int startLine = num;
//    int count = 0;
//    string tmp = "";
//    for(count;count<startLine;count++){
//        getline(ss,tmp);
//    }
//    return new stringstream(ss);
//}

string ParserUtilities::convertClusterToString(ICluster& cluster) {

}

ICluster* ParserUtilities::readSpectralCluster(stringstream& ss, string& line) {
    string currentId = "";
    bool storesPeakLists = false;
    list<ISpectrum*> *spectra = new list<ISpectrum*>;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;
    list<ComparisonMatch> *comparisonMatches = new list<ComparisonMatch>;

    try{
        if(line == "") getline(ss,line);
        while ( line != ""){
            if(line.substr(0,BEGIN_CLUSTER.length()) == BEGIN_CLUSTER){
                currentId = idFromClusterLine(line);
                storesPeakLists =storesPeakListFromClusterLine(line);
                break;
            }
            if(line.substr(0,BEGIN_IONS.length()) == BEGIN_IONS){
                ISpectrum* internalComplete = readMGFScan(ss,line);
                list<IPeak*> peaks = internalComplete->getPeaks();
                ISpectrum *internal = new Spectrum(*internalComplete,peaks);
                ISpectrum *internalFilterd = internal;
                Defaults::getDefaultPeakFilter(*internalFilterd);

                IConsensusSpectrumBuilder* consensusSpectrumBuilder = Defaults::getDefaultConsensusSpectrumBuilder();
                string Id = internal->getId();
                ICluster *ret = new SpectralCluster(Id,*consensusSpectrumBuilder);
                ret->addSpectra(*internalFilterd);
                delete spectra,internalFilterd,internal;
                return ret;
            }
            getline(ss,line);
        }
        if(line == "") return NULL;

        getline(ss,line);

        Properties *properties = new Properties();
        string propertyTitle = "Properties=";
        string comparisonTitle = "ComparisonMatched=";
        string BEGIN_CONSENSUS = "BEGIN CONSENSUS";
        if(line.substr(0,propertyTitle.length()) == propertyTitle){
            properties = parseProperties(line);
            getline(ss,line);
        }
        if(line.substr(0,comparisonTitle.length()) == comparisonTitle){
            comparisonMatches = parseComparisonMatches(line);
            getline(ss,line);
        }

        while(line != "" && line.length() <1) getline(ss,line);

        if(line != "" && line.substr(0,BEGIN_CONSENSUS.length()) == BEGIN_CONSENSUS){
            consensusSpectrumBuilder = parseConsensusSpectrumBuilder(ss,line);
        }
        while (line != ""){
            ISpectrum* internal = readMGFScan(ss,line);
            if(internal != NULL)
                spectra->push_back(internal);
            getline(ss,line);
            if(line == "")
                return NULL;
            if(line.substr(0,END_CLUSTER.length()) == END_CLUSTER){
                ICluster* ret;
                if(storesPeakLists){
                    IConsensusSpectrumBuilder* consensusSpectrumBuilder = Defaults::getDefaultConsensusSpectrumBuilder();

                    ret = new SpectralCluster(currentId,*consensusSpectrumBuilder);

                    ret->addSpectra(*spectra);
                    delete spectra;
                }
                else{
//                    ToDO add GreedySpectralCluster
//                    ret = new GreedySpectralCluster(currentId, spectra, (GreedyConsensusSpectrum) consensusSpectrumBuilder, comparisonMatches);
                }

                set<string>::iterator iter;
                int i =0;
                for(iter= properties->getKeySet().begin();iter != properties->getKeySet().end();iter++){
                    string name = *iter;
                    ret->setProperty(name,properties->getProperty(name));
                }
                return ret;
            }
        }


    }catch(exception){
        throw("runtime_error");
    }
    return NULL;




}

list<ICluster*>* ParserUtilities::readSpectralCluster(string& clusterString) {
    list<ICluster*> *holder = new list<ICluster*>;
    stringstream ss;
    ss << clusterString;
    string line ="";
    ICluster* cls =  readSpectralCluster(ss,line);
    while (cls != NULL){
        holder->push_back(cls);
        cls = readSpectralCluster(ss,line);
    }
    return holder;
}

ISpectrum* ParserUtilities::readMGFScan(stringstream& ss, string& line) {
    string titleLine = "";
    string sequence = "";
    string protein = "";
    string species = "";
    string modifications = "";




    Properties *props = new Properties();

    string annotation = "";

    try{
        if(line == "") getline(ss,line);
        double massToChargeCalledPpMass = 0 ;
        int dcharge = 1;
        string title = "";

        while(line != ""){
            if(line == ""){
                getline(ss,line);
                continue;
            }
            if (BEGIN_IONS == line){
                getline(ss,line);
                break;
            }
            getline(ss,line);
        }
        if (line == "") return NULL;

        list<IPeak*> *holder = new list<IPeak*>;

        while (line != ""){
            getline(ss,line);

            if (line.length() == 0){
                getline(ss,line);
                continue;
            }
            // give up on lines not starting with a letter
            if (!isalnum(line.at(0))){
                getline(ss,line);
                continue;
            }

            if(line.find("=")){
                if(line.substr(0,titleTile.length()) == titleTile){
                    titleLine = line;
                    title = buildMGFTitle(line);
                    int index = line.find(sequenceTitle);
                    if(index != string::npos){
                        sequence = line.substr(index+sequenceTitle.length());
                    }
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,pepmassTitle.length()) == pepmassTitle){
                    massToChargeCalledPpMass = parsePepMassLine(line);
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,chargeTitle.length()) == chargeTitle){
                    line = line.substr(chargeTitle.length()-1); //strip the "+" or "-" behind the value
                    string substring = line.substr(chargeTitle.length());
                    if(substring.find(".")){
                        dcharge = (int)(0.5 + atof(substring.data()));
                    }
                    else{
                        dcharge = atoi(substring.data());
                    }
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,rtinsecondsTitle.length()) == rtinsecondsTitle){
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,taxonomyTitle.length()) == taxonomyTitle){
                    species = line.substr(taxonomyTitle.length());
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,taxonTitle.length()) == taxonTitle){
                    species = line.substr(taxonTitle.length());
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,user02Title.length()) == user02Title){
                    protein = line.substr(user02Title.length());
                    getline(ss,line);
                    continue;
                }
                if(line.substr(0,user03Title.length()) == user03Title){
                    modifications = line.substr(user03Title.length());
                    getline(ss,line);
                    continue;
                }
//                if(KnowProperties::addMGFProperties(props,line)){
//                    getline(skiped_ss,line);
//                    continue;
//                }
                bool tagIsNotHandled = false;
                for(int i = 0;i < NOT_HANDLED_MGF_TAGS->length();i++){
                    string notHandledMgfTag = NOT_HANDLED_MGF_TAGS[i];
                    if (line.substr(0,notHandledMgfTag.length()) == notHandledMgfTag){
                        tagIsNotHandled = true;
                        getline(ss,line);
                        break;
                    }

                }
                if(tagIsNotHandled) continue;

             //   throw new runtime_error("Cannot parse MGF line"+line);
            }
            if (END_IONS == line ){
                double mz = massToChargeCalledPpMass;

                string peptide = sequence;
                sequence = "";

                holder->sort(Peak::cmpPeak);

                IQualityScorer* defaultQualityScorer = Defaults::getDefaultQualityScorer();
                ISpectrum *spectrum = new Spectrum(title,dcharge,(float)mz,defaultQualityScorer,*holder);

                Defaults::getDefaultPeakFilter(*spectrum);

                set<string> keys = props->getKeySet();
                set<string>::iterator keysIter;
                for(keysIter = keys.begin();keysIter != keys.end();keysIter++){
                    spectrum->setProperty(*keysIter,props->getProperty(*keysIter));
                }

                props->clear();

                if (species != "")
                    spectrum->setProperty(KnowProperties::TAXONOMY_KEY,species);
                if (peptide != "")
                    spectrum->setProperty(KnowProperties::IDENTIFIED_PEPTIDE_KEY, peptide);
                if (peptide != "")
                    spectrum->setProperty(KnowProperties::ANNOTATION_KEY, title);
                if (protein != "")
                    spectrum->setProperty(KnowProperties::PROTEIN_KEY, protein);
                if (modifications != "")
                    spectrum->setProperty(KnowProperties::MODIFICATION_KEY, modifications);
                if (titleLine != "")
//                    handleTitleLine(spectrum, titleLine);
                delete props,holder;
                return spectrum;
            }
            else {
                Functions::replace(line,"\t", " ");
                vector<string> items;
                Functions::split(line," ",items);
                if(items.size() >= 2){
                    try{
                        float peakMass = (float)atof(items[0].data());
                        float peakIntensity = (float)atof(items[1].data());
                        Peak *added = new Peak(peakMass,peakIntensity);
                        holder->push_back(added);
                    }catch(exception s){
//                        handleBadMGFData(line);
                    }
                }else{
//                    handleBadMGFData(line);
                }
                getline(ss,line);

            }

        }
    return NULL;
    }catch(exception){
       // throw new runtime_error(" ");
    }


}

IConsensusSpectrumBuilder* ParserUtilities::parseConsensusSpectrumBuilder(stringstream &ss, string& line) {
    string BEGIN_CONSENSUS = "BEGIN CONSENSUS";
    string idTitle = "id=";
    string classTitle = "class=";
    string nspecTitle = "nSpec=";
    string sumchargeTitle = "SumCharge=";
    string sumintensTitle = "SumIntens";
    string summzTitle = "SumMz=";
    string END_CONSENSUS= "END CONSENSUS";

    line = line.substr(BEGIN_CONSENSUS.length());

    vector<string> headerFields;
    Functions::split(line," ",headerFields);

    string id,className;
    int nSpec,sumCharge;
    float sumPrecIntens, sumPrecMz;

    if (headerFields[0].substr(0,idTitle.length()) != idTitle)
        throw("Hedaer field miss id= field");
    else
        id = headerFields[0].substr(idTitle.length());
    if (headerFields[1].substr(0,classTitle.length()) != classTitle)
        throw("Header field miss class= field");
    else
        className = headerFields[1].substr(classTitle.length());
    if (headerFields[2].substr(0,nspecTitle.length()) != nspecTitle)
        throw("Header field miss nSpec= field");
    else
        nSpec = atoi(headerFields[2].substr(nspecTitle.length()).data());
    if (headerFields[3].substr(0,sumchargeTitle.length()) != sumchargeTitle)
        throw("Header field miss sumCharge= field");
    else
        sumCharge = atoi(headerFields[3].substr(sumchargeTitle.length()).data());
    if(headerFields[4].substr(0,sumintensTitle.length()) != sumintensTitle)
        throw("Header field miss sumIntens= field");
    else
        sumPrecIntens = (float)atof(headerFields[4].substr(sumintensTitle.length()).data());
    if(headerFields[5].substr(0,summzTitle.length()) != summzTitle)
        throw("Header field miss sumMz= field");
    else
        sumPrecMz = (float)atof(headerFields[5].substr(summzTitle.length()).data());

    //process the peak list
    list<IPeak*> *peaks = new list<IPeak*>;

    while(getline(ss,line) && line != ""){
        if(line == END_CONSENSUS) break;

        vector<string> peakFields;
        Functions::split(line,"\t",peakFields);
        if(peakFields.size() != 3)
            throw("Invalid consensus peak definition encountered: ",line);
        float mz = (float)atof(peakFields[0].data());
        float intens = (float)atof(peakFields[1].data());
        int count = atoi(peakFields[3].data());

        Peak *peak = new Peak(mz,intens,count);
        peaks->push_back(peak);
    }

    //build the object
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;
    if(className == "uk.ac.ebi.pride.spectracluster.consensus.ConsensusSpectrum")
        consensusSpectrumBuilder = new ConsensusSpectrum(id,nSpec,sumPrecMz,sumPrecIntens,sumCharge,*peaks);

//      not needed in hadoop, the default ConsensusSpectrumBuilder
//                      is ConsensusSpectrum
//    else if (className == "uk.ac.ebi.pride.spectracluster.consensus.GreedyConsensusSpectrum")
//        consensusSpectrumBuilder = new GreedyConsensusSpectrum();
    else
        throw("Cannot recover consensus spectrum of class",className);
    delete peaks;
    return consensusSpectrumBuilder;
}

const int MAX_NUMBER_BAD_MGF_LINES = 2000;
int gNumberBadMGFLines = 0;

//void ParserUtilities::handleTitleLine(ISpectrum spectrum, string titleLine) {
//    string tl = titleLine.substr(titleTile.length());
//}

void ParserUtilities::handleBadMGFData(string& line){
    if(gNumberBadMGFLines++ > MAX_NUMBER_BAD_MGF_LINES){

    }

}
double ParserUtilities::parsePepMassLine(string& pLine) {
    double mass;
    string numeric = pLine.substr(pepmassTitle.length());
    string massStr = numeric.substr(pepmassTitle.length(),numeric.find(" "));
    mass = atof(massStr.data());
    return mass;
}

string ParserUtilities::buildMGFTitle(string line){
    int sequenceIndex = line.find(sequenceTitle);
    int spectrumIdIndex = line.find(titleAndId);

    if(sequenceIndex != string::npos){
        if (spectrumIdIndex!=string::npos) return line.substr(spectrumIdIndex+titleAndId.length(),sequenceIndex);
    }
    else return line.substr(spectrumIdIndex+titleAndId.length());

    return "";
}

string ParserUtilities::idFromClusterLine(string& line) {
    string id = "Id=";
    Functions::replace(line,BEGIN_CLUSTER,"");
    vector<string> split;
    Functions::split(line," ",split);
    for(int index=0;index < split.size();index++){
        if(split[index].substr(0,id.length()) == id) return split[index].substr(id.length());
    }
//    throw exception
}

bool ParserUtilities::storesPeakListFromClusterLine(string& line) {
    string containPeakList = "ContainsPeaklist=";
    Functions::replace(line,BEGIN_CLUSTER,"");
    vector<string> split;
    Functions::split(line," ",split);
    for(int index=0;index < split.size();index++){
        if(split[index].substr(0,containPeakList.length()) == containPeakList){
            return (split[index].substr(containPeakList.length()) != "" ? true:false);
        }
    }
   throw("no ContainsPeaklist= part in ",line);
}

Properties* ParserUtilities::parseProperties(string& line) {
    string propertyTitle = "Properties=";
    line = line.substr(propertyTitle.length());
    vector<string> propertyFields;
    Functions::split(line,"#",propertyFields);
    Properties* properties =new Properties();
    for (int i =0 ;i<propertyFields.size();i++){
        int index = propertyFields[i].find("=");
        if(index == string::npos || index < 1) continue;

        string name = propertyFields[i].substr(0,index);
        string value = propertyFields[i].substr(index+1);
        properties->setProperty(name,value);
    }
    return properties;
}

list<ComparisonMatch>* ParserUtilities::parseComparisonMatches(string line) {
    string comparisonTitle = "ComparisonMatches=";
    string matchedString = line.substr(comparisonTitle.length());
    vector<string> comparisonStrings;
    Functions::split(line,"#",comparisonStrings);
    list<ComparisonMatch> *comparisonMatches = new list<ComparisonMatch>;

    for(int i =0;i < comparisonStrings.size();i++){
        int index = comparisonStrings[i].find(':');
        if(index == string::npos){
            throw("except");
        }
        string similarityString = comparisonStrings[i].substr(0,index);
        string idString = comparisonStrings[i].substr(index+1);

        ComparisonMatch *comparisonMatch = new ComparisonMatch(idString,atof(similarityString.data()));
        comparisonMatches->push_back(*comparisonMatch);
        delete comparisonMatch;
    }
    return comparisonMatches;
}

ParserUtilities::ParserUtilities() {}
