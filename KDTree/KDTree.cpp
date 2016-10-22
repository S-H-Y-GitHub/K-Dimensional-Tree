#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "KDTree.h"
//using namespace std;

vector<string> split(string& s, const string& p);
set<KDDData*> readData(const string& filename);
set<KDDData*> readTestData(const string& filename);

int main()
{
	set<KDDData*> result = readData("asd");
	KDTree kdt(result);
	return 0;
}

vector<string> split(string& s, const string& p)
{ //��s�ַ�����p�ָ�
	int pos;
	int size = s.size();
	vector<string> result;
	result.reserve(50);
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

set<KDDData*> readTestData(const string& filename)
{
	set<KDDData*> result;
	KDDData* data;
	vector<string> temp;
	string s;
	ifstream in;
	clock_t start, finish;
	double totaltime;
	cout << "���ڶ�ȡ�ļ� " << filename << " ...";
	int cEntry = 0;
	start = clock();
	in.open(filename);
	if (in.fail())
	{
		cerr << "���ļ�ʧ�ܣ�" << endl;
		exit(-1);
	}
	temp.reserve(50);
	while (getline(in, s))
	{
		++cEntry;
		data = new KDDData;
		temp = split(s, ",");
		for (int i = 0; i < 9; i++)
			data->properties[i] = atof(temp[i + 22].data());
		if ("normal." == temp[41])
			data->label = IS_NORMAL;
		else if (("back." == temp[41]) || ("land." == temp[41]) || ("neptune." == temp[41])
			|| ("pod." == temp[41]) || ("teardrop." == temp[41]) || ("smurf." == temp[41]))
			data->label = IS_DOS;
		result.insert(data);
	}
	in.close();
	finish = clock();
	totaltime = double(finish - start) / CLOCKS_PER_SEC;
	cout << "�ɹ���ɣ�\n\t�Ѵ�������" << cEntry << "����ȥ���ظ���Ч��Ŀ�����ڲ���KD�������ݼ���СΪ" << result.size() << endl;
	cout << "\t���º�ʱ��" << totaltime << "��" << endl;
	return result;
}

set<KDDData*> readData(const string& filename)
{ //��KDD�����ļ��ж�ȡ����
	set<KDDData*> result;
	KDDData* data;
	vector<string> temp;
	string s;
	ifstream in;
	clock_t start, finish;
	double totaltime;
	cout << "���ڶ�ȡ�ļ� " << filename<<" ...";
	int cEntry = 0, cNormal = 0, cDoS = 0;
	//	in.sync_with_stdio(false);
	start = clock();
	in.open(filename);
	if (in.fail())
	{
		cerr << "���ļ�ʧ�ܣ�" << endl;
		exit(-1);
	}
	temp.reserve(50);
	while (getline(in, s))
	{
		++cEntry;
		temp = split(s, ",");
		if ("normal." == temp[41])
		{
			data = new KDDData;
			data->label = IS_NORMAL;
			++cNormal;
			for (int i = 0; i < 9; i++)
				data->properties[i] = atof(temp[i + 22].data());
			result.insert(data);
		}
		else if (("back." == temp[41]) || ("land." == temp[41]) || "neptune." == temp[41]
			|| ("pod." == temp[41]) || ("teardrop." == temp[41]) || ("smurf." == temp[41]))
		{
			data = new KDDData;
			++cDoS;
			data->label = IS_DOS;
			for (int i = 0; i < 9; i++)
				data->properties[i] = atof(temp[i + 22].data());
			result.insert(data);
		}
	}
	in.close();
	finish = clock();
	totaltime = double(finish - start) / CLOCKS_PER_SEC;
	cout << "�ɹ���ɣ�\n\t�Ѵ�������" << cEntry << "�������У���������" << cNormal << "����DoS����" << cDoS << "����" << endl;
	cout << "\tȥ���ظ���Ч��Ŀ�����ڽ���KD�������ݼ���СΪ" << result.size()<<endl;
	cout << "\t���º�ʱ��" << totaltime << "��" << endl;
	return result;
}
