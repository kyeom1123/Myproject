#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>

namespace
{
    constexpr vsomeip::service_t kService = 0x0420;
    constexpr vsomeip::instance_t kInstance = 0x0001;
    constexpr vsomeip::method_t kMethodEcho = 0x0001;
    constexpr vsomeip::event_t kEventId = 0x8001;
    constexpr vsomeip::eventgroup_t kEventGroup = 0x0001;
}

class Client
{
public:
    Client()
        : app_(vsomeip::runtime::get()->create_application("demo_client")),
          available_(false) {}

    bool init()
    {
        if (!app_->init())
        {
            std::cerr << "[CLIENT] app init failed\n";
            return false;
        }

        // 상태 핸들러
        app_->register_state_handler([this](vsomeip::state_type_e s)
                                     {
            std::cout << "[CLIENT] state=" << (s == vsomeip::state_type_e::ST_REGISTERED ? "REGISTERED":"DEREG") << "\n";
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                app_->request_service(kService, kInstance);

                // 이벤트 그룹 구독 준비 + 구독 요청
                app_->request_event(kService, kInstance, kEventId, {kEventGroup}, true, true);
                app_->subscribe(kService, kInstance, kEventGroup);
            } });

        // 서비스 가용성 콜백
        app_->register_availability_handler(
            kService, kInstance,
            [this](vsomeip::service_t, vsomeip::instance_t, bool av)
            {
                available_ = av;
                std::cout << "[CLIENT] availability: " << (av ? "UP" : "DOWN") << "\n";
            });

        // 메서드 응답
        app_->register_message_handler(
            kService, kInstance, kMethodEcho,
            [this](const std::shared_ptr<vsomeip::message> &rsp)
            {
                if (rsp->get_message_type() == vsomeip::message_type_e::MT_RESPONSE)
                {
                    auto pl = rsp->get_payload();
                    const auto &d = pl->get_data();
                    std::cout << "[CLIENT] echo resp len=" << d.size() << " : ";
                    for (auto b : d)
                        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
                    std::cout << std::dec << "\n";
                }
                else
                {
                    std::cout << "[CLIENT] unexpected response type\n";
                }
            });

        // 이벤트 수신
        app_->register_message_handler(
            kService, kInstance, kEventId,
            [](const std::shared_ptr<vsomeip::message> &msg)
            {
                auto pl = msg->get_payload();
                const auto &d = pl->get_data();
                std::cout << "[CLIENT] event len=" << d.size() << " : ";
                for (auto b : d)
                    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
                std::cout << std::dec << "\n";
            });

        // 주기적으로 echo 호출
        worker_ = std::thread([this]()
                              {
            using namespace std::chrono_literals;
            // 런타임 시작을 기다렸다가 호출
            std::this_thread::sleep_for(2s);
            while (running_) {
                if (available_) send_echo();
                std::this_thread::sleep_for(3s);
            } });

        return true;
    }

    void start() { app_->start(); }
    void stop()
    {
        running_ = false;
        try
        {
            app_->stop();
        }
        catch (...)
        {
        }
        if (worker_.joinable())
            worker_.join();
    }

private:
    void send_echo()
    {
        auto rq = vsomeip::runtime::get()->create_request();
        rq->set_service(kService);
        rq->set_instance(kInstance);
        rq->set_method(kMethodEcho);

        static uint8_t v = 0;
        std::array<uint8_t, 4> payload{0x11, 0x22, 0x33, v++};
        auto pl = vsomeip::runtime::get()->create_payload();
        pl->set_data(payload);
        rq->set_payload(pl);

        std::cout << "[CLIENT] send echo\n";
        app_->send(rq);
    }

    std::shared_ptr<vsomeip::application> app_;
    std::atomic<bool> available_{false};
    std::atomic<bool> running_{true};
    std::thread worker_;
};

int main()
{
    Client c;
    if (!c.init())
        return 1;
    c.start();
    c.stop();
    return 0;
}
