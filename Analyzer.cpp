#include "Analyzer.h"
#include<cstring>

Analyzer::Analyzer()
{
	// 初始化
	idenCount = 0;
	kwCount = 0;
	signCount = 0;
	errCount = 0;
	identry = -1;
	state = 0;
	C = '\0';
	isKey = -1;
	currentRow = 1;
	preCol = 0;
	currentCol = 0;
	token = new char[MAX_WORD_SIZE];
	buffer = new char[2 * HALF_BUFFER_SIZE];
	buffer[HALF_BUFFER_SIZE - 1] = 0;
	buffer[2 * HALF_BUFFER_SIZE - 1] = 0;
	forward = buffer;

	// 创建链表
	headPtr = (errorInfo*)malloc(sizeof(errorInfo));
	tailPtr = headPtr;
	if (headPtr == nullptr) return;
	tailPtr->row = -1;
	tailPtr->column = -1;
	tailPtr->errorType = -1;
	tailPtr->nextError = nullptr;

	// 设置cout精度
	cout.precision(12);
}

Analyzer::~Analyzer()
{
	// 关闭流指针，释放链表
	input.close();
	output.close();

	errorInfo* ptr = headPtr;
	errorInfo* nextPtr = headPtr->nextError;
	while (nextPtr != nullptr) {
		free(ptr);
		ptr = nextPtr;
		nextPtr = nextPtr->nextError;
	}
	free(headPtr);
	free(tailPtr);
}

void Analyzer::startAnalyzer(void)
{
	// 填充buffer左半区
	fillBuffer(LEFT_AREA);
	do
	{
		switch (state)
		{
		case 0:									//----- 初始状态
			flush_token();
			get_char();
			get_nbc();
			switch (C)
			{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				state = 1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				state = 2; break;
			case '<':
				state = 8; break;
			case '>':
				state = 9; break;
			case '#':
				state = 14; break;
			case '!':
				state = 10; break;
			case '?':
				state = 0;
				_return("?", "-");// 识别出 ?
				break;
			case ':':
				state = 0;
				_return(":", "-");// 识别出 :
				break;
			case '=':
				get_char();
				if (C == '=') {
					state = 0;
					_return("relop", "EQ");// 识别出 ==
				}
				else {
					retract();
					state = 0;
					_return("=", "-");// 识别出 =
				}
				break;
			case '+':
				get_char();
				if (C == '+')
					_return("++", "-");// 识别出 ++
				else if (C == '=')
					_return("+=", "-");// 识别出 +=
				else {
					retract();
					_return("+", "-");// 识别出 +
				}
				state = 0;
				break;
			case '-':
				get_char();
				if (C == '-')
					_return("--", "-");// 识别出 --
				else if (C == '=')
					_return("-=", "-");// 识别出 -=
				else if (C == '>')
					_return("->", "-");// 识别出 ->
				else {
					retract();
					_return("-", "-");// 识别出 -
				}
				state = 0;
				break;
			case '*':
				get_char();
				if (C == '=')
					_return("*=", "-");// 识别出 *=
				else {
					retract();
					_return("*", "-");// 识别出 *
				}
				state = 0;
				break;
			case '/':
				state = 11; break;
			case '%':
				get_char();
				if (C == '=')
					_return("%=", "-");// 识别出 %=
				else {
					retract();
					_return("%", "-");// 识别出 %
				}
				state = 0;
				break;
			case '&':
				get_char();
				if (C == '=')
					_return("&=", "-");// 识别出 &=
				else if (C == '&')
					_return("logop", "AND");// 识别出 &&
				else {
					retract();
					_return("&", "-");// 识别出 &
				}
				state = 0;
				break;
			case '|':
				get_char();
				if (C == '=')
					_return("|=", "-");// 识别出 |=
				else if (C == '|')
					_return("logop", "OR");// 识别出 ||
				else {
					retract();
					_return("|", "-");// 识别出 |
				}
				state = 0;
				break;
			case '~':
				state = 0;
				_return("~", "-");// 识别出 ~
				break;
			case '^':
				get_char();
				if (C == '=')
					_return("^=", "-");// 识别出 ^=
				else {
					retract();
					_return("^", "-");// 识别出 ^
				}
				state = 0;
				break;
			case '.':
				state = 0;
				_return(".", "-");// 识别出 .
				break;
			case '(':
				state = 0;
				_return("(", "-");// 识别出 (
				break;
			case ')':
				state = 0;
				_return(")", "-");// 识别出 )
				break;
			case '[':
				state = 0;
				_return("[", "-");// 识别出 [
				break;
			case ']':
				state = 0;
				_return("]", "-");// 识别出 ]
				break;
			case '{':
				state = 0;
				_return("{", "-");// 识别出 {
				break;
			case '}':
				state = 0;
				_return("}", "-");// 识别出 }
				break;
			case ',':
				state = 0;
				_return(",", "-");// 识别出 ,
				break;
			case ';':
				state = 0;
				_return(";", "-");// 识别出 ;
				break;
			case '\'':
				cat();
				get_char();
				if (C == '\\') {
					cat();
					get_char();
					cat();
					get_char();
				}
				else {
					cat();
					get_char();
				}
				if (C == '\'') {
					cat();
					_return("char", string(token));// 识别到常量字符
				}
				else
					_error(TypeError::SIGNILLE);
				state = 0;
				break;
			case '"':
				cat();
				identifyString();
				state = 0;
				break;
			default:
				if(C!=0)
					_error(TypeError::SIGNILLE);
				break;
			}
			break;
		case 1:									//----- 标识符状态
			cat();
			get_char();
			if (letter() || digit() || C == '_')
				state = 1;
			else {
				retract();
				state = 0;
				isKey = reserve();
				if (isKey != -1) {
					kwCount++;
					_return(keyWord[isKey], "-");// 识别出关键字
				}
				else {
					identry = table_insert();
					_return("id", symTable[identry]);// 识别出标识符
				}
			}
			break;
		case 2:									//----- 常数状态
			cat();
			get_char();
			switch (C)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				state = 2;
				break;
			case '.':
				state = 3;
				break;
			case 'E':
				state = 5;
				break;
			default:
				retract();
				state = 0;
				_return("num", SToI());// 识别出整数
				break;
			}
			break;
		case 3:									//----- 小数点状态
			cat();
			get_char();
			if (digit())
				state = 4;
			else {
				_error(TypeError::FRACILLE);
				state = 0;
			}
			break;
		case 4:									//----- 小数状态
			cat();
			get_char();
			switch (C)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				state = 4;
				break;
			case 'E':
			case 'e':
				state = 5;
				break;
			default:
				if (C == 0 || C == ' ' || C == ';' || C == ',' || C == '\t' || C == '\n' || C == '+'
					|| C == '-' || C == '*' || C == '/') {
					retract();
					_return("num", SToF());// 识别出小数
				}
				else
					_error(TypeError::FRACILLE);
				state = 0;
				break;
			}
			break;
		case 5:									//----- 指数状态
			cat();
			get_char();
			switch (C)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				state = 7;
				break;
			case '+':
			case '-':
				state = 6;
				break;
			default:
				_error(TypeError::SCINILLE);
				state = 0;
				break;
			}
			break;
		case 6:
			cat();
			get_char();
			if (digit())
				state = 7;
			else {
				_error(TypeError::SCINILLE);
				state = 0;
			}
			break;
		case 7:
			cat();
			get_char();
			if (digit())
				state = 7;
			else {
				if (C != 0 && C != ' ' && C != ';' && C != ',' && C != '\t' && C != '\n')
					_error(TypeError::SCINILLE);
				else {
					retract();
					_return("num", string(token));// 识别出无符号数

				}
				state = 0;
			}
			break;
		case 8:									//----- '<' 状态
			cat();
			get_char();
			switch (C)
			{
			case '<':
				get_char();
				if (C == '=')
					_return("shiop", "LSE");// 识别出 <<=
				else {
					retract();
					_return("shiop", "LS");// 识别出 <<
				}
				state = 0;
				break;
			case '=':
				state = 0;
				_return("relop", "LE");// 识别出 <=
				break;
			default:
				retract();
				state = 0;
				_return("relop", "LT");// 识别出 <
				break;
			}
			break;
		case 9:									//----- '>' 状态
			cat();
			get_char();
			switch (C)
			{
			case '>':
				get_char();
				if (C == '=')
					_return("shiop", "RSE");// 识别出 >>=
				else {
					retract();
					_return("shiop", "RS");// 识别出 >>
				}
				state = 0;
				break;
			case '=':
				state = 0;
				_return("relop", "GE");// 识别出 >=
				break;
			default:
				retract();
				state = 0;
				_return("relop", "GT");// 识别出 >
				break;
			}
			break;
		case 10:								//----- '!'状态
			get_char();
			if (C == '=')
				_return("relop", "NE");// 识别出 !=
			else {
				retract();
				_return("logop", "NOT");// 识别出 !
			}
			state = 0;
			break;
		case 11:								//----- '/'状态
			get_char();
			if (C == '*')		// 多行注释开头
				state = 12;
			else if (C == '/')	// 单行注释开头
				state = 13;
			else {
				retract();
				state = 0;
				_return("/", "-");// 识别出 /
			}
			break;
		case 12:								//----- 多行注释状态
			get_char();
			while (C != '*')
				get_char();
			get_char();
			if (C == '/')
				state = 0;
			else
				state = 12;
			break;
		case 13:								//----- 单行注释状态
			get_char();
			while (C != '\n' && C != 0)
				get_char();
			state = 0;
			break;
		case 14:								//----- '#'预编译状态
			get_char();
			while (C != '\n')
				get_char();
			state = 0;
			break;
		default:
			cout << "出现自动机不存在的状态，终止执行..." << endl;
			C = 0;
			break;
		}
	} while (C != 0);
	printInfo();
}

void Analyzer::flush_token(void)
{
	memset(token, 0, MAX_WORD_SIZE);
}

void Analyzer::fillBuffer(int area)
{
	if (area == LEFT_AREA)
		input.read(buffer, HALF_BUFFER_SIZE - 1);
	else if (area == RIGHT_AREA)
		input.read(buffer + HALF_BUFFER_SIZE, HALF_BUFFER_SIZE - 1);
	signCount += input.gcount();	// 字符总数加上文件流指针上一次读入的字符
	if (input.eof())
	{
		// 当遇到文件尾时需要手动向缓冲区中加入结束标志
		if (area == LEFT_AREA)
			buffer[input.gcount()] = 0;
		else if (area == RIGHT_AREA)
			buffer[HALF_BUFFER_SIZE + input.gcount()] = 0;
	}
}

void Analyzer::get_char(void)
{
	C = *forward;
	preCol = currentCol;
	currentCol++;				// 列数加一
	if (C == '\n') {
		currentRow++;			// 行数加一
		currentCol = 0;			// 列数置一
	}
	forward++;
	if (*forward == 0)
	{
		if (forward == buffer + HALF_BUFFER_SIZE - 1) {
			// forward 指向左半区终点,读入字符串填充右半区
			fillBuffer(RIGHT_AREA);
			forward++;
		}
		else if (forward == buffer + 2 * HALF_BUFFER_SIZE - 1) {
			// forward 指向右半区终点,读入字符串填充左半区
			fillBuffer(LEFT_AREA);
			forward = buffer;
		}
	}
}

void Analyzer::get_nbc(void)
{
	while (C == ' ' || C == '\t' || C == '\n')
		get_char();
}

void Analyzer::identifyString(void)
{
	int _state = 0;
	bool endMatch = false;	// 匹配结束
	bool newLine = false;

	// 匹配到完整的字符串、遇到文件尾、token已满的情况下才停止匹配
	while (!endMatch && C != 0 && token[MAX_WORD_SIZE - 2] == 0) {
		switch (_state)
		{
		case 0:
			get_char();
			if (C == '\\') {
				cat();
				_state = 1;
			}
			else if (C == '\"') {
				cat();
				endMatch = true;
			}
			else if (C == '\n') {
				newLine = true;
				_state = 0;
			}
			else {
				if (C != 0)
					cat();
				_state = 0;
			}
			break;
		case 1:
			get_char();
			while (C == '\n') {
				get_char();
				newLine = true;
			}
			cat();
			_state = 0;
			break;
		}
	}
	if (newLine)
		_error(TypeError::STRILLE);
	else
		_return("string", string(token));// 识别到常量字符串
}

void Analyzer::cat(void)
{
	int i = 0;
	while (i < MAX_WORD_SIZE - 1 && token[i] != '\0') {
		i++;
	}
	if (i < MAX_WORD_SIZE - 1) {
		token[i] = C;
	}
	else
		_error(TypeError::OVERTOKEN);
}

bool Analyzer::letter(void)
{
	if ((C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z'))
		return true;
	else
		return false;
}

bool Analyzer::digit(void)
{
	if (C >= '0' && C <= '9')
		return true;
	else
		return false;
}

void Analyzer::retract(void)
{
	if (forward == buffer) {
		// forward 为左半区起点，回退到右半区终点，同时文件流指针回退
		forward = buffer + 2 * HALF_BUFFER_SIZE - 2;
		input.seekg(-input.gcount(), ios::cur);
		signCount -= input.gcount();
		currentCol = preCol;
	}
	else if (forward == buffer + HALF_BUFFER_SIZE)
	{
		forward -= 2;
		currentCol -= 2;
	}
	else {
		forward--;
		currentCol--;
	}
	// 若回退后的向前指针指向换行符，记录的行数减一
	if (*forward == '\n')
		currentRow--;
}

int Analyzer::reserve(void)
{
	for (unsigned int pos = 0; pos < keyWord.size(); pos++) {
		if (keyWord[pos] == string(token))
			return pos;
	}
	return -1;
}

int Analyzer::SToI(void)
{
	return atoi(token);
}

double Analyzer::SToF(void)
{
	return atof(token);
}

int Analyzer::table_insert(void)
{
	idenCount++;
	for (unsigned int pos = 0; pos < symTable.size(); pos++) {
		if (symTable[pos] == string(token))
			return pos;
	}
	// 只将符号表中不存在的标识符压入符号表
	symTable.push_back(string(token));
	return symTable.size() - 1;
}

void Analyzer::_error(int errorType)
{
	// 保存错误出现位置与类型
	tailPtr->nextError = (errorInfo*)malloc(sizeof(errorInfo));
	tailPtr = tailPtr->nextError;
	if (tailPtr == nullptr) {
		cout << "malloc failed!" << endl;
		return;
	}
	tailPtr->row = currentRow;
	tailPtr->column = currentCol;
	tailPtr->errorType = errorType;
	tailPtr->nextError = nullptr;

	output << "ERROR in row " << currentRow << ",column " << currentCol << " : ";
	// 按照 error 类型处理错误
	switch (errorType)
	{
	case TypeError::STRILLE:		// 常量字符串非法
		output << "A newline character appears in a constant string!" << endl;
		break;
	case TypeError::SIGNILLE:		// 非法符号
		output << "Illegal sign!" << endl;
		while (C != 0 && C != ' ' && C != ';' && C != ',' && C != '\t' && C != '\n')
			get_char();
		break;
	case TypeError::FRACILLE:		// 小数部分错误
		output << "Illegal fractional part!" << endl;
		while (C != 0 && C != ' ' && C != ';' && C != ',' && C != '\t' && C != '\n')
			get_char();
		break;
	case TypeError::SCINILLE:		// 科学计数法使用错误
		output << "Illegal science notation!" << endl;
		while (C != 0 && C != ' ' && C != ';' && C != ',' && C != '\t' && C != '\n')
			get_char();
		break;
	case TypeError::OVERTOKEN:		// 标识符太长
		output << "The length of word exceeds maximum length of token!!!" << endl;
		while (C != 0 && C != ' ' && C != ';' && C != ',' && C != '\t' && C != '\n')
			get_char();
		retract();
		break;
	default:
		output << "Function _error() index error!" << endl;
		break;
	}
	errCount++;			// 错误数加一
}

void Analyzer::printInfo(void)
{
	cout << "语句总行数：" << currentRow << endl;
	cout << "标识符总数：" << idenCount << endl;
	cout << "关键字总数：" << kwCount << endl;
	cout << "字符总数：" << signCount << endl;
	cout << "错误总数：" << errCount << endl << endl;

	output << "\n----------------------------------------" << endl;
	output << "语句总行数：" << currentRow << endl;
	output << "标识符总数：" << idenCount << endl;
	output << "关键字总数：" << kwCount << endl;
	output << "字符总数：" << signCount << endl;
	output << "错误总数：" << errCount << endl << endl;

	// 打印链表中统计的错误位置与类型
	errorInfo* ptr = headPtr;
	while (ptr->nextError != nullptr) {
		ptr = ptr->nextError;
		cout << "Error in row " << ptr->row << ",column " << ptr->column << " : ";
		output << "Error in row " << ptr->row << ",column " << ptr->column << " : ";

		if (ptr->errorType == TypeError::STRILLE) {
			cout << "A newline character appears in a constant string!" << endl;
			output << "A newline character appears in a constant string!" << endl;
		}
		else if (ptr->errorType == TypeError::SIGNILLE) {
			cout << "Illegal sign!" << endl;
			output << "Illegal sign!" << endl;
		}
		else if (ptr->errorType == TypeError::FRACILLE) {
			cout << "Illegal fractional part!" << endl;
			output << "Illegal fractional part!" << endl;
		}
		else if (ptr->errorType == TypeError::SCINILLE) {
			cout << "Illegal science notation!" << endl;
			output << "Illegal science notation!" << endl;
		}
		else {
			cout << "The length of word exceeds maximum length of token!!!" << endl;
			output << "The length of word exceeds maximum length of token!!!" << endl;
		}
	}
	output << "----------------------------------------" << endl;
}

template<typename T> void Analyzer::_return(string sign, T attr)
{
	output << "<" << sign << " , " << attr << ">" << endl;
}