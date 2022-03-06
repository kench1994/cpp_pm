#pragma once
#include <map>
#include <mutex>
#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <boost/process.hpp>
#include <utils/io_service_pool.hpp>

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
        //TODO:ֱ�ӳ�Ա������������������?
        std::shared_ptr<std::chrono::system_clock::time_point> spLastUpTime, spLastDownTime;
        
    }ProcessWrap;

    //TODO:֧��restart top delete  {namespace} ��Ƭ����
    class ProcessManager{
        public:
            ProcessManager();
            ~ProcessManager(){}

            int Initialize(std::string& strErrInfo);
            //todo: startup mode ? runtime path?
            int StartProcess(const std::string& strName, const std::string& strProcessPath, const std::string& strArgs, std::string& strErrInfo);

			//TODO:�Ӹ�kill�źŽӿ�
			int StopProcess(unsigned int id, bool bRemove = false);

			int StopProcess(const std::vector<unsigned int>& vIds, bool bRemove = false);
			
			//TODO:delete single / vecotr


            //�����ָ����stream??
            int Info();
        private:
            std::mutex m_mtxProcess;
            std::atomic<unsigned int> m_auId;
            std::map<unsigned int, std::shared_ptr<ProcessWrap>>m_mapProcess;
    };
}
