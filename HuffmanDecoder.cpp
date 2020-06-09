#include "HuffmanDecoder.hpp"

using namespace std;

HuffmanDecoder::HuffmanDecoder()
{
	rootNode = nullptr;
	cur = nullptr;

	cntNonZeroSym = 0;
	totalNumOfNode = 0;
	totalCodeLen = 0;

	sym = 0;
	len = NULL;

	memset(codeBuf, NULL, sizeof(codeBuf));

	memset(symBuf, 0, sizeof(symBuf));

	totalNumOfBit = 0;
	copyTotalNumOfBit = 0;
	countBit = 0;
	symBufIdx = 0;

	flag = true;
}


HuffmanDecoder::~HuffmanDecoder()
{
}


void HuffmanDecoder::performDecoding(vector<unsigned short> &symData, ifstream &fin)
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

		fin.read(reinterpret_cast<char *>(&sym), sizeof(sym));
		fin.read(reinterpret_cast<char *>(&len), sizeof(len));

		fin.read(reinterpret_cast<char *>(&codeBuf), sizeof(unsigned char) * (int)len);

		codeBuf[(int)len] = NULL;
		totalCodeLen += (int)len;

		for (unsigned int j = 0; j < strlen(codeBuf); j++) {
			if (judgement(&codeBuf[j]) == 0) {
                getLeftChildAddr();  // 0 is left direction
            } else if (judgement(&codeBuf[j]) == 1) {
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


int HuffmanDecoder::judgement(char *oneORzero)
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


void HuffmanDecoder::getLeftChildAddr()
{
	if (cur->left == NULL)
		cur->left = createNewNode();


	cur = cur->left;

	totalNumOfNode += 1;
}


void HuffmanDecoder::getRightChildAddr()
{
	if (cur->right == NULL)
		cur->right = createNewNode();


	cur = cur->right;

	totalNumOfNode += 1;
}


HuffmantreeNode* HuffmanDecoder::createNewNode()
{
	HuffmantreeNode *newNode = (HuffmantreeNode *)calloc(1, sizeof(HuffmantreeNode));


	if (newNode == NULL)
	{
		puts("Failed to allocate new node memory !!");

		return NULL;
	}


	newNode->left = NULL;

	newNode->right = NULL;


	return newNode;
}


void HuffmanDecoder::decode(vector<unsigned short> &symData, ifstream &fin)
{
	cur = rootNode;

	fin.read(reinterpret_cast<char *>(&totalNumOfBit), sizeof(totalNumOfBit));

	printf("totalNumOfBit: %d\n", totalNumOfBit);
	if (totalNumOfBit != 0) {
		copyTotalNumOfBit = totalNumOfBit;
		memset(codeBuf, NULL, sizeof(codeBuf));

		int byteCount = 0;

		while (flag == TRUE)
		{
			int readCNT = 0;
			for (int i = 0; i < (int)sizeof(codeBuf); i++) {
				if (byteCount > copyTotalNumOfBit / 8) break;
				fin.read(reinterpret_cast<char *>(&codeBuf[i]), sizeof(codeBuf[i]));
				byteCount++;
				readCNT++;
			}
			for (int i = 0; i < readCNT; i++) {
				for (int j = 0; j < 8; j++) {
					if ((char)(codeBuf[i] & 0x80) == 0) {
						cur = cur->left;
					}
					else {
						cur = cur->right;
					}

					codeBuf[i] = codeBuf[i] << 1;

					totalNumOfBit -= 1;

					countBit += 1;


					if (cur->left == NULL && cur->right == NULL) {
						recData(symData);
					}
				}
			}
		}
	}
	/*copyTotalNumOfBit = totalNumOfBit;
	memset(codeBuf, NULL, sizeof(codeBuf));
	int byteCount = 0;
	while (flag == TRUE)
	{
		int readCNT = 0;
		for (int i = 0; i < (int)sizeof(codeBuf); i++) {
			if (byteCount > copyTotalNumOfBit / 8) break;
			fin.read(reinterpret_cast<char *>(&codeBuf[i]), sizeof(codeBuf[i]));
			byteCount++;
			readCNT++;
		}
		for (int i = 0; i < readCNT; i++) {
			for (int j = 0; j < 8; j++) {
				if ((char)(codeBuf[i] & 0x80) == 0) {            
					cur = cur->left;
				}
				else {
					cur = cur->right;
				}
				codeBuf[i] = codeBuf[i] << 1;
				totalNumOfBit -= 1;
				countBit += 1;
				if (cur->left == NULL && cur->right == NULL) {
					recData(symData);
				}
			}
		}
	}*/
}


void HuffmanDecoder::recData(vector<unsigned short> &symData)
{
	symBuf[symBufIdx] = cur->symbol;

	symBufIdx += 1;

	cur = rootNode;


	if (totalNumOfBit == 0)
	{
		if (copyTotalNumOfBit == countBit)
		{
			for (int i = 0; i < symBufIdx; i++) {
				symData.push_back(symBuf[i]);
			}


			puts("----------------------------------------------------> Decode");
			puts("Complete to decode");
			puts("");

			flag = FALSE;
		}
	}
	else
	{
		if (symBufIdx == sizeof(symBuf) / 2)
		{
			for (int i = 0; i < sizeof(symBuf) / 2; i++) {
				symData.push_back(symBuf[i]);
			}
		
			memset(symBuf, 0, sizeof(symBuf));

			symBufIdx = 0;
		}
	}
}