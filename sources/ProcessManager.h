#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/process.hpp>

namespace utils
{
    namespace pm
    {
        class ProcessManager{
            public:
                ProcessManager(){}
                ~ProcessManager(){}

                int Initialize(std::string& strErrInfo);

                //todo: startup mode ? runtime path?
                int StartProcess(const std::string& strName, const std::string& strProcessPath);
            private:
                std::shared_ptr<boost::asio::io_context> m_spIO;
                std::shared_ptr<boost::asio::io>
                std::unordered_map<std::string, std::shared_ptr<boost::process::child>>m_mapProcess;
        };
    }
}