#include "Analyzer.h"

int main()
{
	Analyzer* analyzer = new Analyzer();
	analyzer->input.open("ccode.txt", ios::in);
	analyzer->output.open("output.txt", ios::trunc);
	if (!analyzer->input.is_open())
	{
		cout << "open file fail!" << endl;
		analyzer->output.close();
		return -1;
	}
	analyzer->startAnalyzer();
	system("Pause");
	return 0;
}