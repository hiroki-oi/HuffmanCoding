#ifndef HuffmanEncoder_hpp
#define HuffmanEncoder_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HuffmanTreeNode.hpp"

class HuffmanEncoder
{
private:

	std::vector<int> symFreq;
    int cntNonZeroSym;

	int lastHeapIdx;
	std::vector<HuffmanTreeNode*> heap; // Bynary heap
	HuffmanTreeNode *newNode;
	HuffmanTreeNode *rootNode;
	HuffmanTreeNode *first;
	HuffmanTreeNode *second;
	HuffmanTreeNode *mergeOfTwoChild;

    std::vector<std::string> symCodeWord;
	std::string huffmanCode;
	int codeWordLen;

	int totalNumOfSym;

	std::vector<unsigned char> bitBuf;
	int bitBufIdx;
	int bitShiftCnt;
	unsigned char bitShiftVal;

	int locaOfTotalBitNum;
	int totalNumOfBit;


	void cntSymFreq(const std::vector<Symbol> &symData);
	void cntNZSym();

	int getHighPriorityChildIdx(int index);

	int getParentIdx(int index);
	int getLeftChildIdx(int index);
	int getRightChildIdx(int index);

	bool chkEmptyHeapMem();

	void upHeap(HuffmanTreeNode *newNode);
	HuffmanTreeNode* downHeap();

	void alloHeapMem();
	void alloNewNode();

	void huffmanTree();

	void affixToCodeWord(HuffmanTreeNode *rootNode, char codeWord);
	
	void prtHuffCodeTbl();

	void recHuffCodeTbl(std::ofstream &fout);

	void bitStream(const std::vector<Symbol> &symData, std::ofstream &fout);

	int judgement(char oneORzero);

	void bitUnitOper(std::string huffmanCode, std::ofstream &fout);

	void releaseNewNodeMem(HuffmanTreeNode *rootNode);
	void releaseAlloMem();

public:

	HuffmanEncoder();
	~HuffmanEncoder();
	void performEncoding(const std::vector<Symbol> &symData, std::ofstream &fout);
};

#endif /* HuffmanEncoder_hpp */