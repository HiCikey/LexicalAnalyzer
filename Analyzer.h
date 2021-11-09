#pragma once
#include<cstdlib>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

#define HALF_BUFFER_SIZE 4096		// 半个缓冲区大小 4KB
#define MAX_WORD_SIZE 256			// 单词最大长度
#define LEFT_AREA 0					// 左半区
#define RIGHT_AREA 1				// 右半区

// 保存错误信息
typedef struct errorInfo{
	int row;						// 错误所在行
	int column;						// 错误所在列
	int errorType;					// 错误类型
	struct errorInfo* nextError;
}errorInfo;

// 错误类型枚举量
enum TypeError {
	STRILLE,		// 常量字符串非法（中间出现换行符）
	SIGNILLE,		// 非法符号
	FRACILLE,		// 非法小数
	SCINILLE,		// 非法科学计数法
	OVERTOKEN		// 标识符长度超过允许的最大长度
};

class Analyzer
{
public:
	explicit Analyzer();			// 构造函数

	ifstream input;					// 读文件流指针
	ofstream output;				// 写文件流指针
	void startAnalyzer(void);		// 与 main 的接口，启动词法分析器
private:
	~Analyzer();					// 析构函数

	vector<string> symTable;		// 符号表，存标识符
	vector<string> keyWord = { "auto","break","case","char","const","continue","default","do",
								"double","else","enum","extern","float","for","goto","if",
								"int","long","register","return","short","signed","sizeof","static",
								"struct","switch","typedef","union","unsigned","void","volatile","while" };

	errorInfo* headPtr;				// 错误信息表头
	errorInfo* tailPtr;				// 错误信息表尾
	int idenCount;					// 标识符总数
	int kwCount;					// 关键字总数
	int errCount;					// 错误总数
	long long signCount;			// 字符总数
	int currentRow;					// 当前行
	int preCol;						// 先前列，防止回退后跳回上一行丢失列数
	int currentCol;					// 当前列

	int state;						// 当前自动机状态
	char C;							// 当前读入的字符
	int identry;					// 暂存标识符在符号表中的位置
	int isKey;						// 指明当前 token 中单词的类型
	char* token;					// 当前正在识别的单词字符串
	char* forward;					// 向前指针，下一个要读入的字符位置
	char* buffer;					// 输入缓冲区，分为左半区和右半区
	
	void flush_token(void);			// 清空 token
	void fillBuffer(int area);		// 填充缓冲区，area=0 时填充左半区，area=1 时填充右半区
	void identifyString(void);		// 识别常量字符串，判断是否合法
	void get_char(void);			// 根据 forward 从输入缓冲区中读入一个字符，然后移动 forward 使其指向下一个字符
	void get_nbc(void);				// 使 C 中字符非空格、非换行符
	void cat(void);					// 将 C 中字符连接在 token 中字符串末尾
	void retract(void);				// forward 后退一个字符
	bool letter(void);				// 判断 C 中字符是否为字母
	bool digit(void);				// 判断 C 中字符是否为数字
	int reserve(void);				// 根据 token 中的字符串查关键字表。若是关键字，返回关键字表下标；否则，返回-1
	int table_insert(void);			// 将自定义标识符插入符号表，返回符号表中的位置指针
	int SToI(void);					// 将 token 中的字符串转换为整数
	double SToF(void);				// 将 token 中的字符串转换为浮点数
	void printInfo(void);			// 输出标识符总数、关键字总数等统计信息
	void _error(int errorType);		// 接收并处理发现的各种错误
	template<typename T> void _return(string sign,T attr);		// 将识别出来的单词 <记号, 属性> 二元组输出到屏幕与文件
};