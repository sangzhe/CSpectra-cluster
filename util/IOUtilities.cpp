//
// Created by SangZhe on 16/8/30.
//

#include "IOUtilities.h"

int   IOUtilities::INT_MAX_VALUE = INT_MAX;

float IOUtilities::FLOAT_MAX_VALUE =  FLT_MAX;



void  IOUtilities::replace(string& string1,const string &Old, const string &New){
    replace_all(string1,Old,New);

}

void IOUtilities::split(const string& src, const string& separator, vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do
    {
        index = str.find_first_of(separator,start);
        if (index != string::npos)
        {
            substring = str.substr(start,index-start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
            if (start == string::npos) return;
        }
    }while(index != string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}

int IOUtilities::compare(const float A, const float B) {
    const float EPSINON = 0.00001;
    if ( A < B ) return -1;
    if ( A > B ) return 1;
    if(fabs( A - B ) < EPSINON ) return 0;

}

int IOUtilities::compare(const double A, const double B) {
    const float EPSINON = 0.00000001;
    if ( A < B ) return -1;
    if ( A > B ) return 1;
    if(fabs( A - B ) < EPSINON ) return 0;
}

string IOUtilities::FloatToString(const float &Num,const string& accuracy) {
    char output[50];
    snprintf(output,50,accuracy.data(),Num);
    string ret = string(output);
    return ret;

}

string IOUtilities::IntToString(const int &Num, const string &accuracy) {
    char output[50];
    snprintf(output,50,accuracy.data(),Num);
    string ret = string(output);
    return ret;
}

float IOUtilities::StringToFloat(const string &input) {
    try {
        float num = boost::lexical_cast<float>(input);
        return num;
    }
    catch(boost::bad_lexical_cast & e)
    {
        printf("%s/n", e.what());
    }

}

int IOUtilities::StringToInt(const string &input) {
    try {
        int num = boost::lexical_cast<int>(input);
        return num;
    }
    catch(boost::bad_lexical_cast & e)
    {
        printf("%s/n", e.what());
    }
}

double IOUtilities::StringToDouble(const string &input) {
    try {
        double num = boost::lexical_cast<double>(input);
        return num;
    }
    catch(boost::bad_lexical_cast & e)
    {
        printf("%s/n", e.what());
    }
}

