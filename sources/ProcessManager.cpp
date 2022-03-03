#include "ProcessManager.h"
#include <boost/process/extend.hpp>
#include <boost/process/windows.hpp>
#include <boost/process/async_pipe.hpp>
#include <iostream>

namespace pm
{
    int ProcessManager::Initialize(std::string& strErrInfo)
    {

        return 0;
    }

    int ProcessManager::StartProcess(const std::string& strName, const std::string& strProcessPath, std::string& strErrInfo)
    {
        std::error_code ec;
        //TODO:½Ó¹ÜIO
        //boost::process::async_pipe pioe_out(*theIO);
        //boost::process::async_pipe pipe_in(*theIO);
        boost::process::opstream os;
        boost::process::ipstream is;
        auto spChildProcess = std::make_shared<boost::process::child>(
            //boost::process::shell,
            strProcessPath,
            //"cmd.exe",
            //"C:\\Users\\kench\\Documents\\WeChat Files\\wxid_y9ar5ahmtw1c22\\FileStorage\\File\\2021-12\\nng_pair0_2021-1216-151437.exe",
            //boost::process::std_in < pipe_in,
            //(boost::process::std_out & boost::process::std_err) > pioe_out,
            boost::process::windows::show_normal,
            ec,
            *(service::theCtx->spIO),
            /*
            ///This handler is invoked before the process in launched, to setup parameters. The required signature is `void(Exec &)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_setup_>   on_setup;
            ///This handler is invoked if an error occured. The required signature is `void(auto & exec, const std::error_code&)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_error_>   on_error;
            ///This handler is invoked if launching the process has succeeded. The required signature is `void(auto & exec)`, where `Exec` is a template parameter.
            constexpr boost::process::detail::make_handler_t<boost::process::detail::on_success_> on_success;
            */
            boost::process::extend::on_setup = 
                [](auto& exec) {
                std::cout << "Process setup thread id: "
                    << std::this_thread::get_id() << std::endl;
                exec.creation_flags |= boost::winapi::CREATE_NEW_CONSOLE_;
            },
            boost::process::extend::on_success =
                [](const auto& exec) {
                std::cout << "Process success thread id: "
                    << std::this_thread::get_id() << std::endl;
            },
            boost::process::on_exit =
                [](int exit, const std::error_code& ec_in) {
                std::cout << "Process exited thread id: "
                    << std::this_thread::get_id() << std::endl;
            },
            boost::process::extend::on_error =
                [](auto&, const std::error_code& errorCode) {
                std::cout << "Process error thread id: "
                    << std::this_thread::get_id() << std::endl;
                //session->result.errorCode = errorCode;
                //session->conclude();
            },
            ec
        );
        if (ec)
        {
            std::cerr << ec.message() << std::endl;
            strErrInfo = std::move(ec.message());
            return ec.value();    
        }

        std::lock_guard<std::mutex> lck(m_mtxProcess);
        m_mapProcess.emplace(strName, std::make_shared<ProcessWrap>(strName, strProcessPath, spChildProcess));
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
