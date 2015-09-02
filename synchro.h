#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "task.h"

using namespace std;

class SynchroPath {
    SynchroPath(string source, string dest, bool content_check);
    string source;
    string dest;
    bool content_check;
};

string* get_drives(int &nr);
string list_drives();
bool files_cmp(string file1, string file2);

#endif
