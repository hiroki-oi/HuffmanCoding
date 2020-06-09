#include "HuffmanDecoder.hpp"

using namespace std;

HuffmanDecoder::HuffmanDecoder()
{
	rootNode = nullptr;
	cur = nullptr;

	cntNonZeroSym = 0;
	totalNumOfNode = 0;
	totalCodeLen = 0;

	symBuf.resize(BUF_SIZE, 0);

	totalNumOfBit = 0;
	copyTotalNumOfBit = 0;
	countBit = 0;
	symBufIdx = 0;

	flag = true;
}


HuffmanDecoder::~HuffmanDecoder()
{
}


void HuffmanDecoder::performDecoding(vector<Symbol> &symData, ifstream &fin)
{
	rebuildHuffmanTree(fin);
	decode(symData, fin);
}


void HuffmanDecoder::rebuildHuffmanTree(ifstream &fin)                                  
{
	fin.read(reinterpret_cast<char *>(&cntNonZeroSym), sizeof(cntNonZeroSym));

	rootNode = createNewNode();
	cur = rootNode;

	for (int i = 0; i < cntNonZeroSym; i++) {
		cur = rootNode;
		
		Symbol sym;
		unsigned char len;

		fin.read(reinterpret_cast<char *>(&sym), sizeof(sym));
		fin.read(reinterpret_cast<char *>(&len), sizeof(len));

		string huffmanCode((int)len, '\0');

		fin.read(reinterpret_cast<char *>(&huffmanCode[0]), sizeof(char) * (int)len);

		cout << "sym: " << sym << " len: " << len << " code: " << huffmanCode << endl;
		totalCodeLen += (int)len;

		for (unsigned int j = 0; j < (int)huffmanCode.size(); j++) {
			if (judgement(huffmanCode[j]) == 0) {
				getLeftChildAddr();  // 0 is left direction
			} else if (judgement(huffmanCode[j]) == 1) {
				getRightChildAddr(); // 1 is right direction
			} else {
				cout << "---------- CRITICAL ERROR : INVALID Huffman Encode Table ----------" << endl;
				exit(EXIT_FAILURE);
			}
		}
		cur->symbol = sym;
	}

	if (totalNumOfNode == totalCodeLen) {
		cout << "----------------------------------------------------> Rebuild the huffman table" << endl;
		cout << "Complete to rebuild the Huffman Table" << endl << endl << endl;
	} else {
		cout << "Failed to rebuild the Huffman Table !!" << endl << endl;
		exit(EXIT_FAILURE);
	}
}


int HuffmanDecoder::judgement(char oneORzero)
{
	if (oneORzero == '0') {
		return 0;
	} else if (oneORzero == '1') {
		return 1;
	} else {
		cout << "---------- CRITICAL ERROR : INVALID Huffman Encode Table ----------" << endl;
		exit(EXIT_FAILURE);
	}
}


void HuffmanDecoder::getLeftChildAddr()
{
	if (cur->left == nullptr) {
        cur->left = createNewNode();
	}
	
	cur = cur->left;
	totalNumOfNode++;
}


void HuffmanDecoder::getRightChildAddr()
{
	if (cur->right == nullptr) {
        cur->right = createNewNode();
	}
	
	cur = cur->right;
	totalNumOfNode++;
}


HuffmanTreeNode* HuffmanDecoder::createNewNode()
{
	HuffmanTreeNode *newNode = new HuffmanTreeNode;

	if (newNode == nullptr) {
		cout << "Failed to allocate new node memory !!" << endl;
		return nullptr;
	}

	newNode->left = nullptr;
	newNode->right = nullptr;

	return newNode;
}


void HuffmanDecoder::decode(vector<Symbol> &symData, ifstream &fin)
{
	cur = rootNode;

	fin.read(reinterpret_cast<char *>(&totalNumOfBit), sizeof(totalNumOfBit));

	// 自分でもよくわからない
	if (totalNumOfBit != 0) {
		copyTotalNumOfBit = totalNumOfBit;
		string codeBuf(BUF_SIZE, '\0');

		int byteCNT = 0;

		while (flag) {
			int readCNT = 0;
			for (int i = 0; i < (int)codeBuf.size(); i++) {
				if (byteCNT > copyTotalNumOfBit / 8) break;
				fin.read(reinterpret_cast<char *>(&codeBuf[i]), sizeof(codeBuf[i]));
				byteCNT++;
				readCNT++;
			}
			for (int i = 0; i < readCNT; i++) {
				for (int j = 0; j < 8; j++) {
					if ((char)(codeBuf[i] & 0x80) == 0) {
						cur = cur->left;
					} else {
						cur = cur->right;
					}

					codeBuf[i] = codeBuf[i] << 1;
					totalNumOfBit--;
					countBit++;

					if (cur->left == nullptr && cur->right == nullptr) {
						pushSymData(symData);
					}
				}
			}
		}
	}
}


void HuffmanDecoder::pushSymData(vector<Symbol> &symData)
{
	symBuf[symBufIdx] = cur->symbol;
	symBufIdx++;

	cur = rootNode;

	if (totalNumOfBit == 0 && copyTotalNumOfBit == countBit) {
		for (int i = 0; i < symBufIdx; i++) {
				symData.push_back(symBuf[i]);
		}

		cout << "----------------------------------------------------> Decode" << endl;
		cout << "Complete to decode" << endl << endl;

		flag = false;
	} else if (symBufIdx == (int)symBuf.size() / 2) {
		for (int i = 0; i < (int)symBuf.size() / 2; i++) {
			symData.push_back(symBuf[i]);
		}
		
		symBuf.resize(BUF_SIZE, 0);
		symBufIdx = 0;
	}
}