#ifndef HuffmanDecoder_hpp
#define HuffmanDecoder_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HuffmanTreeNode.hpp"

class HuffmanDecoder
{
private: 

	HuffmanTreeNode *rootNode;
	HuffmanTreeNode *cur;

	int cntNonZeroSym;
	int totalNumOfNode;
	int totalCodeLen;

	short sym;
	unsigned char len;

	char codeBuf[BUF_SIZE];

	unsigned short symBuf[BUF_SIZE];

	int totalNumOfBit;
	int copyTotalNumOfBit;
	int countBit;
	int symBufIdx;

	bool flag;

	void rebuildHuffmanTree(std::ifstream &fin);

	int judgement(char *oneORzero);
	void getLeftChildAddr();
	void getRightChildAddr();
	HuffmanTreeNode* createNewNode();

	void decode(std::vector<unsigned short> &symData, std::ifstream &fin);
	void recData(std::vector<unsigned short> &symData);

public:

	HuffmanDecoder();
	~HuffmanDecoder();
	void performDecoding(std::vector<unsigned short> &symData, std::ifstream &fin);
};

#endif /* HuffmanDecoder_hpp */