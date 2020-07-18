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
ostringstream oMsg;

int main(int argc, char *argv[])
{
    int maxBound = 1000000;
    if (argc > 1)
        maxBound = atoi(argv[1]);
    string kmer;
    int count;
    while (cin >> kmer)
    {
        cin >> count;
        if (count > maxBound)
            count = maxBound;
        for (int i = 0; i < count; i++)
        {
            cout << ">read\n" << kmer << endl;
        }
    }
}
