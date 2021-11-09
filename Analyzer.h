#pragma once
#include<cstdlib>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

#define HALF_BUFFER_SIZE 4096		// �����������С 4KB
#define MAX_WORD_SIZE 256			// ������󳤶�
#define LEFT_AREA 0					// �����
#define RIGHT_AREA 1				// �Ұ���

// ���������Ϣ
typedef struct errorInfo{
	int row;						// ����������
	int column;						// ����������
	int errorType;					// ��������
	struct errorInfo* nextError;
}errorInfo;

// ��������ö����
enum TypeError {
	STRILLE,		// �����ַ����Ƿ����м���ֻ��з���
	SIGNILLE,		// �Ƿ�����
	FRACILLE,		// �Ƿ�С��
	SCINILLE,		// �Ƿ���ѧ������
	OVERTOKEN		// ��ʶ�����ȳ����������󳤶�
};

class Analyzer
{
public:
	explicit Analyzer();			// ���캯��

	ifstream input;					// ���ļ���ָ��
	ofstream output;				// д�ļ���ָ��
	void startAnalyzer(void);		// �� main �Ľӿڣ������ʷ�������
private:
	~Analyzer();					// ��������

	vector<string> symTable;		// ���ű����ʶ��
	vector<string> keyWord = { "auto","break","case","char","const","continue","default","do",
								"double","else","enum","extern","float","for","goto","if",
								"int","long","register","return","short","signed","sizeof","static",
								"struct","switch","typedef","union","unsigned","void","volatile","while" };

	errorInfo* headPtr;				// ������Ϣ��ͷ
	errorInfo* tailPtr;				// ������Ϣ��β
	int idenCount;					// ��ʶ������
	int kwCount;					// �ؼ�������
	int errCount;					// ��������
	long long signCount;			// �ַ�����
	int currentRow;					// ��ǰ��
	int preCol;						// ��ǰ�У���ֹ���˺�������һ�ж�ʧ����
	int currentCol;					// ��ǰ��

	int state;						// ��ǰ�Զ���״̬
	char C;							// ��ǰ������ַ�
	int identry;					// �ݴ��ʶ���ڷ��ű��е�λ��
	int isKey;						// ָ����ǰ token �е��ʵ�����
	char* token;					// ��ǰ����ʶ��ĵ����ַ���
	char* forward;					// ��ǰָ�룬��һ��Ҫ������ַ�λ��
	char* buffer;					// ���뻺��������Ϊ��������Ұ���
	
	void flush_token(void);			// ��� token
	void fillBuffer(int area);		// ��仺������area=0 ʱ����������area=1 ʱ����Ұ���
	void identifyString(void);		// ʶ�����ַ������ж��Ƿ�Ϸ�
	void get_char(void);			// ���� forward �����뻺�����ж���һ���ַ���Ȼ���ƶ� forward ʹ��ָ����һ���ַ�
	void get_nbc(void);				// ʹ C ���ַ��ǿո񡢷ǻ��з�
	void cat(void);					// �� C ���ַ������� token ���ַ���ĩβ
	void retract(void);				// forward ����һ���ַ�
	bool letter(void);				// �ж� C ���ַ��Ƿ�Ϊ��ĸ
	bool digit(void);				// �ж� C ���ַ��Ƿ�Ϊ����
	int reserve(void);				// ���� token �е��ַ�����ؼ��ֱ����ǹؼ��֣����عؼ��ֱ��±ꣻ���򣬷���-1
	int table_insert(void);			// ���Զ����ʶ��������ű����ط��ű��е�λ��ָ��
	int SToI(void);					// �� token �е��ַ���ת��Ϊ����
	double SToF(void);				// �� token �е��ַ���ת��Ϊ������
	void printInfo(void);			// �����ʶ���������ؼ���������ͳ����Ϣ
	void _error(int errorType);		// ���ղ������ֵĸ��ִ���
	template<typename T> void _return(string sign,T attr);		// ��ʶ������ĵ��� <�Ǻ�, ����> ��Ԫ���������Ļ���ļ�
};