
#pragma once
#include <memory>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_service_strand.hpp>
namespace service
{
    typedef struct tagContext{
        std::shared_ptr<boost::asio::io_service> spIO;
        std::shared_ptr<boost::asio::io_service::work> spWork;
        std::shared_ptr<boost::asio::io_service::strand> spStrand;
    }Ctx;

    extern std::unique_ptr<service::Ctx> theCtx;
}

