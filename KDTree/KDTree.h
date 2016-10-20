#pragma once
#include <vector>
#include <string>
//������Խ����
#define IS_NORMAL 0
#define IS_DOS 1
#define OTHERS 2

using namespace std;


struct KDTreeNode
{
	int d;			//�ָ�ά
	double v;		//�ָ��
	double max;		//���ֵ
	double min;		//��Сֵ
	KDTreeNode* lc = nullptr;	//С�ڵ��ڷָ��ķ�֧
	KDTreeNode* gc = nullptr;	//���ڷָ��ķ�֧
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

	return nullptr;
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
