#ifndef HuffmanTreeNode_hpp
#define HuffmanTreeNode_hpp

#include <cmath>

#define MAX pow(2, sizeof(Symbol) * 8)
#define BUF_SIZE 1000

typedef unsigned short Symbol;

struct HuffmanTreeNode
{
	Symbol symbol;
	int frequency;

	HuffmanTreeNode *left;
	HuffmanTreeNode *right;
};

#endif /* HuffmanTreeNode_hpp */