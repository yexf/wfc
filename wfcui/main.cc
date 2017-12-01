#include <iostream>
#include "../libwfc/libwfc.h"

void DoContextInited() {
	std::cout << "DoContextInited \n";
	wfcBrowser *pBrowser = CreateBrowser(0, "www.baidu.com", 0);
}

int main()
{
	RegAppHandle(DoContextInited);

	InitCef(SINGLE_THREAD_MSGLOOP);

	RunMsgLoopCef();
	
	CoUnInitializeCef();

	std::cout << "Exit \n";
}
