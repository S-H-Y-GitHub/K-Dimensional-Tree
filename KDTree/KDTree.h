#pragma once
#include <string>
#include <list>
#include <time.h>
#include <math.h>
//������Խ����
#define IS_NORMAL 0
#define IS_DOS 1
#define OTHERS 2
#define	NEW_MAX 1000000
#define NEW_MIN 0
#define BUCKET_SIZE 4
#define MAX_DST 900000

using namespace std;

struct KDDData
{
	/*
	//�������Լ�
	double duration;		//length (number of seconds) of the connection					0
	string protocol_type;	//type of the protocol, e.g. tcp, udp, etc.						1
	string service;			//network service on the destination, e.g., http, telnet,etc.	2
	string flag;			//normal or error status of the connection						3
	double src_bytes;		//number of data bytes from source to destination				4
	double dst_bytes;		//number of data bytes from destination to source				5
	bool land;				//1 if connection is from/to the same host/port;0 otherwise		6
	double wrong_fragment;	//number of ``wrong'' fragments									7
	double urgent;			//number of urgent packets										8
	//��������9~21
	//�������Լ�
	double count; 			//number of connections to the same host as the current connection in the past two seconds		22
	double srv_count;		//number of connections to the same service as the current connection in the past two seconds	23
	double serror_rate;		//% of connections that have ``SYN'' errors						24
	double srv_serror_rate;	//% of connections that have ``SYN'' errors						25
	double rerror_rate;		//% of connections that have ``REJ'' errors						26
	double srv_rerror_rate;	//% of connections that have ``REJ'' errors						27
	double same_srv_rate;	//% of connections to the same service							28
	double diff_srv_rate;	//% of connections to different services						29
	double srv_diff_host_rate;//% of connections to different hosts							30
	//��������31~40
	*/
	double properties[9];	//�������Լ�����ӦԴ�����еĵ�22-30��
	int label;		//������ӵ����ͣ�normal��ʾ��������, back��land��neptune��pod��teardrop��smurf��ʾDoS����		41
	KDDData() { label = OTHERS; }
	bool operator==(const KDDData& rhs) const
	{
		bool r = true;
		for (int i = 0; i < 9; ++i)
			if (this->properties[i] != rhs.properties[i])
				r = false;
		if (label != rhs.label)
			r = false;
		return r;
	}
};
namespace std
{
	template<> struct hash<KDDData>
	{
		typedef KDDData argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const
		{
			result_type const h1(std::hash<double>{}(s.properties[0]));
			result_type const h2(std::hash<double>{}(s.properties[1]));
			result_type const h3(std::hash<int>{}(s.label));
			return h1 * 31 + h2 * 59 + h3;
		}
	};
}

struct KDTreeNode
{
	int d = 0;			//�ָ�ά
	double spno;		//�ָ��
	KDTreeNode* lc = nullptr;	//С�ڵ��ڷָ��ķ�֧
	KDTreeNode* gc = nullptr;	//���ڷָ��ķ�֧
	list<KDDData*> value;	//�����Ҷ��㣬�����ﱣ�����ݵ��ָ��
};

class KDTree
{
public:
	KDTree(vector<KDDData>* datas);
	~KDTree();
	void test(vector<KDDData>* testDatas) const;
	int getResult(KDDData* testData) const;

private:
	void buildTree(KDTreeNode* node);

	KDTreeNode* root;
	double radios[9];		//���ڹ��ʱ�ı���
	double maxs[9];		//��Nά�����ֵ
	double mins[9];		//��Nά����Сֵ
};



inline KDTree::KDTree(vector<KDDData>* datas)
{
	clock_t start, finish;
	double totaltime;
	start = clock();
	cout << "���ڽ���KD��...";
	if (datas->empty())
	{
		cerr << "���ڽ���KD�������ݼ��ǿռ���" << endl;
		return;
	}
	//else
	//��ȡ���ݱ߽�
	for (int i = 0; i < 9; ++i)
		mins[i] = maxs[i] = (datas->begin())->properties[i];
	for (KDDData& data : *datas)
	{
		for (int i = 0; i < 9; i++)
		{
			maxs[i] = data.properties[i] > maxs[i] ? data.properties[i] : maxs[i];
			mins[i] = data.properties[i] < mins[i] ? data.properties[i] : mins[i];
		}
	}
	//�������
	for (int i = 0; i < 9; ++i)
	{
		if (maxs[i] - mins[i] == 0)
		{
			radios[i] = 1;
			continue;
		}
		radios[i] = (NEW_MAX - NEW_MIN) / (maxs[i] - mins[i]);
		for (KDDData& data : *datas)
			data.properties[i] = (data.properties[i] - mins[i])*radios[i];//+NEW_MIN
	}
	//�ݹ齨��
	root = new KDTreeNode;
	for (KDDData& data : *datas)
		root->value.push_back(&data);
	buildTree(root);
	finish = clock();
	totaltime = double(finish - start) / CLOCKS_PER_SEC;
	cout << "��ɣ�" << endl;
	cout << "\t���ò����� NEW_MAX:" << NEW_MAX << ", NEW_MIN:" << NEW_MIN << ", BUCKET_SIZE:" << BUCKET_SIZE << ", MAX_DST:" << MAX_DST << endl;
	cout << "\t���º�ʱ" << totaltime << "��" << endl;
}

inline void KDTree::buildTree(KDTreeNode* node)
{//�ݹ齨��
	double insidemaxs[9];		//��Nά�����ֵ
	double insidemins[9];		//��Nά����Сֵ
	double maxrange = 0;	//�����չ��
	if (node->value.size() <= BUCKET_SIZE)
		return;
	//��ȡ���ݱ߽�
	for (int i = 0; i < 9; ++i)
		insidemins[i] = insidemaxs[i] = node->value.front()->properties[i];
	for (KDDData* data : node->value)
	{
		for (int i = 0; i < 9; i++)
		{
			insidemaxs[i] = data->properties[i] > insidemaxs[i] ? data->properties[i] : insidemaxs[i];
			insidemins[i] = data->properties[i] < insidemins[i] ? data->properties[i] : insidemins[i];
		}
	}
	//�ҳ����������չ�ȵ�ά
	for (int i = 0; i < 9; ++i)
	{
		if (maxrange < (insidemaxs[i] - insidemins[i]))
		{
			maxrange = insidemaxs[i] - insidemins[i];
			node->d = i;
		}
	}
	if (maxrange == 0)
		return;
	//�涨�ָ�㣬���㼯�ָ�
	node->spno = insidemins[node->d] + maxrange / 2;
	node->gc = new KDTreeNode;
	node->lc = new KDTreeNode;
	for (KDDData* data : node->value)
	{
		if (data->properties[node->d] > node->spno)
			node->gc->value.push_back(data);
		else
			node->lc->value.push_back(data);
	}
	//�ݹ齨��
	buildTree(node->lc);
	buildTree(node->gc);
}

inline KDTree::~KDTree()
{
	//todo �����������
}

inline void KDTree::test(vector<KDDData>* testDatas) const
{
	int result;
	int cn = 0, cd = 0, co = 0, rn = 0, rd = 0, ro = 0;
	double totaltime, nmis = 0, dmatch = 0;
	clock_t start, finish;
	start = clock();
	cout << "���ڽ��в���...";
	for (KDDData& testData : *testDatas)
	{
		result = getResult(&testData);
		switch (result)
		{
		case IS_NORMAL:
			++rn;
			if (testData.label == IS_NORMAL)
				++cn;
			else if (testData.label == IS_DOS)
				++cd;
			else
				++co;
			break;
		case IS_DOS:
			++rd;
			if (testData.label == IS_DOS)
			{
				++cd;
				++dmatch;
			}
			else if (testData.label == IS_NORMAL)
			{
				++nmis;
				++cn;
			}
			else
				++co;
			break;
		default:
			++ro;
			if (testData.label == IS_DOS)
			{
				++cd;
				dmatch += 0.5;
			}
			else if (testData.label == IS_NORMAL)
			{
				++cn;
				nmis += 0.5;
			}
			else
				++co;
			break;
		}
	}
	finish = clock();
	totaltime = double(finish - start) / CLOCKS_PER_SEC;
	cout << "��ɣ�\n\t���Խ������������:" << rn << ", DoS����:" << rd << ", �������:" << ro << endl;
	cout << "\tʵ���������������:" << cn << ", DoS����:" << cd << ", �������:" << co << endl;
	cout << "\tDoS����ʣ�" << dmatch / cd * 100 << "%, �������������ʣ�" << nmis / cn * 100 << "%" << endl;
	cout << "\t���º�ʱ" << totaltime << "��" << endl;
}

inline int KDTree::getResult(KDDData* testData) const
{
	//��񻯲�������
	for (int i = 0; i < 9; ++i)
		testData->properties[i] = (testData->properties[i] - mins[i])*radios[i];//+NEW_MIN
	//����
	KDTreeNode *c = root, *nc = root;
	while (nc != nullptr)
	{
		c = nc;
		nc = testData->properties[c->d] > c->spno ? c->gc : c->lc;
	}
	//�ж��Ƿ�����
	double dst = DBL_MAX, sum;
	KDDData* point = nullptr;
	for (KDDData* data : c->value)
	{
		sum = 0;
		for (int i = 0; i < 9; ++i)
			sum += (data->properties[i] - testData->properties[i]) * (data->properties[i] - testData->properties[i]);
		sum = sqrt(sum);
		if (dst > sum)
		{
			dst = sum;
			point = data;
		}
	}
	if (dst > MAX_DST)
		return OTHERS;
	return point->label;
}
