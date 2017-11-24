#include "../libwfc/libwfc.h"

int main()
{
	InitCef(SINGLE_PROCESS|SINGLE_THREAD_MSGLOOP);
	
	wfcBrowser *pBrowser = CreateBrowser(0, "www.baidu.com", 0);
	
	RunMsgLoopCef();
	
	CoUnInitializeCef();
}