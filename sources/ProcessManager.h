#pragma once
#include "ctx.h"
#include <mutex>
#include <memory>
#include <string>
#include <unordered_map>
#include <boost/process.hpp>
namespace pm
{
    //TODO:PID, ProcessStatus, uptimes
    typedef struct tagProcessWrap{
        tagProcessWrap(){}
        tagProcessWrap(std::string name, std::string path, std::shared_ptr<boost::process::child>p)
            : strName(std::move(name)),\
              strProcessPath(std::move(path)),\
              spProcess(std::move(p))
        {}

        std::string strName;
        std::string strProcessPath;
        std::shared_ptr<boost::process::child> spProcess;
    }ProcessWrap;
    class ProcessManager{
        public:
            ProcessManager(){}
            ~ProcessManager(){}

            int Initialize(std::string& strErrInfo);
            //todo: startup mode ? runtime path?
            int StartProcess(const std::string& strName, const std::string& strProcessPath, std::string& strErrInfo);

            //输出到指定的stream??
            int Info();
        private:
            std::mutex m_mtxProcess;
            std::unordered_map<std::string, std::shared_ptr<ProcessWrap>>m_mapProcess;
    };
}
