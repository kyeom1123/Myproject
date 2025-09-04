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

class Server
{
public:
    Server()
        : app_(vsomeip::runtime::get()->create_application("demo_server")),
          offered_(false), running_(true) {}

    bool init()
    {
        if (!app_->init())
        {
            std::cerr << "[SERVER] app init failed\n";
            return false;
        }

        // 상태 핸들러
        app_->register_state_handler([this](vsomeip::state_type_e s)
                                     {
            std::cout << "[SERVER] state=" << (s == vsomeip::state_type_e::ST_REGISTERED ? "REGISTERED":"DEREG") << "\n";
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                // 서비스 제공
                app_->offer_service(kService, kInstance);

                // 메서드/이벤트 등록
                app_->register_message_handler(
                    kService, kInstance, kMethodEcho,
                    std::bind(&Server::on_echo, this, std::placeholders::_1));

                // 이벤트 제공(UDP unreliable)
                app_->offer_event(kService, kInstance, kEventId, {kEventGroup}, true, true);
                offered_ = true;
            } });

        return true;
    }

    void start()
    {
        // 이벤트 주기 송신 스레드
        event_thread_ = std::thread([this]()
                                    {
            while (running_) {
                if (offered_) publish_event();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            } });

        // vSomeIP 런타임
        app_->start();
    }

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
        if (event_thread_.joinable())
            event_thread_.join();
    }

private:
    void on_echo(const std::shared_ptr<vsomeip::message> &rq)
    {
        auto pl = rq->get_payload();
        const auto &data = pl->get_data();

        std::cout << "[SERVER] Echo request len=" << data.size() << " : ";
        for (auto b : data)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
        std::cout << std::dec << "\n";

        // 그대로 돌려주는 응답
        auto resp = vsomeip::runtime::get()->create_response(rq);
        auto out = vsomeip::runtime::get()->create_payload();
        out->set_data(data); // 에코
        resp->set_payload(out);
        app_->send(resp);
    }

    void publish_event()
    {
        // 간단한 카운터 이벤트
        static uint32_t cnt = 0;
        std::array<uint8_t, 4> bytes{
            static_cast<uint8_t>((cnt >> 24) & 0xFF),
            static_cast<uint8_t>((cnt >> 16) & 0xFF),
            static_cast<uint8_t>((cnt >> 8) & 0xFF),
            static_cast<uint8_t>(cnt & 0xFF)};
        cnt++;

        auto msg = vsomeip::runtime::get()->create_notification();
        msg->set_service(kService);
        msg->set_instance(kInstance);
        msg->set_event(kEventId);

        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(bytes);
        msg->set_payload(payload);

        app_->notify(msg);
        std::cout << "[SERVER] event published cnt=" << (cnt - 1) << "\n";
    }

    std::shared_ptr<vsomeip::application> app_;
    std::thread event_thread_;
    std::atomic<bool> offered_;
    std::atomic<bool> running_;
};

int main()
{
    Server s;
    if (!s.init())
        return 1;
    s.start();
    s.stop();
    return 0;
}
