//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_PROPERTIES_H
#define CSPECTRA_CLUSTER_PROPERTIES_H
#include "string"
#include "map"
#include "set"

using namespace std;
class Properties {
    public:

        Properties(){};

        string getProperty(string key);

        void setProperty(string& key,string& value);

        map<string,string> getMap();

        set<string> getKeySet();

        void clear();

        void putAll(Properties& props);

        void remove(string key);

        ~Properties();


private:

        map<string,string>* propertyMap  = new map<string,string>;

        set<string>* keyName = new set<string>;


};


#endif //CSPECTRA_CLUSTER_PROPERTIES_H
