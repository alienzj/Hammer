#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
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
string sbuf;

#include "defs.h"
#include "union.h"

int kmersize = 0;
char *reads;

struct Kmer
{
    long loc;
    int mult;
    float freq;
};

int comp(const void *k1, const void *k2)
{
    long x = ((Kmer *)k1)->loc;
    long y = ((Kmer *)k2)->loc;

    if (x == y)
        return 0;
    bool flipx = false; // are we revcomping x?
    bool flipy = false;
    bool detx = false; // have we determine whether we are revcomping x?
    bool dety = false;
    for (int i = 0; i < kmersize; i++)
    {
        char xchar, ychar;
        if (detx && !flipx)
        {
            xchar = reads[x + i];
        }
        else if (detx && flipx)
        {
            xchar = revcomp(reads[x + kmersize - i - 1]);
        }
        else
        { //! detx
            if (reads[x + i] == revcomp(reads[x + kmersize - i - 1]))
            {
                xchar = reads[x + i];
            }
            else
            {
                detx = true;
                flipx = (reads[x + i] > revcomp(reads[x + kmersize - i - 1]));
                if (flipx)
                {
                    xchar = revcomp(reads[x + kmersize - i - 1]);
                }
                else
                {
                    xchar = reads[x + i];
                }
            }
        }
        if (dety && !flipy)
        {
            ychar = reads[y + i];
        }
        else if (dety && flipy)
        {
            ychar = revcomp(reads[y + kmersize - i - 1]);
        }
        else
        { //! dety
            if (reads[y + i] == revcomp(reads[y + kmersize - i - 1]))
            {
                ychar = reads[y + i];
            }
            else
            {
                dety = true;
                flipy = (reads[y + i] > revcomp(reads[y + kmersize - i - 1]));
                if (flipy)
                {
                    ychar = revcomp(reads[y + kmersize - i - 1]);
                }
                else
                {
                    ychar = reads[y + i];
                }
            }
        }

        if (xchar < ychar)
            return -1;
        if (xchar > ychar)
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    kmersize = atoi(argv[1]);
    long readslim = atol(argv[2]);
    long totlim = atol(argv[3]);

    cerr << "Reading in prepro file...\n";
    long maxKmers = (totlim - readslim) / sizeof(Kmer);
    cerr << "maxKmers = " << add_commas(maxKmers) << endl;
    ;
    Kmer *indices = new Kmer[maxKmers];
    reads = new char[readslim];
    vector<string> row;
    long counter = 0;
    Kmer kmer;
    long counter2 = 0;
    long indicesSize = 0;
    while (get_row_whitespace(cin, row))
    {
        counter2++;
        memcpy(&reads[counter], row[0].data(), row[0].length());
        for (int i = 0; i < max(0, (int)row[0].length() - kmersize + 1); i++)
        { // don't know why the max is necessary, but it wouldn't work
          // otherwise
            kmer.loc = counter + i;
            kmer.mult = 1;
            kmer.freq = atof(row[1].c_str());
            if (indicesSize > maxKmers)
            {
                cerr << "Out of memory, after " << counter2
                     << " lines of your reads.prepro.\n";
                exit(1);
            }
            indices[indicesSize] = kmer;
            indicesSize++;
        }
        counter += row[0].length();
    }

    cerr << "Sorting kmers...\n";
    qsort(indices, indicesSize, sizeof(Kmer), comp);

    cerr << "Removing duplicates...\n";
    long upos = 0;
    for (long i = 1; i < indicesSize; i++)
    {
        if (0 == comp(&indices[upos], &indices[i]))
        {
            indices[upos].mult += indices[i].mult;
            indices[upos].freq += indices[i].freq;
        }
        else
        {
            upos++;
            indices[upos] = indices[i];
        }
    }
    indicesSize = upos + 1;

    cerr << "Ouputing kmers file...\n";
    for (long i = 0; i < indicesSize; i++)
    {
        string seq = "";
        for (int j = 0; j < kmersize; j++)
        {
            seq += reads[indices[i].loc + j];
        }
        assert(indices[i].mult < 100000);
        assert(indices[i].freq < 100000);
        printf("%s %5i %8.2f\n", rcnorm(seq).c_str(), indices[i].mult,
               indices[i].freq);
    }
    delete indices;
}
