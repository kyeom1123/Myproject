#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring> 

class speed_publisher {
public:
    speed_publisher() {
        app_ = vsomeip::runtime::get()->create_application("Service");
        app_->init();

        app_->register_state_handler([this](vsomeip::state_type_e state) {
            if (state == vsomeip::state_type_e::ST_REGISTERED) {
                app_->offer_service(SERVICE_ID, INSTANCE_ID);
                app_->offer_event(
                    SERVICE_ID,
                    INSTANCE_ID,
                    EVENT_ID,
                    { EVENTGROUP_ID },
                    vsomeip::event_type_e::ET_FIELD
                );
                std::cout << "[Service] Offered event: vehicle speed" << std::endl;
            }
        });
    }

    void start() {
        app_->start();
    }

    void publish_loop() {
        while (true) {
            float speed = get_next_speed();
            std::vector<vsomeip::byte_t> data(sizeof(float));
            std::memcpy(data.data(), &speed, sizeof(float));

            auto payload = vsomeip::runtime::get()->create_payload();
            payload->set_data(data);

            app_->notify(SERVICE_ID, INSTANCE_ID, EVENT_ID, payload);
            std::cout << "[Service] Published speed = " << speed << " km/h" << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

private:
    float get_next_speed() {
        static float speed = 0.0f;
        speed += 7.5f;
        if (speed > 150.0f) speed = 0.0f;
        return speed;
    }

    std::shared_ptr<vsomeip::application> app_;
    const vsomeip::service_t SERVICE_ID = 0x1234;
    const vsomeip::instance_t INSTANCE_ID = 0x5678;
    const vsomeip::eventgroup_t EVENTGROUP_ID = 0x01;
    const vsomeip::event_t EVENT_ID = 0x4444;
};

int main() {
    speed_publisher pub;
    std::thread loop_thread([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // app_->start() 이후 실행
        pub.publish_loop();
    });

    pub.start();
    loop_thread.join();
    return 0;
}
