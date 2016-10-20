#pragma once
#include <vector>
#include <string>
//������Խ����
#define IS_NORMAL 0
#define IS_DOS 1
#define OTHERS 2

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
	int d;			//�ָ�ά
	double v;		//�ָ��
	double max;		//���ֵ
	double min;		//��Сֵ
	KDTreeNode* lc = NULL;	//С�ڵ��ڷָ��ķ�֧
	KDTreeNode* gc = NULL;	//���ڷָ��ķ�֧
	vector<KDDData> value;	//�����Ҷ��㣬�����ﱣ�����ݵ�
};

class KDTree
{
public:
	KDTree(vector<KDDData> datas);
	~KDTree();
	void test(vector<KDDData> testDatas);
	int getResult(KDDData testData);

private:
	KDTreeNode * buildTree();
	KDTreeNode * root;
};

KDTree::KDTree(vector<KDDData> datas)
{
	double new_max = 2;
	double new_min = 0;
	double maxs[9];
	double mins[9];
	if (datas.empty())
	{
		cerr << "���ڽ���KD�������ݼ��ǿռ�" << endl;
		return;
	}
	for (vector<KDDData>::iterator data = datas.begin();data != datas.end();++data)
	{
		for (int i = 0; i < 9; i++)
		{
			maxs[i] = data->properties[i];
			mins[i] = data->properties[i];
		}
	}
	//TODO:���
	
	//todo �ݹ齨��
}

KDTreeNode * KDTree::buildTree()
{//�ݹ齨��

	return NULL;
}

KDTree::~KDTree()
{

}

inline void KDTree::test(vector<KDDData> testDatas)
{

}

inline int KDTree::getResult(KDDData testData)
{

	return OTHERS;
}
