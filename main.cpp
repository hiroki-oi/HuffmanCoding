#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HuffmanEncoder.hpp"

using namespace std;

int main(void)
{
    vector<unsigned short> E{1, 2, 2, 2, 2, 4, 5, 56, 56, 56, 240, 240, 255, 2, 0};

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

    return 0;
}