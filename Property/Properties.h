//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_PROPERTIES_H
#define CSPECTRA_CLUSTER_PROPERTIES_H
#include "string"
#include "boost/unordered/unordered_map.hpp"
#include "boost/unordered/unordered_set.hpp"

using namespace boost::unordered;
using namespace std;
class Properties {
    public:

        Properties(){};

        string getProperty(string key);

        void setProperty(string& key,string& value);

        unordered_map<string,string> getMap();

        unordered_set<string> getKeySet();

        void clear();

        void putAll(Properties& props);

        void remove(string key);

        Properties& operator =(Properties O);




private:

    unordered_map<string,string> propertyMap;

    unordered_set<string> keyName;


};


#endif //CSPECTRA_CLUSTER_PROPERTIES_H
