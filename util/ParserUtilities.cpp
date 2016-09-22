//
// Created by SangZhe on 16/8/24.
//

#include "ParserUtilities.h"

string ParserUtilities::BEGIN_IONS = "BEGIN IONS";
 string ParserUtilities::END_IONS = "END IONS";
 string ParserUtilities::BEGIN_CLUSTER = "BEGIN CLUSTER";
 string ParserUtilities::END_CLUSTER = "END CLUSTER";

 string ParserUtilities::titleTile = "TITLE=";
 string ParserUtilities::sequenceTitle = ",sequence=";
 string ParserUtilities::titleAndId = "TITLE=id=";
 string ParserUtilities::pepmassTitle = "PEPMASS=";
 string ParserUtilities::chargeTitle = "CHARGE=";
 string ParserUtilities::rtinsecondsTitle = "RTINSECONDS=";
 string ParserUtilities::taxonomyTitle = "TAXONOMY=";
 string ParserUtilities::taxonTitle = "TAXON=";
 string ParserUtilities::user02Title = "USER02=";
 string ParserUtilities::user03Title = "USER03=";

 string ParserUtilities::NOT_HANDLED_MGF_TAGS[19] = {
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
                                 "SEQ=",
                                 "COMP=",
                                 "TAG=",
                                 "ETAG=",
                                 "SCANS=",
                                 "IT_MODS=",
                                 "CLUSTER_SIZE=",
                                 "PRECURSOR_INTENSITY="};


string ParserUtilities::convertClusterToString(ICluster& cluster) {

}

ICluster* ParserUtilities::readSpectralCluster(stringstream& ss, string& line) {
    string currentId = "";
    bool storesPeakLists = false;
    list<ISpectrum*> spectra;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder = nullptr;
    list<ComparisonMatch> comparisonMatches;

    try{
        if(line == "") getline(ss,line);
        while ( line != ""){
            if(line.substr(0,BEGIN_CLUSTER.length()) == BEGIN_CLUSTER){
                currentId = idFromClusterLine(line);
                storesPeakLists =storesPeakListFromClusterLine(line);
                break;
            }
            // naked spectrum
            if(line.substr(0,BEGIN_IONS.length()) == BEGIN_IONS){
                ISpectrum *internalComplete = readMGFScan(ss,line);
                vector<IPeak*> peaks = internalComplete->getPeaks();
                ISpectrum* internalFilterd = new Spectrum(*internalComplete,peaks);
                PointerPool::remove(internalComplete);
                // perform default peak filtering

                ISpectrum *Filterd = Defaults::doDefaultPeakFilter(*internalFilterd);
                delete internalFilterd;
                IConsensusSpectrumBuilder* consensusSpectrumBuilder = Defaults::getDefaultConsensusSpectrumBuilder();
                string Id = Filterd->getId();
                ICluster *ret = new SpectralCluster(Id,consensusSpectrumBuilder);
                PointerPool::add(ret);
                ret->addSpectra(internalFilterd);

                return ret;
            }
            getline(ss,line);
        }
        if(line == "") return nullptr;

        getline(ss,line);

        Properties properties;
        string propertyTitle = "Properties=";
        string comparisonTitle = "ComparisonMatched=";
        string BEGIN_CONSENSUS = "BEGIN CONSENSUS";
        // check if properties are stored
        if(line.substr(0,propertyTitle.length()) == propertyTitle){
            properties = parseProperties(line);
            getline(ss,line);
        }
        // check if comparison matches were stored
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
            if(internal != nullptr)
                spectra.push_back(internal);
            getline(ss,line);
            if(line == "")
                return nullptr;
            if(line.substr(0,END_CLUSTER.length()) == END_CLUSTER){
                ICluster* ret;
                if(storesPeakLists){
                    IConsensusSpectrumBuilder* consensusSpectrumBuilder = Defaults::getDefaultConsensusSpectrumBuilder();
                    ret = new SpectralCluster(currentId,consensusSpectrumBuilder);
                    PointerPool::add(ret);
                    ret->addSpectra(spectra);
                }
                else{
//                    ToDO add GreedySpectralCluster
                    ret = new GreedySpectralCluster(currentId, spectra, (GreedyConsensusSpectrum*)consensusSpectrumBuilder, comparisonMatches);
                }

                unordered_set<string>::iterator iter;
                int i =0;
                unordered_set<string> keys = properties.getKeySet();
                if(keys.size() != 0) {
                    for (iter = keys.begin(); iter != keys.end(); iter++) {
                        string name = *iter;
                        ret->setProperty(name, properties.getProperty(name));
                    }
                }
                return ret;
            }
        }


    }catch(exception){
        throw("runtime_error");
    }
    return nullptr;




}

list<ICluster*> ParserUtilities::readSpectralCluster(string& clusterString) {
    list<ICluster*> holder ;
    stringstream ss;
    ss << clusterString;
    string line ="";
    ICluster* cls =  readSpectralCluster(ss,line);
    while (cls != nullptr){
        holder.push_back(cls);
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


    Spectrum ret;

    Properties props;

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
        if (line == "") return nullptr;

        vector<IPeak*> holder;

        while (line != ""){
//            getline(ss,line);

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
                    size_t index = line.find(sequenceTitle);
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

                    line = line.substr(chargeTitle.length()); //strip the "+" or "-" behind the value
                    line = line.substr(0,line.length()-1);
                    if(line.find(".")){
                        dcharge = (int)(0.5 + IOUtilities::StringToFloat(line));
                    }
                    else{
                        dcharge = IOUtilities::StringToInt(line);
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
                for(int i = 0;i < 19;i++){
                    string notHandledMgfTag = ParserUtilities::NOT_HANDLED_MGF_TAGS[i];
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

                holder.sort(Peak::cmpPeak);

                IQualityScorer* defaultQualityScorer = Defaults::getDefaultQualityScorer();
                ISpectrum *spectrum = new Spectrum(title,dcharge,(float)mz,defaultQualityScorer,holder);

                ISpectrum *spectrum1 = Defaults::doDefaultPeakFilter(*spectrum);

                delete spectrum;

                spectrum = spectrum1;

                unordered_set<string> keys = props.getKeySet();
                if(keys.size() != 0) {
                    unordered_set<string>::iterator keysIter;
                    for (keysIter = keys.begin(); keysIter != keys.end(); keysIter++) {
                        spectrum->setProperty(*keysIter, props.getProperty(*keysIter));
                    }
                }
                props.clear();

                if (species != "")
                    spectrum->setProperty(KnownProperties::TAXONOMY_KEY,species);
                if (peptide != "")
                    spectrum->setProperty(KnownProperties::IDENTIFIED_PEPTIDE_KEY, peptide);
                if (peptide != "")
                    spectrum->setProperty(KnownProperties::ANNOTATION_KEY, title);
                if (protein != "")
                    spectrum->setProperty(KnownProperties::PROTEIN_KEY, protein);
                if (modifications != "")
                    spectrum->setProperty(KnownProperties::MODIFICATION_KEY, modifications);
                if (titleLine != "")
//                    handleTitleLine(spectrum, titleLine);
                return spectrum;
            }
            else {
                IOUtilities::replace(line,"\t", " ");
                vector<string> items;
                IOUtilities::split(line," ",items);
                if(items.size() >= 2){
                    try{
                        float peakMass = IOUtilities::StringToFloat(items[0]);
                        float peakIntensity = IOUtilities::StringToFloat(items[1]);
                        IPeak *added =new Peak(peakMass,peakIntensity);
                        holder.push_back(added);
                        PointerPool::add(added);
                    }catch(exception s){
//                        handleBadMGFData(line);
                    }
                }else{
//                    handleBadMGFData(line);
                }
                getline(ss,line);

            }

        }
    return nullptr;
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
    IOUtilities::split(line," ",headerFields);

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
        nSpec = IOUtilities::StringToInt(headerFields[2].substr(nspecTitle.length()).data());
    if (headerFields[3].substr(0,sumchargeTitle.length()) != sumchargeTitle)
        throw("Header field miss sumCharge= field");
    else
        sumCharge = IOUtilities::StringToInt(headerFields[3].substr(sumchargeTitle.length()).data());
    if(headerFields[4].substr(0,sumintensTitle.length()) != sumintensTitle)
        throw("Header field miss sumIntens= field");
    else
        sumPrecIntens = IOUtilities::StringToFloat(headerFields[4].substr(sumintensTitle.length()).data());
    if(headerFields[5].substr(0,summzTitle.length()) != summzTitle)
        throw("Header field miss sumMz= field");
    else
        sumPrecMz = IOUtilities::StringToFloat(headerFields[5].substr(summzTitle.length()));

    //process the peak list
    vector<IPeak*> peaks;

    while(getline(ss,line) && line != ""){
        if(line == END_CONSENSUS) break;

        vector<string> peakFields;
        IOUtilities::split(line,"\t",peakFields);
        if(peakFields.size() != 3)
            throw("Invalid consensus peak definition encountered: ",line);
        float mz = IOUtilities::StringToFloat(peakFields[0]);
        float intens = IOUtilities::StringToFloat(peakFields[1]);
        int count = IOUtilities::StringToInt(peakFields[3]);

        IPeak *peak =new Peak(mz,intens,count);
        peaks.push_back(peak);
        PointerPool::add(peak);
    }

    //build the object
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;
    if(className == "uk.ac.ebi.pride.spectracluster.consensus.ConsensusSpectrum")
        consensusSpectrumBuilder = new ConsensusSpectrum(id,nSpec,sumPrecMz,sumPrecIntens,sumCharge,peaks);

    else if (className == "uk.ac.ebi.pride.spectracluster.consensus.GreedyConsensusSpectrum")
        consensusSpectrumBuilder = new GreedyConsensusSpectrum(Defaults::getFragmentIonTolerance(), id, nSpec, sumPrecMz, sumPrecIntens, sumCharge, peaks);
    else
        throw("Cannot recover consensus spectrum of class",className);
    return consensusSpectrumBuilder;
}

int ParserUtilities::MAX_NUMBER_BAD_MGF_LINES = 2000;
int ParserUtilities::gNumberBadMGFLines = 0;

//void ParserUtilities::handleTitleLine(ISpectrum spectrum, string titleLine) {
//    string tl = titleLine.substr(titleTile.length());
//}

void ParserUtilities::handleBadMGFData(string& line){
    if(gNumberBadMGFLines++ > MAX_NUMBER_BAD_MGF_LINES){

    }

}
double ParserUtilities::parsePepMassLine(string& pLine) {
    double mass;
    string massStr;
    string numeric = pLine.substr(pepmassTitle.length());
    if(numeric.find(" ") != string::npos) {
         massStr = numeric.substr(0, numeric.find(" "));
    }
    else {
         massStr = numeric.substr(0,pepmassTitle.length());
    }
    mass = IOUtilities::StringToFloat(massStr);
    return mass;
}

string ParserUtilities::buildMGFTitle(string line){
    size_t sequenceIndex = line.find(sequenceTitle);
    size_t spectrumIdIndex = line.find(titleAndId);

    if(sequenceIndex != string::npos){
        if (spectrumIdIndex!=string::npos) return line.substr(spectrumIdIndex+titleAndId.length(),sequenceIndex);
    }
    else return line.substr(spectrumIdIndex+titleAndId.length());

    return "";
}

string ParserUtilities::idFromClusterLine(string& line) {
    string id = "Id=";
    IOUtilities::replace(line,BEGIN_CLUSTER,"");
    vector<string> split;
    IOUtilities::split(line," ",split);
    for(int index=0;index < split.size();index++){
        if(split[index].substr(0,id.length()) == id) return split[index].substr(id.length());
    }
//    throw exception
}

bool ParserUtilities::storesPeakListFromClusterLine(string& line) {
    string containPeakList = "ContainsPeaklist=";
    IOUtilities::replace(line,BEGIN_CLUSTER,"");
    vector<string> split;
    IOUtilities::split(line," ",split);
    for(int index=0;index < split.size();index++){
        if(split[index].substr(0,containPeakList.length()) == containPeakList){
            return (split[index].substr(containPeakList.length()) != "" ? true:false);
        }
    }
   throw("no ContainsPeaklist= part in ",line);
}

Properties ParserUtilities::parseProperties(string& line) {
    string propertyTitle = "Properties=";
    line = line.substr(propertyTitle.length());
    vector<string> propertyFields;
    IOUtilities::split(line,"#",propertyFields);
    Properties properties;
    for (int i =0 ;i<propertyFields.size();i++){
        size_t index = propertyFields[i].find("=");
        if(index == string::npos || index < 1) continue;

        string name = propertyFields[i].substr(0,index);
        string value = propertyFields[i].substr(index+1);
        properties.setProperty(name,value);
    }
    return properties;
}

list<ComparisonMatch> ParserUtilities::parseComparisonMatches(string line) {
    string comparisonTitle = "ComparisonMatches=";
    string matchedString = line.substr(comparisonTitle.length());
    vector<string> comparisonStrings;
    IOUtilities::split(line,"#",comparisonStrings);
    list<ComparisonMatch> comparisonMatches;

    for(int i =0;i < comparisonStrings.size();i++){
        size_t index = comparisonStrings[i].find(':');
        if(index == string::npos){
            throw("except");
        }
        string similarityString = comparisonStrings[i].substr(0,index);
        string idString = comparisonStrings[i].substr(index+1);

        ComparisonMatch comparisonMatch = ComparisonMatch(idString,IOUtilities::StringToFloat(similarityString));
        comparisonMatches.push_back(comparisonMatch);
    }
    return comparisonMatches;
}

ParserUtilities::ParserUtilities() {}

ParserUtilities::~ParserUtilities() {
}
