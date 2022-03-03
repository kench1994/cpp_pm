#pragma once
#include "ctx.h"
#include <map>
#include <mutex>
#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <boost/process.hpp>
namespace pm
{
    enum class enProcessStatus{
        UP,
        DOWN
    };


    //TODO:PID, ProcessStatus, uptimes
    typedef struct tagProcessWrap{
        tagProcessWrap(): uId(0), spProcess(nullptr) ,spLastUpTime(nullptr), spLastDownTime(nullptr)
        {}
        unsigned int uId;
        std::string strName;
        std::string strProcessPath;
        std::string strArgs;
        std::shared_ptr<boost::process::child> spProcess;
        //TODO:直接成员变量还是用容器包裹?
        std::shared_ptr<std::chrono::system_clock::time_point> spLastUpTime, spLastDownTime;
        
    }ProcessWrap;

    //TODO:支持restart top delete  {namespace} 整片区域
    class ProcessManager{
        public:
            ProcessManager();
            ~ProcessManager(){}

            int Initialize(std::string& strErrInfo);
            //todo: startup mode ? runtime path?
            int StartProcess(const std::string& strName, const std::string& strProcessPath, const std::string& strArgs, std::string& strErrInfo);

			//TODO:加个kill信号接口
			int StopProcess(unsigned int id, bool bRemove = false);

			int StopProcess(const std::vector<unsigned int>& vIds, bool bRemove = false);
			
			//TODO:delete single / vecotr


            //输出到指定的stream??
            int Info();
        private:
            std::mutex m_mtxProcess;
            std::atomic<unsigned int> m_auId;
            std::map<unsigned int, std::shared_ptr<ProcessWrap>>m_mapProcess;
    };
}
