#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HuffmanEncoder.hpp"
#include "HuffmanDecoder.hpp"

using namespace std;

int main(void)
{
    vector<unsigned short> E{2, 2, 2, 2, 2, 2};

    ofstream fout;
    string filename = "output.encoded";
    
    fout.open(filename, ios::binary);
    if(fout.fail()) {
		cout << "Failed to open FILE" << endl;
		exit(EXIT_FAILURE);
	}
    
    HuffmanEncoder *e = new HuffmanEncoder;
    e->performEncoding(E, fout);
    delete(e);
    unsigned short add = 5;
    fout.write(reinterpret_cast<char *>(&add), sizeof(add));

    ifstream fin;
    fin.open(filename, ios::binary);
    if(fin.fail()) {
		cout << "Failed to open FILE" << endl;
		exit(EXIT_FAILURE);
	}
    
    vector<unsigned short> D;
    HuffmanDecoder *d = new HuffmanDecoder;
    d->performDecoding(D, fin);
    delete(d);

    cout << "D.size(): " << D.size() << endl;
    for(int i = 0; i < (int)D.size(); i++) {
        cout << (int)D[i] << endl;
    }
    return 0;
}