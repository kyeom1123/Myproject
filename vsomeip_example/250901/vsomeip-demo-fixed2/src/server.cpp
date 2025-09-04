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

class Server {
public:
    Server()
    : app_(vsomeip::runtime::get()->create_application("demo_server")),
      offered_(false), running_(true) {}

    bool init() {
        if (!app_->init()) {
            std::cerr << "[SERVER] app init failed\n";
            return false;
        }

        app_->register_state_handler([this](vsomeip::state_type_e s){
            std::cout << "[SERVER] state=" << (s == vsomeip::state_type_e::ST_REGISTERED ? "REGISTERED":"DEREG") << std::endl;
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                // Offer service
                app_->offer_service(kService, kInstance);

                // Register method handler
                app_->register_message_handler(
                    kService, kInstance, kMethodEcho,
                    std::bind(&Server::on_echo, this, std::placeholders::_1));

                // Offer event (explicit event type & reliability)
                app_->offer_event(
                    kService, kInstance, kEventId, {kEventGroup},
                    vsomeip::event_type_e::ET_EVENT,               // event type
                    std::chrono::milliseconds::zero(),             // cycle (unused here)
                    false, false, {},                               // change/resend settings
                    vsomeip::reliability_type_e::RT_UNRELIABLE     // UDP
                );

                offered_ = true;
            }
        });

        return true;
    }

    void start() {
        // periodic event thread
        event_thread_ = std::thread([this](){
            while (running_) {
                if (offered_) publish_event();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        });

        app_->start();
    }

    void stop() {
        running_ = false;
        try { app_->stop(); } catch (...) {}
        if (event_thread_.joinable()) event_thread_.join();
    }

private:
    void on_echo(const std::shared_ptr<vsomeip::message>& rq) {
        auto pl = rq->get_payload();
        const auto *data = pl->get_data();
        const auto len = pl->get_length();

        std::cout << "[SERVER] Echo request len=" << len << " : ";
        for (std::size_t i=0;i<len;++i) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
        std::cout << std::dec << std::endl;

        auto resp = vsomeip::runtime::get()->create_response(rq);
        auto out  = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> v(data, data + len);
        out->set_data(v);
        resp->set_payload(out);
        app_->send(resp);
    }

    void publish_event() {
        static uint32_t cnt = 0;
        std::array<uint8_t, 4> bytes{
            static_cast<uint8_t>((cnt >> 24) & 0xFF),
            static_cast<uint8_t>((cnt >> 16) & 0xFF),
            static_cast<uint8_t>((cnt >>  8) & 0xFF),
            static_cast<uint8_t>((cnt      ) & 0xFF)
        };
        cnt++;

        auto payload = vsomeip::runtime::get()->create_payload();
        // set_data expects vector or pointer+length
        std::vector<vsomeip::byte_t> v(bytes.begin(), bytes.end());
        payload->set_data(v);

        // notify via application API
        app_->notify(kService, kInstance, kEventId, payload, true);

        std::cout << "[SERVER] event published cnt=" << (cnt-1) << std::endl;
    }

    std::shared_ptr<vsomeip::application> app_;
    std::thread event_thread_;
    std::atomic<bool> offered_;
    std::atomic<bool> running_;
};

int main() {
    Server s;
    if (!s.init()) return 1;
    s.start();
    s.stop();
    return 0;
}