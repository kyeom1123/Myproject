// server.cpp
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "db.hpp"
#include "codec.hpp"

class Server {
public:
    explicit Server(const ethdb::Database& db)
    : db_(db),
      app_(vsomeip::runtime::get()->create_application("demo_server")),
      offered_(false), running_(true) {}

    bool init() {
        if (!app_->init()) {
            std::cerr << "[SERVER] app init failed" << std::endl;
            return false;
        }

        app_->register_state_handler([this](vsomeip::state_type_e s){
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                std::cout << "[SERVER] REGISTERED" << std::endl;
                app_->offer_service(db_.svc.service, db_.svc.instance);
                for (const auto& m : db_.svc.methods) {
                    app_->register_message_handler(
                        db_.svc.service, db_.svc.instance, m.id,
                        std::bind(&Server::on_method, this, std::placeholders::_1));
                }
                vsomeip::reliability_type_e rel = (db_.svc.unreliable == "TCP")
                    ? vsomeip::reliability_type_e::RT_RELIABLE
                    : vsomeip::reliability_type_e::RT_UNRELIABLE;
                for (const auto& e : db_.svc.events) {
                    app_->offer_event(db_.svc.service, db_.svc.instance, e.id, {e.eventgroup},
                                      vsomeip::event_type_e::ET_EVENT,
                                      std::chrono::milliseconds::zero(),
                                      false, false, {}, rel);
                }
                offered_ = true;
            }
        });
        return true;
    }

    void start() {
        event_thread_ = std::thread([this](){
            while (running_) {
                if (offered_) publish_events();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        });
        app_->start();
    }

    void stop() {
        running_ = false;
        try { app_->stop(); } catch(...) {}
        if (event_thread_.joinable()) event_thread_.join();
    }

private:
    void on_method(const std::shared_ptr<vsomeip::message>& rq) {
        auto pl = rq->get_payload();
        const auto *data = pl->get_data();
        const auto len = pl->get_length();
        std::cout << "[SERVER] Method 0x" << std::hex << rq->get_method()
                  << " len=" << std::dec << len << " : ";
        for (std::size_t i=0;i<len;++i)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
        std::cout << std::dec << std::endl;

        auto resp = vsomeip::runtime::get()->create_response(rq);
        auto out  = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> v(data, data + len);
        out->set_data(v);
        resp->set_payload(out);
        app_->send(resp);
    }

    void publish_events() {
        for (const auto& e : db_.svc.events) {
            codec::bytes payload;
            static uint32_t cnt = 0;
            if (e.payload_fields.size()==1 && e.payload_fields[0].type=="u32") {
                codec::put_u32(payload, cnt++);
            } else {
                codec::put_u32(payload, 0);
            }
            auto vp = vsomeip::runtime::get()->create_payload();
            vp->set_data(payload);
            app_->notify(db_.svc.service, db_.svc.instance, e.id, vp, true);
            std::cout << "[SERVER] event 0x" << std::hex << e.id << " published" << std::dec << std::endl;
        }
    }

    const ethdb::Database& db_;
    std::shared_ptr<vsomeip::application> app_;
    std::thread event_thread_;
    std::atomic<bool> offered_;
    std::atomic<bool> running_;
};

int main(){
    const char* p = std::getenv("ETHERNET_DB");
    std::string db_path = p ? p : "db/ethernet_db.json";
    auto db = ethdb::load(db_path);
    Server s(db);
    if (!s.init()) return 1;
    s.start();
    s.stop();
    return 0;
}
