#include <iostream>
#include "../libwfc/libwfc.h"

void DoContextInited() {
	std::cout << "DoContextInited \n";
	wfcBrowser *pBrowser = CreateBrowser(0, "www.baidu.com", 0);
}

int main()
{
	RunSingleCef(DoContextInited);
	std::cout << "Exit \n";
}
