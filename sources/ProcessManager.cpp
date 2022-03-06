#include "ProcessManager.h"
#include <boost/process/extend.hpp>
#include <boost/process/windows.hpp>
#include <boost/process/async_pipe.hpp>
#include <iostream>

namespace pm
{
    ProcessManager::ProcessManager()
        : m_auId(0)
    {
        
    }

    int ProcessManager::Initialize(std::string& strErrInfo)
    {

        return 0;
    }

    int ProcessManager::StartProcess(const std::string& strName, const std::string& strProcessPath, const std::string& strArgs, std::string& strErrInfo)
    {
        std::error_code ec;
        //TODO:接管IO
        //boost::process::async_pipe pioe_out(*theIO);
        //boost::process::async_pipe pipe_in(*theIO);
        boost::process::opstream os;
        boost::process::ipstream is;

        auto spWrpProcess = std::make_shared<ProcessWrap>();
        std::weak_ptr<ProcessWrap> wspWrpProcess {spWrpProcess};

        auto spProcessHwnd = std::make_shared<boost::process::child>(
            strProcessPath,
            //boost::process::std_in < pipe_in,
            //(boost::process::std_out & boost::process::std_err) > pioe_out,
            boost::process::windows::show_normal,
            ec,
            *utils::io_service_pool::instance().pick_io_service(),
            /*
            ///This handler is invoked before the process in launched, to setup parameters. The required signature is `void(Exec &)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_setup_>   on_setup;
            ///This handler is invoked if an error occured. The required signature is `void(auto & exec, const std::error_code&)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_error_>   on_error;
            ///This handler is invoked if launching the process has succeeded. The required signature is `void(auto & exec)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_success_> on_success;
            */
            boost::process::extend::on_setup = 
                [wspWrpProcess](auto& exec) {

                std::cout << "Process setup thread id: "
                    << std::this_thread::get_id() << std::endl;
                exec.creation_flags |= boost::winapi::CREATE_NEW_CONSOLE_;
            },

            boost::process::extend::on_success =
                [wspWrpProcess](const auto& exec) {

                std::cout << "Process success thread id: "
                    << std::this_thread::get_id() << std::endl;

                std::shared_ptr<ProcessWrap> spWrpProcess;
                if(nullptr == (spWrpProcess = wspWrpProcess.lock()))
                    return;
                spWrpProcess->spLastUpTime = \
                    std::make_shared<std::chrono::system_clock::time_point>(\
                        std::chrono::system_clock::now()
                    );
            },

            boost::process::on_exit =
                [wspWrpProcess](int exit, const std::error_code& ec) {

                std::cout << "Process exited thread id: "
                    << std::this_thread::get_id() << std::endl;

                std::shared_ptr<ProcessWrap> spWrpProcess;
                if(nullptr == (spWrpProcess = wspWrpProcess.lock()))
                    return;
                spWrpProcess->spLastDownTime = \
                    std::make_shared<std::chrono::system_clock::time_point>(\
                        std::chrono::system_clock::now()
                    );

				std::cout << "exit_code:" << spWrpProcess->spProcess->exit_code() << std::endl;
            },

				boost::process::extend::on_error =
				[wspWrpProcess](auto& exec, const std::error_code& ec) {

				std::cout << "Process error thread id: "
					<< std::this_thread::get_id() << std::endl;

				std::shared_ptr<ProcessWrap> spWrpProcess;
				if (nullptr == (spWrpProcess = wspWrpProcess.lock()))
					return;

				std::cout << "exit_code:" << spWrpProcess->spProcess->exit_code() << std::endl;
			},
            ec
        );
        if (ec)
        {
            std::cerr << ec.message() << std::endl;
            strErrInfo = std::move(ec.message());
            return ec.value();    
        }

        unsigned int id = m_auId.fetch_add(1, std::memory_order::memory_order_relaxed);
        spWrpProcess->uId = id;
        spWrpProcess->strName = strName;
        spWrpProcess->strProcessPath = strProcessPath;
        spWrpProcess->spProcess = std::move(spProcessHwnd);

        std::lock_guard<std::mutex> lck(m_mtxProcess);
        m_mapProcess[id] = std::move(spWrpProcess);
        return 0;
    }

	int ProcessManager::StopProcess(unsigned int id, bool bRemove)
	{
		std::vector<unsigned int> vIds{ id };
		return StopProcess(vIds, bRemove);
	}

	int ProcessManager::StopProcess(const std::vector<unsigned int>& vIds, bool bRemove)
	{
		std::list<std::shared_ptr<ProcessWrap>> vTerminalProcList;
		std::lock_guard<std::mutex> lck(m_mtxProcess);
		for (const auto& id : vIds)
		{
			auto itF = m_mapProcess.find(id);
			if (itF == m_mapProcess.end())
				return 1;
			auto spWrpProcess = itF->second;
			if (bRemove) m_mapProcess.erase(itF);
			spWrpProcess->spProcess->terminate();
		}
		return 0;
	}

	int ProcessManager::Info()
    {
        std::lock_guard<std::mutex> lck(m_mtxProcess);
        for(const auto& iter : m_mapProcess)
        {
            auto&& spProcess = iter.second;
            std::cout << "name:" << spProcess->strName << ",process:" << spProcess->strProcessPath << std::endl;
        }
        return 0;
    }

}
