#ifndef HuffmanEncoder_hpp
#define HuffmanEncoder_hpp

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "HuffmanTreeNode.hpp"

using namespace std;

class HuffmanEncoder
{
private:

	int symFreq[MAX];

	int cntNonZeroSym;

	int lastHeapIdx;

	HuffmanTreeNode **heap;

	HuffmanTreeNode *newNode;

	HuffmanTreeNode *rootNode;

	HuffmanTreeNode *first;
	HuffmanTreeNode *second;

	HuffmanTreeNode *mergeOfTwoChild;

	char *symCodeWord[MAX];
	char *huffmanCode;

	char codeWordBuf[NUM];
	int codeWordLen;

	int totalNumOfSym;

	char recBuf[NUM];

	short sym;
	unsigned char len;

	unsigned char bitBuf[BUF_SIZE];
	int bitBufIdx;
	int bitShiftCnt;
	char bitShiftVal;

	int locaOfTotalBitNum;
	int totalNumOfBit;


	void cntSymFreq(const vector<unsigned short> &symData);
	void cntNZSym();

	int getHighPriorityChildIdx(int index);

	int getParentIdx(int index);
	int getLeftChildIdx(int index);
	int getRightChildIdx(int index);

	int chkEmptyHeapMem();

	void addToHeap(HuffmanTreeNode *newNode);
	HuffmanTreeNode* popToHeap();

	void alloHeapMem();
	void alloNewNode();

	void huffmanTree();

	void affixToCodeWord(HuffmanTreeNode *rootNode, char codeWord);
	
	void prtHuffCodeTbl();

	void recHuffCodeTbl(ofstream &fout);

	void bitStream(const vector<unsigned short> &symData, ofstream &fout);

	int judgement(char *oneORzero);

	void bitUnitOper(char huffmanCode[], ofstream &fout);

	void releaseNewNodeMem(HuffmanTreeNode *rootNode);
	void releaseAlloMem();

public:

	HuffmanEncoder();
	~HuffmanEncoder();
	void performEncoding(const vector<unsigned short> &symData, ofstream &fout);
};

#endif /* HuffmanEncoder_hpp */