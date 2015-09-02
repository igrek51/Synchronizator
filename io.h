#ifndef IO_H
#define IO_H

#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

class IO {
private:
	static IO* instance;
    IO();
public:
    static IO* geti();
    static IO* i();
    //  LOG
    void clear_log();
    void delete_log();
    void log(string l);
    void log(int l);
    void log(string s, int l);
    void error(string l, bool show_output = true);
    void critical_error(string l);
    //  ECHO
    string last_echo;
    int repeated_echo;
    void echo(string s);
    void echo(int e);
    //  Message Box
    void message_box(string title, string message = "");
};

string get_time();
void ss_clear(stringstream &sstream);

#endif
