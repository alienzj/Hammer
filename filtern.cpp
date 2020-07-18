#include "defs.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{

    string header, seq, junk, qv;
    while (getline(cin, header))
    {
        getline(cin, seq);
        getline(cin, junk);
        getline(cin, qv);
        if (seq.find('N') == string::npos)
            cout << header << endl << seq << endl << junk << endl << qv << endl;
    }
}
