#include <string>
#include <iostream>
#include "ProcessManager.h"
#include <utils/io_service_pool.hpp>

int main()
{
    utils::io_service_pool::instance().run();

    std::string strErrInfo;
    pm::ProcessManager pm;
    pm.Initialize(strErrInfo);
    //pm.StartProcess();
    for(;;)
    {
        strErrInfo.clear();
        std::string strHolder, strName;
        std::cin >> strHolder;
        if("l" == strHolder || "list" == strHolder)
        {
            pm.Info();
            continue;
        }
		else if ("stop" == strHolder)
		{
			unsigned int uId;
			strHolder.clear();
			std::cin >> strHolder;
			pm.StopProcess(static_cast<unsigned int>(std::atoi(strHolder.data())));
			continue;
		}
        auto nPos = strHolder.find_last_of("\\");
        strName = std::string::npos == nPos ? strHolder : strHolder.substr(nPos + 1);
        pm.StartProcess(strName, strHolder, "", strErrInfo);
    }
    return 0;
}