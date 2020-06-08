#ifndef HuffmanTreeNode_hpp
#define HuffmanTreeNode_hpp

constexpr auto MAX = 65536;
constexpr auto BUF_SIZE = 1000;
constexpr auto NUM = 100;

struct HuffmanTreeNode
{
	unsigned short symbol;
	int frequency;

	HuffmanTreeNode *left;
	HuffmanTreeNode *right;
};

#endif /* HuffmanTreeNode_hpp */