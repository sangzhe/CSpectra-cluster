//
// Created by SangZhe on 16/8/24.
//

#include "Properties.h"


void Properties::setProperty(string &key, string &value) {
    propertyMap.insert(map<string,string>::value_type(key,value));
}



string Properties::getProperty(string key) {
    string ret;
    try{
        ret = propertyMap.at(key);
    }catch(out_of_range){
        ret = "";
    }
    return ret;
}

unordered_map<string,string> Properties::getMap() {
    return propertyMap;
}

unordered_set<string> Properties::getKeySet() {
    unordered_map<string,string>::iterator iter_1;
    if (keyName.size() == 0 ){
        for(iter_1=propertyMap.begin();iter_1 != propertyMap.end();iter_1++){
            keyName.insert(iter_1->first);
        }

    }
    return keyName;
}

void Properties::putAll(Properties &props) {
    unordered_set<string>::iterator iter;
    unordered_set<string> keys = props.getKeySet();
    for(iter = keys.begin();iter != keys.end();iter++){
        string name = *iter;
        propertyMap.insert(pair<string,string>(name,props.getProperty(name)));
    }
}

void Properties::remove(string key) {
    unordered_map<string,string>::iterator iter = propertyMap.find(key);
    if(iter != propertyMap.end()){
        propertyMap.erase(key);
    }
}

void Properties::clear() {
    propertyMap.clear();
    keyName.clear();
}

Properties& Properties::operator=(Properties O) {
    propertyMap = O.getMap();
    keyName = O.getKeySet();
}





