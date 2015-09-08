#ifndef STRINGS_H
#define STRINGS_H

#include <iostream>
#include <sstream>

using namespace std;

string trim_spaces(string s);
string trim_quotes(string s);

bool string_compare(string s1, string s2);
bool string_begins(string s1, string s2);

void ss_clear(stringstream &sstream);

string get_time();

#endif
