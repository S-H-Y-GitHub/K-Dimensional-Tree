#pragma once
#include <vector>
#include <string>

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

struct KDTreeSpliter
{
	int d;

};

class KDTree
{
public:
	KDTree(vector<KDDData> datas);
	~KDTree();
	
private:

};

KDTree::KDTree(vector<KDDData> datas)
{

}

KDTree::~KDTree()
{
}
