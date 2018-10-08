#include <iostream>
#include <sstream>
#include "InputFile.h"

using namespace std;

void paraJudge(int &argc, char *argv[], string *input, string *output, int *weight, int *phraseLength, int *sortNum);
void writeTxt(string *output, int &crtNum, int &lineNum, int &wordNum, int &sortNum, vector<pair<string, int>>::iterator outItor, vector<pair<string, int>>::iterator outItorEnd);

int main(int argc, char *argv[])
{
	/*�����в�����أ��ֱ�Ϊ��
	 -i, -o, -w, -m, -n
	 */
	string input;
	string output;
	int weight = 0;
	int phraseLength = 1;
	int sortNum = 10;

	/*
	input = "input.txt";
	weight = 0;
	phraseLength = 1;
	sortNum = 25;
	*/
	paraJudge(argc, argv, &input, &output, &weight, &phraseLength, &sortNum);

	InputFile inputFile(input, phraseLength, weight, sortNum);

	int crtNum = inputFile.getCrtNum();
	int lineNum = inputFile.getLineNum();
	int wordNum = inputFile.getWordNum();
	vector<pair<string, int>> orderWord = inputFile.getOrderWord();
	vector<pair<string, int>>::iterator outItor;
	vector<pair<string, int>>::iterator outItorEnd;
	outItor = orderWord.begin();
	outItorEnd = orderWord.end();

	writeTxt(&output, crtNum, lineNum, wordNum, sortNum, outItor, outItorEnd);

	/*�������*/
	///*
	//*/
	/*
	cout << "characters: " << inputFile.getCrtNum() << endl;
	cout << "words: " << inputFile.getWordNum() << endl;
	cout << "lines: " << inputFile.getLineNum() << endl;
	vector<pair<string, int>> orderWord = inputFile.getOrderWord();
	vector<pair<string, int>>::iterator outItor;
	outItor = orderWord.begin();
	for (int i = 0; i < sortNum && outItor != orderWord.end(); i++, outItor++) 
	{
		cout << "<" << outItor->first << ">" << ": " << outItor->second << endl;
	}
	//system("pause");
	*/

	return 0;
}

void paraJudge(int &argc, char *argv[], string *input, string *output, int *weight, int *phraseLength, int *sortNum)
{
	bool has_i = false;
	bool has_o = false;
	bool has_w = false;
	bool has_m = false;
	bool has_n = false;

	string temp;
	stringstream stream;

	if (argc % 2 == 0 || argc < 7)
	{
		cerr << "�����в�����ʽ����" << endl;
		exit(1);
	}

	for (int i = 1; i < argc; i += 2)
	{
		temp = argv[i + 1];
		if (!strcmp("-i", argv[i]))
		{
			if (has_i)
			{
				cerr << "�ظ�����-iָ��" << endl;
				exit(1);
			}
			has_i = true;
			*input = temp;
		}
		else if (!strcmp("-o", argv[i]))
		{
			if (has_o)
			{
				cerr << "�ظ�����-oָ��" << endl;
				exit(1);
			}
			has_o = true;
			*output = temp;
		}
		else if (!strcmp("-w", argv[i]))
		{
			if (has_w)
			{
				cerr << "�ظ�����-wָ��" << endl;
				exit(1);
			}
			has_w = true;
			stream << temp;
			stream >> *weight;
			stream.clear();
			if (*weight != 0 && *weight != 1)
			{
				cerr << "-wָ���������" << endl;
				exit(1);
			}
		}
		else if (!strcmp("-m", argv[i]))
		{
			if (has_m)
			{
				cerr << "�ظ�����-mָ��" << endl;
				exit(1);
			}
			has_m = true;
			stream << temp;
			stream >> *phraseLength;
			stream.clear();
			if (*phraseLength < 2 || *phraseLength > 10)
			{
				cerr << "-mָ���������" << endl;
				exit(1);
			}
		}
		else if (!strcmp("-n", argv[i]))
		{
			if (has_n)
			{
				cerr << "�ظ�����-nָ��" << endl;
				exit(1);
			}
			has_n = true;
			stream << temp;
			stream >> *sortNum;
			stream.clear();
			if (*sortNum < 0 || *sortNum > 100)
			{
				cerr << "-nָ���������" << endl;
				exit(1);
			}
		}
		else
		{
			cerr << "ָ�����" << endl;
			exit(1);
		}
	}

	if (!has_i || !has_o || !has_w)
	{
		cerr << "�����в�����ʽ����" << endl;
		exit(1);
	}
}

void writeTxt(string *output, int &crtNum, int &lineNum, int &wordNum, int &sortNum, vector<pair<string, int>>::iterator outItor, vector<pair<string, int>>::iterator outItorEnd)
{
	ofstream fileWrite(*output, ios::out | ios::trunc);
	if (!fileWrite) {										//�쳣����
		cerr << output << "�ļ�д��ʧ��!" << endl;
		exit(1);											//�ڵ�Ԫ����ʱΪ��֤���Բ��ж��轫��ע��
	}
	fileWrite << "characters: " << crtNum << endl;
	fileWrite << "words: " << wordNum << endl;
	fileWrite << "lines: " << lineNum << endl;
	for (int i = 0; i < sortNum && outItor != outItorEnd; i++, outItor++) {
		fileWrite << "<" << outItor->first << ">" << ": " << outItor->second << endl;
	}
	fileWrite.close();
}