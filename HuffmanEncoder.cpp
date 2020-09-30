#include "HuffmanEncoder.hpp"

using namespace std;

HuffmanEncoder::HuffmanEncoder()
{
	symFreq.resize(MAX, 0);
	cntNonZeroSym = 0;
	
	lastHeapIdx = 0;
	newNode = nullptr;
	rootNode = nullptr;
	first = nullptr;
	second = nullptr;
	mergeOfTwoChild = nullptr;

	symCodeWord.resize(MAX);

	totalNumOfSym = 0;

	bitBuf.resize(BUF_SIZE, 0);
	bitBufIdx = 0;
	bitShiftCnt = 7;
	bitShiftVal = 0;

	locaOfTotalBitNum = 0;
	totalNumOfBit = 0;
}


HuffmanEncoder::~HuffmanEncoder()
{
	if (rootNode->left != nullptr && rootNode->right != nullptr) {
		releaseNewNodeMem(rootNode->left);
		releaseNewNodeMem(rootNode->right);
	}

	releaseAlloMem();
}


void HuffmanEncoder::performEncoding(const vector<Symbol> &symData, ofstream &fout)
{
	cntSymFreq(symData);
	cntNZSym();

	alloHeapMem();
	alloNewNode();
	huffmanTree();

	if (rootNode->left == nullptr || rootNode->right == nullptr) {
		totalNumOfSym++;
		symCodeWord[(unsigned short)rootNode->symbol] = "0";
	}
	else {
		affixToCodeWord(rootNode->left, '0');
		affixToCodeWord(rootNode->right, '1');
	}

	prtHuffCodeTbl();

	recHuffCodeTbl(fout);
	bitStream(symData, fout);
}



void HuffmanEncoder::cntSymFreq(const vector<Symbol> &symData)
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
	if (getLeftChildIdx(index) > lastHeapIdx) {
		return 0;
	} else if (getLeftChildIdx(index) == lastHeapIdx) {
		return getLeftChildIdx(index);
	} else {
		if (heap[getLeftChildIdx(index)]->frequency < heap[getRightChildIdx(index)]->frequency) {
			return getLeftChildIdx(index);
		} else {
			return getRightChildIdx(index);
		}	
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


bool HuffmanEncoder::chkEmptyHeapMem()
{
	if (lastHeapIdx == 0) return true;
	else return false;
}


void HuffmanEncoder::upHeap(HuffmanTreeNode *newNode)
{
	int index = lastHeapIdx + 1;
	
	// Heap is 1-origin
	while (index != 1) {
		if (index > cntNonZeroSym + 1) {
			cout << "Try to refer over the heap Memory, program is terminated !!" << endl;
			exit(EXIT_FAILURE);
		}

		if (newNode->frequency < heap[getParentIdx(index)]->frequency) {
			heap[index] = heap[getParentIdx(index)];
			index = getParentIdx(index);
		} else {
			break;
		}
	}

	heap[index] = newNode;

	lastHeapIdx++;
}


HuffmanTreeNode* HuffmanEncoder::downHeap()
{
	if (chkEmptyHeapMem()) {
		cout << "Complete to pop from heap : Heap is Empty" << endl;
		return nullptr;
	}

	int parentIdx = 1;
	int childIdx;

	HuffmanTreeNode *returnRoot = heap[parentIdx];
	HuffmanTreeNode *lastNode = heap[lastHeapIdx];

	while ((childIdx = getHighPriorityChildIdx(parentIdx))) {
		if (lastNode->frequency <= heap[childIdx]->frequency) break;

		heap[parentIdx] = heap[childIdx];
		parentIdx = childIdx;
	}

	heap[parentIdx] = lastNode;
	lastHeapIdx--;

	return returnRoot;
}


void HuffmanEncoder::alloHeapMem()
{
    heap.resize(cntNonZeroSym + 1);
	
	cout << "------------------------------------------------------> Heap mem & node to heap" << endl;
	cout << "Complete to allocate the heap memory(NODE)" << endl;
}


void HuffmanEncoder::alloNewNode()
{
	for (int i = 0; i < MAX; i++) {
		if (symFreq[i] > 0) {
			newNode = new HuffmanTreeNode;

			if (newNode == nullptr) {
				cout << "Failed to allocate newnode(NODE) !!" << endl;
				exit(EXIT_FAILURE);
			} else {
				newNode->symbol = (Symbol)i;
				newNode->frequency = symFreq[i];
				newNode->left = nullptr;
				newNode->right = nullptr;

				upHeap(newNode);
			}
		}
	}

	cout << "Complete to stack a new node at heap" << endl;
}


void HuffmanEncoder::huffmanTree()
{
	while (!chkEmptyHeapMem()) {
		first = downHeap();                         
		second = downHeap();

		if (second == nullptr) {
			cout << "Complete to build huffman tree" << endl << endl << endl;
			break;
		}

		mergeOfTwoChild = new HuffmanTreeNode;

		if (mergeOfTwoChild == nullptr) {
			cout << "Failed to allocate mergeOfTwoChild memory(NODE)" << endl;
			exit(EXIT_FAILURE);
		}
		
		// Making trees
		mergeOfTwoChild->symbol = 0; // Symbol has no meaning
		mergeOfTwoChild->frequency = first->frequency + second->frequency;
		mergeOfTwoChild->left = first;
		mergeOfTwoChild->right = second;

		upHeap(mergeOfTwoChild);
	}

	rootNode = first;
}


void HuffmanEncoder::affixToCodeWord(HuffmanTreeNode *rootNode, char codeWord)
{
	huffmanCode += codeWord;                     

	if (rootNode->left == nullptr && rootNode->right == nullptr) {
		totalNumOfSym++;
		symCodeWord[(unsigned short)rootNode->symbol] = huffmanCode;
	} else {
		affixToCodeWord(rootNode->left, '0');
		affixToCodeWord(rootNode->right, '1');
	}

	huffmanCode.pop_back();

	return;
}


void HuffmanEncoder::prtHuffCodeTbl()
{
	cout << "------------------------------------------------------> Build huffman table" << endl;
	
	if (cntNonZeroSym == totalNumOfSym) {
		cout << "Complete to build the huffman code table" << endl << endl << endl;
	} else {
		cout << "Failed to build the huffman code table !!" << endl;
	}
}


void HuffmanEncoder::recHuffCodeTbl(ofstream &fout)
{
	fout.write(reinterpret_cast<char *>(&cntNonZeroSym), sizeof(cntNonZeroSym));

	for(int i = 0; i < MAX; i++) {
		if ((int)symCodeWord[i].size() > 0) {
			Symbol sym = (Symbol)i;                                                          
			unsigned char len = (unsigned char)symCodeWord[i].size();         

			fout.write(reinterpret_cast<char *>(&sym), sizeof(sym));
			fout.write(reinterpret_cast<char *>(&len), sizeof(len));
			
			// 1bitずつにできそう
			fout.write(symCodeWord[i].c_str(), sizeof(char) * (int)symCodeWord[i].size());
		}
	}

	cout << "------------------------------------------------------> Bitstream" << endl;
	cout << "Complete to write the huffman code table" << endl;
}


void HuffmanEncoder::bitStream(const vector<Symbol> &symData, ofstream &fout)
{
	locaOfTotalBitNum = (int)fout.tellp();

	fout.seekp(sizeof(int), ios::cur);

	for (int i = 0; i < (int)symData.size(); i++) {
		huffmanCode = symCodeWord[symData[i]];
		bitUnitOper(huffmanCode, fout);
	}

	fout.write(reinterpret_cast<char *>(&bitBuf[0]), sizeof(unsigned char) * (bitBufIdx + 1));
	
	fout.seekp(locaOfTotalBitNum, ios::beg);
	fout.write(reinterpret_cast<char *>(&totalNumOfBit), sizeof(totalNumOfBit));
	fout.seekp(0, ios::end);

	cout << "Complete to build bitstream" << endl << endl << endl;
}


int HuffmanEncoder::ctoi(char oneORzero)
{
	if (oneORzero == '0') {
		return 0;
	}                 
	if (oneORzero == '1')  {
		return 1;
	}
	
	cout << "---------- CRITICAL ERROR : INVALID Huffman Encode Table ----------" << endl;
	exit(EXIT_FAILURE);
}


void HuffmanEncoder::bitUnitOper(string huffmanCode, ofstream &fout)
{
	for (unsigned int i = 0; i < (int)huffmanCode.size(); i++) {
		bitShiftVal = ctoi(huffmanCode[i]);
		bitShiftVal = bitShiftVal << bitShiftCnt;

		bitBuf[bitBufIdx] |= bitShiftVal;            
		bitShiftCnt -= 1;
		totalNumOfBit += 1;

		if (bitShiftCnt < 0) {
			bitShiftCnt = 7;
			bitBufIdx += 1;

			if ((int)bitBuf.size() <= bitBufIdx) {
				fout.write(reinterpret_cast<char *>(&bitBuf[0]), sizeof(unsigned char) * (int)bitBuf.size());
				
				for(int j = 0; j < (int)bitBuf.size(); j++) bitBuf[j] = 0;
				bitBufIdx = 0;
			}
		}
	}
}


void HuffmanEncoder::releaseNewNodeMem(HuffmanTreeNode *rootNode)
{
	if (rootNode->left == nullptr && rootNode->right == nullptr) {
		delete(rootNode);
	} else {
		releaseNewNodeMem(rootNode->left);
		rootNode->left = nullptr;

		releaseNewNodeMem(rootNode->right);
		rootNode->right = nullptr;
	}

	return;
}


void HuffmanEncoder::releaseAlloMem()
{
	delete(rootNode);

	cout << "------------------------------------------------------> Release memory" << endl;
	cout << "Complete to release the allocated memory" << endl << endl;
}