#include <iostream>
#include <fstream>
#include "KDTree.h"
//using namespace std;

vector<string> split(string s, string p);
vector<KDDData> readData(const string filename, int divider);

int main()
{
	vector<KDDData> result = readData("kddcup.data_10_percent_corrected", 500);
	return 0;
}

vector<string> split(string s, string p)
{ //��s�ַ�����p�ָ�
	int pos;
	int size = s.size();
	vector<string> result;
	s += p;
	for (int i = 0; i < size; i++)
	{
		pos = s.find(p, i);
		if (pos <= size)
		{
			string temp = s.substr(i, pos - i);
			result.push_back(temp);
			i = pos + p.size() - 1;
		}
	}
	return result;
}

vector<KDDData> readData(const string filename, int divider)
{ //��KDD�����ļ��ж�ȡ1/n������
	vector<KDDData> result; //todo ������vector̫��
	KDDData data;
	vector<string> temp;
	string s;
	ifstream in;
	in.open(filename);
	if (in.fail())
	{
		cerr << "���ļ�ʧ��" << endl;
		exit(-1);
	}
	while (getline(in, s))
	{
		for (int i = 0;i < divider; i++)
		{
			if (!getline(in, s))
			{
				in.close();
				return result;
			}
		}
		temp = split(s, ",");
		for (int i = 0; i < 9; i++)
			data.properties[i] = atof(temp[i + 22].data());
		if (temp[41] == "normal")
			data.isNormal = true;
		else if ((temp[41] == "back") || (temp[41] == "land") || (temp[41] == "neptune")
			|| (temp[41] == "pod") || (temp[41] == "teardrop") || (temp[41] == "smurf"))
			data.isDoS = true;
		result.push_back(data);
	}
	in.close();
	return result;
}