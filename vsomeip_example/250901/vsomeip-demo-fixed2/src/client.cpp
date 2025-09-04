#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <array>
#include <vector>

namespace {
constexpr vsomeip::service_t    kService      = 0x0420;
constexpr vsomeip::instance_t   kInstance     = 0x0001;
constexpr vsomeip::method_t     kMethodEcho   = 0x0001;
constexpr vsomeip::event_t      kEventId      = 0x8001;
constexpr vsomeip::eventgroup_t kEventGroup   = 0x0001;
}

class Client {
public:
    Client()
    : app_(vsomeip::runtime::get()->create_application("demo_client")),
      available_(false) {}

    bool init() {
        if (!app_->init()) {
            std::cerr << "[CLIENT] app init failed\n";
            return false;
        }

        app_->register_state_handler([this](vsomeip::state_type_e s){
            std::cout << "[CLIENT] state=" << (s == vsomeip::state_type_e::ST_REGISTERED ? "REGISTERED":"DEREG") << std::endl;
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                app_->request_service(kService, kInstance);

                // request & subscribe to event group (explicit types)
                app_->request_event(
                    kService, kInstance, kEventId, {kEventGroup},
                    vsomeip::event_type_e::ET_EVENT,
                    vsomeip::reliability_type_e::RT_UNRELIABLE
                );
                app_->subscribe(kService, kInstance, kEventGroup);
            }
        });

        app_->register_availability_handler(kService, kInstance,
            [this](vsomeip::service_t, vsomeip::instance_t, bool av){
                available_ = av;
                std::cout << "[CLIENT] availability: " << (av ? "UP":"DOWN") << std::endl;
            });

        // method response
        app_->register_message_handler(kService, kInstance, kMethodEcho,
            [this](const std::shared_ptr<vsomeip::message>& rsp){
                if (rsp->get_message_type() == vsomeip::message_type_e::MT_RESPONSE) {
                    auto pl = rsp->get_payload();
                    const auto *d = pl->get_data();
                    const auto len = pl->get_length();
                    std::cout << "[CLIENT] echo resp len=" << len << " : ";
                    for (std::size_t i=0;i<len;++i) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)d[i] << " ";
                    std::cout << std::dec << std::endl;
                } else {
                    std::cout << "[CLIENT] unexpected response type" << std::endl;
                }
            });

        // event
        app_->register_message_handler(kService, kInstance, kEventId,
            [](const std::shared_ptr<vsomeip::message>& msg){
                auto pl = msg->get_payload();
                const auto *d = pl->get_data();
                const auto len = pl->get_length();
                std::cout << "[CLIENT] event len=" << len << " : ";
                for (std::size_t i=0;i<len;++i) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)d[i] << " ";
                std::cout << std::dec << std::endl;
            });

        // periodic echo
        worker_ = std::thread([this](){
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
            while (running_) {
                if (available_) send_echo();
                std::this_thread::sleep_for(3s);
            }
        });

        return true;
    }

    void start() { app_->start(); }
    void stop() {
        running_ = false;
        try { app_->stop(); } catch(...) {}
        if (worker_.joinable()) worker_.join();
    }

private:
    void send_echo() {
        auto rq = vsomeip::runtime::get()->create_request();
        rq->set_service(kService);
        rq->set_instance(kInstance);
        rq->set_method(kMethodEcho);

        static uint8_t v = 0;
        std::array<uint8_t, 4> bytes{0x11, 0x22, 0x33, v++};
        std::vector<vsomeip::byte_t> payload(bytes.begin(), bytes.end());
        auto pl = vsomeip::runtime::get()->create_payload();
        pl->set_data(payload);
        rq->set_payload(pl);

        std::cout << "[CLIENT] send echo" << std::endl;
        app_->send(rq);
    }

    std::shared_ptr<vsomeip::application> app_;
    std::atomic<bool> available_{false};
    std::atomic<bool> running_{true};
    std::thread worker_;
};

int main() {
    Client c;
    if (!c.init()) return 1;
    c.start();
    c.stop();
    return 0;
}