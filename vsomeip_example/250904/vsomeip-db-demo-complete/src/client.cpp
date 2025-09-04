// client.cpp
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

class Client {
public:
    explicit Client(const ethdb::Database& db)
    : db_(db),
      app_(vsomeip::runtime::get()->create_application("demo_client")),
      available_(false), running_(true) {}

    bool init() {
        if (!app_->init()) {
            std::cerr << "[CLIENT] app init failed" << std::endl;
            return false;
        }

        app_->register_state_handler([this](vsomeip::state_type_e s){
            if (s == vsomeip::state_type_e::ST_REGISTERED) {
                std::cout << "[CLIENT] REGISTERED" << std::endl;
                app_->request_service(db_.svc.service, db_.svc.instance);
                vsomeip::reliability_type_e rel = (db_.svc.unreliable == "TCP")
                    ? vsomeip::reliability_type_e::RT_RELIABLE
                    : vsomeip::reliability_type_e::RT_UNRELIABLE;
                for (const auto& e : db_.svc.events) {
                    app_->request_event(db_.svc.service, db_.svc.instance, e.id, {e.eventgroup},
                                        vsomeip::event_type_e::ET_EVENT, rel);
                    app_->subscribe(db_.svc.service, db_.svc.instance, e.eventgroup);
                }
            }
        });

        app_->register_availability_handler(db_.svc.service, db_.svc.instance,
            [this](vsomeip::service_t, vsomeip::instance_t, bool av){
                available_ = av;
                std::cout << "[CLIENT] availability: " << (av ? "UP":"DOWN") << std::endl;
            });

        for (const auto& m : db_.svc.methods) {
            app_->register_message_handler(db_.svc.service, db_.svc.instance, m.id,
                [this](const std::shared_ptr<vsomeip::message>& rsp){
                    if (rsp->get_message_type() == vsomeip::message_type_e::MT_RESPONSE) {
                        auto pl = rsp->get_payload();
                        const auto *d = pl->get_data();
                        const auto len = pl->get_length();
                        std::cout << "[CLIENT] resp method 0x" << std::hex << rsp->get_method()
                                  << " len=" << std::dec << len << " : ";
                        for (std::size_t i=0;i<len;++i)
                            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)d[i] << " ";
                        std::cout << std::dec << std::endl;
                    }
                });
        }

        if (!db_.svc.methods.empty()) {
            worker_ = std::thread([this](){
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2s);
                std::size_t seq=0;
                while (running_) {
                    if (available_) send_first_method(seq++);
                    std::this_thread::sleep_for(3s);
                }
            });
        }

        return true;
    }

    void start(){ app_->start(); }
    void stop(){
        running_ = false;
        try { app_->stop(); } catch(...) {}
        if (worker_.joinable()) worker_.join();
    }

private:
    void send_first_method(std::size_t seq){
        const auto& m = db_.svc.methods.front();
        auto rq = vsomeip::runtime::get()->create_request();
        rq->set_service(db_.svc.service);
        rq->set_instance(db_.svc.instance);
        rq->set_method(m.id);

        codec::bytes payload;
        for (std::size_t i=0;i<m.request_fields.size(); ++i){
            const auto& f = m.request_fields[i];
            if (f.type=="u8")
                codec::put_u8(payload, (i+1< m.request_fields.size()) ? uint8_t(0x10+i) : uint8_t(seq & 0xFF));
            else if (f.type=="u16")
                codec::put_u16(payload, 0x1122);
            else if (f.type=="u32")
                codec::put_u32(payload, 0x11223344);
            else if (f.type=="u64")
                codec::put_u64(payload, 0x1122334455667788ULL);
            else if (f.type=="string")
                codec::put_string(payload, "hello");
            else
                codec::put_u8(payload, 0x00);
        }

        auto pl = vsomeip::runtime::get()->create_payload();
        pl->set_data(payload);
        rq->set_payload(pl);

        std::cout << "[CLIENT] send method 0x" << std::hex << m.id << std::dec << std::endl;
        app_->send(rq);
    }

    const ethdb::Database& db_;
    std::shared_ptr<vsomeip::application> app_;
    std::atomic<bool> available_{false};
    std::atomic<bool> running_{true};
    std::thread worker_;
};

int main(){
    const char* p = std::getenv("ETHERNET_DB");
    std::string db_path = p ? p : "db/ethernet_db.json";
    auto db = ethdb::load(db_path);
    Client c(db);
    if (!c.init()) return 1;
    c.start();
    c.stop();
    return 0;
}
