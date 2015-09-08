#include "strings.h"
#include <ctime>

string trim_spaces(string s){
    //obcięcie spacji na końcu
    while(s.length() > 0  && s[s.length()-1] == ' '){
        s = s.substr(0, s.length()-1);
    }
    //obcięcie spacji na początku
    while(s.length() > 0  && s[0] == ' '){
        s = s.substr(1);
    }
    return s;
}

string trim_quotes(string s){
    if(s.length()>=3){
        //jeśli cudzysłowy są na poczatku i na końcu
        if(s[0]=='\"' && s[s.length()-1]=='\"'){
            //jeśli w całym stringu znajdują się tylko 2 cudzysłowy
            if(s.substr(1, s.length()-2).find('\"')==string::npos){
                s = s.substr(1, s.length()-2); //usuń je
            }
        }
    }
    return s;
}

char to_lowercase(char in){
	if(in>='A' && in<='Z') return in-'A'+'a';
	if(in=='Ą') return 'ą';
	if(in=='Ż') return 'ż';
	if(in=='Ś') return 'ś';
	if(in=='Ź') return 'ź';
	if(in=='Ę') return 'ę';
	if(in=='Ć') return 'ć';
	if(in=='Ń') return 'ń';
	if(in=='Ó') return 'ó';
	if(in=='Ł') return 'ł';
	return in;
}

bool string_compare_lower(string s1, string s2){
	if(s1.length()!=s1.length()) return false;
	for(unsigned int i=0; i<s1.length(); i++){
		if(to_lowercase(s1[i])!=to_lowercase(s2[i])) return false;
	}
	return true;
}

bool string_begins(string s1, string s2){
    if(s1.length() < s2.length()) return false;
    for(unsigned int i=0; i<s2.length(); i++){
        if(s1.at(i) != s2.at(i)) return false;
    }
    return true;
}

void ss_clear(stringstream &sstream){
	sstream.str("");
	sstream.clear();
}

string get_time(){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream ss;
    if(timeinfo->tm_hour<10) ss<<"0";
    ss<<timeinfo->tm_hour<<":";
    if(timeinfo->tm_min<10) ss<<"0";
    ss<<timeinfo->tm_min<<":";
    if(timeinfo->tm_sec<10) ss<<"0";
    ss<<timeinfo->tm_sec<<", ";
    if(timeinfo->tm_mday<10) ss<<"0";
    ss<<timeinfo->tm_mday<<".";
    if(timeinfo->tm_mon+1<10) ss<<"0";
    ss<<timeinfo->tm_mon+1<<".";
    ss<<timeinfo->tm_year+1900;
    return ss.str();
}
