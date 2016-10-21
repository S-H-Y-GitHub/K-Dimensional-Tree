#pragma once
#include <string>
#include <set>
#include <list>
#include <vector>
//������Խ����
#define IS_NORMAL 0
#define IS_DOS 1
#define OTHERS 2
#define	new_max 2
#define new_min 0
#define bucket_size 5

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
	bool isNormal = false;	//������ӵ����ͣ�normal��ʾ��������										41
	bool isDoS = false;		//������ӵ����ͣ�back��land��neptune��pod��teardrop��smurf��ʾDoS����		41
};

struct KDTreeNode
{
	int d = 0;			//�ָ�ά
	double spno;		//�ָ��
//	double max = new_max;		//���ֵ
//	double min = new_min;		//��Сֵ
	KDTreeNode* lc = nullptr;	//С�ڵ��ڷָ��ķ�֧
	KDTreeNode* gc = nullptr;	//���ڷָ��ķ�֧
	list<KDDData*> value;	//�����Ҷ��㣬�����ﱣ�����ݵ��ָ��
};

class KDTree
{
public:
	KDTree(set<KDDData*>& datas);
	~KDTree();
	void test(set<KDDData*> testDatas);
	int getResult(KDDData testData);

private:
	void buildTree(KDTreeNode* node) const;

	KDTreeNode* root;
	double radios[9];		//���ڹ��ʱ�ı�����-1��ʾĳά��ֻ�г���ֵ
};

inline KDTree::KDTree(set<KDDData*>& datas)
{
	double maxs[9];		//��Nά�����ֵ
	double mins[9];		//��Nά����Сֵ
	if (datas.empty())
	{
		cerr << "���ڽ���KD�������ݼ��ǿռ�" << endl;
		return;
	}
	//else
	//��ȡ���ݱ߽�
	for (int i = 0; i < 9; ++i)
		mins[i] = maxs[i] = (*(datas.begin()))->properties[i];
	for (KDDData* data : datas)
	{
		for (int i = 0; i < 9; i++)
		{
			maxs[i] = data->properties[i] > maxs[i] ? data->properties[i] : maxs[i];
			mins[i] = data->properties[i] < mins[i] ? data->properties[i] : mins[i];
		}
	}
	//�������
	for (int i = 0; i < 9; ++i)
	{
		if (maxs[i] - mins[i] == 0)
		{
			radios[i] = -1;
			continue;
		}
		radios[i] = (new_max - new_min) / (maxs[i] - mins[i]);
		for (KDDData* data : datas)
			data->properties[i] = (data->properties[i] - mins[i])*radios[i];//+new_min
	}
	//�ݹ齨��
	root = new KDTreeNode;
	for (KDDData* data : datas)
		root->value.push_back(data);
	buildTree(root);
}

inline void KDTree::buildTree(KDTreeNode* node) const
{//�ݹ齨��
	double maxs[9];		//��Nά�����ֵ
	double mins[9];		//��Nά����Сֵ
	double maxrange = 0;	//�����չ��
	if (node->value.size() <= bucket_size)
		return;
	//��ȡ���ݱ߽�
	for (int i = 0; i < 9; ++i)
		mins[i] = maxs[i] = node->value.front()->properties[i];
	for (KDDData* data : node->value)
	{
		for (int i = 0; i < 9; i++)
		{
			maxs[i] = data->properties[i] > maxs[i] ? data->properties[i] : maxs[i];
			mins[i] = data->properties[i] < mins[i] ? data->properties[i] : mins[i];
		}
	}
	//�ҳ����������չ�ȵ�ά
	for (int i = 0; i < 9; ++i)
	{
		if (maxrange < (maxs[i] - mins[i]))
		{
			maxrange = maxs[i] - mins[i];
			node->d = i;
		}
	}
	if (maxrange == 0)
		return;
	//�涨�ָ�㣬���㼯�ָ�
	node->spno = mins[node->d] + maxrange / 2;
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

inline void KDTree::test(set<KDDData*> testDatas)
{
	//��񻯲�������

	//todo 
}

inline int KDTree::getResult(KDDData testData)
{

	return OTHERS;
}
