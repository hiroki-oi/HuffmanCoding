#include "HuffmanEncoder.hpp"

HuffmanEncoder::HuffmanEncoder()
{
	memset(symFreq, 0, sizeof(symFreq));

	cntNonZeroSym = 0;

	heap = NULL;

	newNode = NULL;

	rootNode = NULL;

	first = NULL;
	second = NULL;

	mergeOfTwoChild = NULL;

	memset(symCodeWord, NULL, sizeof(symCodeWord));
	huffmanCode = NULL;

	memset(codeWordBuf, NULL, sizeof(codeWordBuf));
	codeWordLen = -1;

	totalNumOfSym = 0;

	memset(recBuf, NULL, sizeof(recBuf));

	sym = 0;
	len = NULL;

	memset(bitBuf, NULL, sizeof(bitBuf));
	bitBufIdx = 0;
	bitShiftCnt = 7;
	bitShiftVal = NULL;

	locaOfTotalBitNum = 0;
	totalNumOfBit = 0;
}


HuffmanEncoder::~HuffmanEncoder()
{
	releaseNewNodeMem(rootNode->left);
	releaseNewNodeMem(rootNode->right);

	releaseAlloMem();
}


void HuffmanEncoder::performEncoding(const vector<unsigned short> &symData, ofstream &fout)
{
	cntSymFreq(symData);
	cntNZSym();

	alloHeapMem();
	alloNewNode();
	huffmanTree();

	affixToCodeWord(rootNode->left, '0');             
	affixToCodeWord(rootNode->right, '1');

	prtHuffCodeTbl();

	recHuffCodeTbl(fout);
	bitStream(symData, fout);
}


void HuffmanEncoder::cntSymFreq(const vector<unsigned short> &symData)
{
	for (int i = 0; i < (int)symData.size(); i++) {
		symFreq[symData[i]]++;
	}
}


void HuffmanEncoder::cntNZSym()
{
	for (int i = 0; i < MAX; i++) {
		if (symFreq[i] > 0) {
			cntNonZeroSym++;
		}
	}
}


int HuffmanEncoder::getHighPriorityChildIdx(int index)
{
	if (lastHeapIdx < getLeftChildIdx(index))                       
	{
		return 0;
	}
	else if (getLeftChildIdx(index) == lastHeapIdx)                  
	{
		return getLeftChildIdx(index);
	}
	else                                                                        
	{
		if (heap[getLeftChildIdx(index)]->frequency < heap[getRightChildIdx(index)]->frequency)
			return getLeftChildIdx(index);
		else
			return getRightChildIdx(index);
	}
}


int HuffmanEncoder::getParentIdx(int index)
{
	return index / 2;
}


int HuffmanEncoder::getLeftChildIdx(int index)
{
	return index * 2;
}


int HuffmanEncoder::getRightChildIdx(int index)
{
	return getLeftChildIdx(index) + 1;
}


int HuffmanEncoder::chkEmptyHeapMem()
{
	if (lastHeapIdx == 0)
		return TRUE;
	else
		return FALSE;
}


void HuffmanEncoder::addToHeap(HuffmanTreeNode *newNode)
{
	int index = lastHeapIdx + 1;


	while (index != 1)
	{
		if (cntNonZeroSym + 1 < index)
		{
			puts("Try to refer over the heap Memory, program is terminated !!");

			exit(0);
		}

		if (newNode->frequency < heap[getParentIdx(index)]->frequency)
		{
			heap[index] = heap[getParentIdx(index)];

			index = getParentIdx(index);
		}
		else
			break;
	}


	heap[index] = newNode;

	lastHeapIdx += 1;
}


HuffmanTreeNode* HuffmanEncoder::popToHeap()
{
	if (chkEmptyHeapMem())
	{
		puts("Complete to pop from heap : Heap is Empty");

		return NULL;
	}


	int parentIdx = 1;
	int childIdx;


	HuffmanTreeNode *returnRoot = heap[parentIdx];
	HuffmanTreeNode *lastNode = heap[lastHeapIdx];


	while (childIdx = getHighPriorityChildIdx(parentIdx))
	{
		if (lastNode->frequency <= heap[childIdx]->frequency)
			break;


		heap[parentIdx] = heap[childIdx];

		parentIdx = childIdx;
	}


	heap[parentIdx] = lastNode;

	lastHeapIdx -= 1;


	return returnRoot;
}


void HuffmanEncoder::alloHeapMem()
{
	heap = (HuffmanTreeNode **)calloc(cntNonZeroSym + 1, (sizeof(HuffmanTreeNode *)));   

	if (heap == NULL)
	{
		puts("Fail to allocate the Whole HEAP Memory !!");

		exit(0);
	}

	puts("------------------------------------------------------> Heap mem & node to heap");
	puts("Complete to allocate the heap memory(NODE)");
}


void HuffmanEncoder::alloNewNode()
{
	for (int i = 0; i < MAX; i++)
	{
		if (0 < symFreq[i])
		{
			newNode = (HuffmanTreeNode *)calloc(1, (sizeof(HuffmanTreeNode)));


			if (newNode == NULL)
			{
				puts("Failed to allocate newnode(NODE) !!");

				exit(0);
			}
			else
			{
				newNode->symbol = i;
				newNode->frequency = symFreq[i];
				newNode->left = NULL;
				newNode->right = NULL;


				addToHeap(newNode);
			}
		}
	}


	puts("Complete to stack a new node at heap");
}


void HuffmanEncoder::huffmanTree()
{
	while (!chkEmptyHeapMem())
	{
		first = popToHeap();                         
		second = popToHeap();


		if (second == NULL)
		{
			puts("Complete to build huffman tree");
			puts("");
			puts("");


			break;
		}


		mergeOfTwoChild = (HuffmanTreeNode *)calloc(1, (sizeof(HuffmanTreeNode)));


		if (mergeOfTwoChild == NULL)
		{
			puts("Failed to allocate mergeOfTwoChild memory(NODE)");

			exit(0);
		}


		mergeOfTwoChild->symbol = NULL;
		mergeOfTwoChild->frequency = first->frequency + second->frequency;
		mergeOfTwoChild->left = first;
		mergeOfTwoChild->right = second;


		addToHeap(mergeOfTwoChild);
	}


	rootNode = first;
}


void HuffmanEncoder::affixToCodeWord(HuffmanTreeNode *rootNode, char codeWord)
{
	codeWordLen += 1;
	codeWordBuf[codeWordLen] = codeWord;
	codeWordBuf[codeWordLen + 1] = NULL;                           


	if (rootNode->left == NULL && rootNode->right == NULL)
	{
		huffmanCode = (char *)calloc(strlen(codeWordBuf) + 1, sizeof(char));    

		totalNumOfSym += 1;


		if (huffmanCode == NULL)
		{
			puts("Failed to allocate huffmanCode memory, program is terminated !!");

			exit(0);
		}


		strcpy(huffmanCode, codeWordBuf);

		symCodeWord[(unsigned short)rootNode->symbol] = huffmanCode;
	}
	else
	{
		affixToCodeWord(rootNode->left, '0');
		affixToCodeWord(rootNode->right, '1');
	}


	codeWordBuf[codeWordLen] = NULL;

	codeWordLen -= 1;


	return;
}


void HuffmanEncoder::prtHuffCodeTbl()
{
	puts("------------------------------------------------------> Build huffman table");
	
	if (cntNonZeroSym == totalNumOfSym)
	{
		puts("Complete to build the huffman code table");
		puts("");
		puts("");
	}
	else
		puts("Failed to build the huffman code table !!");
}


void HuffmanEncoder::recHuffCodeTbl(ofstream &fout)
{
	fout.write(reinterpret_cast<char *>(&cntNonZeroSym), sizeof(cntNonZeroSym));

	for(int i = 0; i < MAX; i++)                                                                    
	{
		if (symCodeWord[i] != NULL)
		{
			sym = (short)i;                                                          
			len = (unsigned char)strlen(symCodeWord[i]);               

			strcpy(recBuf, symCodeWord[i]);


			fout.write(reinterpret_cast<char *>(&sym), sizeof(sym));
			fout.write(reinterpret_cast<char *>(&len), sizeof(len));

			fout.write(reinterpret_cast<char *>(&recBuf), sizeof(char) * strlen(symCodeWord[i]));
		}


		sym = 0;
		len = NULL;
		memset(recBuf, NULL, sizeof(recBuf));
	}


	puts("------------------------------------------------------> Bitstream");
	puts("Complete to write the huffman code table");
}


void HuffmanEncoder::bitStream(const vector<unsigned short> &symData, ofstream &fout)
{
	locaOfTotalBitNum = (int)fout.tellp();

	fout.seekp(sizeof(int), ios::cur);

	huffmanCode = NULL;

	for (int i = 0; i < (int)symData.size(); i++) {
		huffmanCode = symCodeWord[symData[i]];
		bitUnitOper(huffmanCode, fout);
	}

	fout.write(reinterpret_cast<char *>(&bitBuf), sizeof(char) * (bitBufIdx + 1));
	
	fout.seekp(locaOfTotalBitNum, ios::beg);
	fout.write(reinterpret_cast<char *>(&totalNumOfBit), sizeof(totalNumOfBit));
	fout.seekp(0, ios::end);

	puts("Complete to build bitstream");
	puts("");
	puts("");
}


int HuffmanEncoder::judgement(char *oneORzero)
{
	if (*oneORzero == '0')                   
		return FALSE;
	else if (*oneORzero == '1')                
		return TRUE;
	else
	{
		puts("---------- CRITICAL ERROR : INVALID Huffman Encode Table ----------");

		exit(1);
	}
}


void HuffmanEncoder::bitUnitOper(char huffmanCode[], ofstream &fout)
{
	for (unsigned int i = 0; i < strlen(huffmanCode); i++)
	{
		bitShiftVal = NULL;

		bitShiftVal = judgement(&huffmanCode[i]);


		bitShiftVal = bitShiftVal << bitShiftCnt;

		bitBuf[bitBufIdx] |= bitShiftVal;            

		bitShiftCnt -= 1;

		totalNumOfBit += 1;


		if (bitShiftCnt < 0)
		{
			bitShiftCnt = 7;

			bitBufIdx += 1;


			if (sizeof(bitBuf) <= bitBufIdx)
			{
				fout.write(reinterpret_cast<char *>(&bitBuf), sizeof(bitBuf));

				memset(bitBuf, NULL, sizeof(bitBuf));

				bitBufIdx = 0;
			}
		}
	}
}


void HuffmanEncoder::releaseNewNodeMem(HuffmanTreeNode *rootNode)
{
	if (rootNode->left == NULL && rootNode->right == NULL)
		free(rootNode);
	else
	{
		releaseNewNodeMem(rootNode->left);

		rootNode->left = NULL;


		releaseNewNodeMem(rootNode->right);

		rootNode->right = NULL;
	}


	return;
}


void HuffmanEncoder::releaseAlloMem()
{
	free(rootNode);
	free(heap);


	puts("------------------------------------------------------> Release memory");
	puts("Complete to release the allocated memory");
	puts("");
}