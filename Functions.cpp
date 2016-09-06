//
// Created by SangZhe on 16/8/30.
//

#include "Functions.h"

void  Functions::replace(string& string1,const string &Old, const string &New){
    size_t pos=0;
    while(true){
        pos = string1.find(Old,pos);
        if(pos == string::npos) break;
        string1.replace(pos, New.length(),New);
        pos += New.length();
    }
}

void Functions::split(const string& src, const string& separator, vector<string>& dest)
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

int Functions::compare(const float A, const float B) {
    const float EPSINON = 0.00001;
    if ( A < B ) return -1;
    if ( A > B ) return 1;
    if(fabs( A - B ) < EPSINON ) return 0;

}

string Functions::FloatToString(const float &Num,const string& accuracy) {
    char output[50];
    snprintf(output,50,accuracy.data(),Num);
    string ret = string(output);
    return ret;

}

string Functions::IntToString(const int &Num, const string &accuracy) {
    char output[50];
    snprintf(output,50,accuracy.data(),Num);
    string ret = string(output);
    return ret;
}

