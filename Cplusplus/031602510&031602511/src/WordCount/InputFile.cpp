#include "InputFile.h"
#include "PhraseCmp.h"
#include <algorithm>
#include <iostream>

/*bug1��ֻ�ᵥ��ֻ�����ͳ��һ��
 �޸���������char����wordArray�ж��һ�����ʣ��ڴﵽ(Ҫ��)������ʱ����phraseLength�����ʣ� ÿ�ζ�ȡ�����ָ�����һ����׺�ľ���
  bug2��Abstract��ĩβû�л��з�ʱ����ͳ��һ�ε���
 �޸�������1.ѯ�����������ļ���ʽ
 */

/*�����ú���*/
vector<pair<string, int>> InputFile::getOrderWord()
{
	if (orderWord.empty()) sortPair();
	return orderWord;
}

InputFile::InputFile(string inputPath, int _phraseLength, int _weight, int _sortNum)
{
	crtNum = 0;
	lineNum = 0;
	wordNum = 0;

	sortNum = _sortNum;
	phraseLength = _phraseLength;
	weight = _weight;
	hasCount = false;	

	inputFile.open(inputPath, ios::in);

	if (!inputFile)
	{
		hasOpenFile = false;
		cerr << inputPath << "�ļ������ڣ�" << endl;
		exit(1);
	}
					
	hasOpenFile = true;
}

InputFile::~InputFile()
{
	inputFile.close();
}

int InputFile::getCrtNum()
{
	if (!hasCount) readFile();
	return crtNum;
}

int InputFile::getLineNum()
{
	if (!hasCount) readFile();
	return lineNum;
}

int InputFile::getWordNum()
{
	if (!hasCount) readFile();
	return wordNum;
}

void InputFile::readFile()
{
	hasCount = true;
	if (!hasOpenFile) return;

	/*��ͷ��ȡ�ļ�*/
	inputFile.clear(ios::goodbit);
	inputFile.seekg(ios::beg);
	
	/*�ݴ��Ա����*/
	int _lineNum = 0;
	int _crtNum = 0;
	int _wordNum = 0;

	/*�ж��ַ��Ƿ���Ч���õľֲ�����*/
	int crlfNum = 0;											//���з���Ŀ
	bool flag_switch = true;									//�Ƿ����switch���
	int skipNum = 0;											//Title��Abstract���������ַ���
	bool flag_count = false;									//�Ƿ���д�Ƶͳ��	

	bool flag_hasCrt = false;									//�ж��Ƿ�հ���
	int _weight = 1;

	/*�жϵ������õľֲ�����*/
	char tempChar;											
	int alpLength = 0;											//������������ĸ����
	bool flag_numBefAlp = false;								//�ж���ĸǰ�Ƿ��������
	char wordArray[100];										//��Ŵ��������	
	int arrayLength = 0;										//���鳤��(����char����תstring)


	int wordLength[10];											//����ÿ�����ʼ��Ϸָ�����׺�ĳ���
	int tag = 0;												//���浱ǰ��׺���ȵ��±�(���к��0��ʼ)
	int tempTag = 0;
	int tempInt = 0;											//���浥����׺����
	string phrase;												//��ʱ����Ĵ���
	int _phraseLength = 0;										//phrase�ַ����е��ʵĸ���(�����Ŀǰ����)

	inputFile >> noskipws;										//�����Կո�
	
	while (inputFile.get(tempChar))
	{
		/*ͨ��crlfNum��ֵ�жϵ�ǰ������һ��*/
		switch (crlfNum)
		{
		/*Title�У������߸��ַ�����д�Ƶͳ��*/
		case 1:
		{
			if (skipNum == 7)
			{
				flag_count = true;
				skipNum = 0;
				if (weight == 1) _weight = 10;
			}
			if (flag_count) break;
			skipNum++;
			break;
		}
		/*Abstract�У�����10���ַ�����д�Ƶͳ��*/
		case 2:
		{
			if (skipNum == 10)
			{
				flag_count = true;
				skipNum = 0;
				_weight = 1;
			}
			if (flag_count) break;
			skipNum++;
			break;
		}
		/*�����У����Ե�ǰ�������ַ�*/
		default:
			if (tempChar == '\n') crlfNum = (crlfNum + 1) % 5;
			flag_count = false;
			break;
		}

		if (!flag_count) continue;

		/*��Ƶͳ��*/
		_crtNum++;
		
		/*
		��Ƶͳ��
		 1. �жϵ���
		 */
																/*��ȡ�ַ�Ϊ�Ƿָ���*/
		if ((tempChar >= 'a' && tempChar <= 'z') || (tempChar >= 'A' && tempChar <= 'Z') || (tempChar >= '0' && tempChar <= '9')) 
		{	
			/*���ڷǷ�����ʱ�޷����ɴ���,��ջ���*/
			if ((tempChar >= '0' && tempChar <= '9') && (alpLength < 4)) 
			{
				alpLength = 0;												
				arrayLength = 0;	
				_phraseLength = 0;
				flag_numBefAlp = true;			
				tempInt = 0;							
				continue;
			}
			else if (!(tempChar >= '0' && tempChar <= '9')) 
			{							
				if (flag_numBefAlp && alpLength < 4) continue;	//���ǰһ���ַ���������str���鿪ͷ������ĸ��Ŀ<4,��һ�����ǵ��ʣ�ֱ�ӽ�����һ�ζ��ַ�

				if (tempChar >= 'A' && tempChar <= 'Z') tempChar = tempChar - 'A' + 'a';	
				alpLength++;									//�����ʿ�ͷ��������ĸ��Ŀ+1
				if (alpLength == 1 && _phraseLength > 0)		/*�µĵ��ʶ��룬�ַ�����ǰһ��Ϊ��׺*/
				{
					wordLength[tag] = tempInt;
					tempInt = 0;
					tag = (tag + 1) % 10;
				}
			}
			wordArray[arrayLength++] = tempChar;				//����Ҫ����ַ������������α����
			tempInt++;
		}
																/*��ȡ�ַ�Ϊ�ָ���*/
		else
		{
			/*�ָ���ǰ��һ�������ĵ���*/
			if (alpLength >= 4)
			{
				_phraseLength++;	
				_wordNum++;
				tempInt++;

				cout << wordArray << endl;

				/*���ʸ�������������Ҫ��*/
																//����Ҫ��Ӧ����ǰ�������phrase��������������׺��������
				if (_phraseLength == phraseLength)
				{
					/*�������phrase*/
					wordArray[arrayLength] = 0;
					phrase = wordArray;						

					/*���������������*/
					itor = phraseMap.find(phrase);
					if (itor != phraseMap.end()) itor->second += _weight;
					else phraseMap[phrase] = _weight;
				
					/*����������׺��������*/
					tempTag = tag - phraseLength + 1;
					if (tempTag < 0) tempTag += 10;
					phrase = phrase.substr(wordLength[tempTag]);
					strcpy_s(wordArray, phrase.c_str());
					arrayLength -= (wordLength[tempTag]);		//�൱���ַ�����ָ�����һ����׺�ĳ���

					_phraseLength--;							//�ַ������д�׺��Ŀ-1
				}
				wordArray[arrayLength++] = tempChar;
				alpLength = 0;
			}
			/*�ָ�����������ָ�����ã��豣��*/
			else if (alpLength == 0 && _phraseLength > 0)
			{
				wordArray[arrayLength++] = tempChar;
				tempInt++;
			}
			/*�ָ���ǰ���ǷǷ����ʣ�������黺��*/
			else if (alpLength > 0 && alpLength < 4)
			{
				alpLength = 0;
				arrayLength = 0;
				_phraseLength = 0;
				tempInt = 0;
			}
		}

		/*
		��Ƶͳ��
		 2. �жϿհ���
		 */
		if (tempChar != ' ' && !flag_hasCrt && tempChar != '\t' && tempChar != '\n') flag_hasCrt = true;
		if (tempChar == '\n')
		{
			if (flag_hasCrt = true)
			{
				_lineNum++;
				flag_hasCrt = false;

				alpLength = 0;
				arrayLength = 0;
				_phraseLength = 0;

				tempInt = 0;
				tag = 0;
			}
			crlfNum = (crlfNum + 1) % 5;
			flag_count = false;
			continue;
		}
	}
	
	lineNum = _lineNum;
	crtNum = _crtNum;
	wordNum = _wordNum;
}

/*�����ʰ�Ҫ������󱣴浽orderWord������*/
void InputFile::sortPair() {
	/*ͨ����������unordered_map�е�pair���ݸ��Ƶ�vector��*/
	for (itor = phraseMap.begin(); itor != phraseMap.end(); itor++) {
		orderWord.push_back(make_pair(itor->first, itor->second));
	}
	size_t i = phraseMap.size();	
	if (i > sortNum) i = sortNum;
	/*ͨ��paritial_sort������vector�еĵ��ʰ���ĿҪ������*/
	partial_sort(orderWord.begin(), orderWord.begin() + i, orderWord.end(), PhraseCmp());
}