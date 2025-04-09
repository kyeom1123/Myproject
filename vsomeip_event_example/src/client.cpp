#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <cstring>

class speed_subscriber {
public:
    speed_subscriber() {
        app_ = vsomeip::runtime::get()->create_application("Client");
        app_->init();

        app_->register_state_handler([this](vsomeip::state_type_e state) {
            if (state == vsomeip::state_type_e::ST_REGISTERED) {
                app_->request_service(SERVICE_ID, INSTANCE_ID);
                app_->subscribe(SERVICE_ID, INSTANCE_ID, EVENTGROUP_ID);
                std::cout << "[Client] Subscribed to vehicle speed event group" << std::endl;
            }
        });

        app_->register_message_handler(SERVICE_ID, INSTANCE_ID, EVENT_ID,
            std::bind(&speed_subscriber::on_event, this, std::placeholders::_1));
    }

    void start() {
        app_->start();
    }

private:
    void on_event(const std::shared_ptr<vsomeip::message> &msg) {
        auto payload = msg->get_payload();
        const vsomeip::byte_t* data = payload->get_data();
        auto size = payload->get_length();

        if (size == sizeof(float)) {
            float speed;
            std::memcpy(&speed, data, sizeof(float));
            std::cout << "[Client] Received speed = " << speed << " km/h" << std::endl;
        }
    }

    std::shared_ptr<vsomeip::application> app_;
    const vsomeip::service_t SERVICE_ID = 0x1234;
    const vsomeip::instance_t INSTANCE_ID = 0x5678;
    const vsomeip::eventgroup_t EVENTGROUP_ID = 0x01;
    const vsomeip::event_t EVENT_ID = 0x4444;
};

int main() {
    speed_subscriber client;
    client.start();
    return 0;
}
