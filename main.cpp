#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HuffmanEncoder.hpp"

using namespace std;

int main(void)
{
    vector<unsigned short> E{1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    ofstream fout;
    string filename = "output.encoded";
    
    fout.open(filename, ios::binary);
    if(fout.fail()) {
		cout << "Failed to open FILE" << endl;
		exit(EXIT_FAILURE);
	}
    //string a("011");
    
    //fout.write(reinterpret_cast<char *>(&a[0]), sizeof(char)*3);
    HuffmanEncoder e;
    e.performEncoding(E, fout);

    return 0;
}