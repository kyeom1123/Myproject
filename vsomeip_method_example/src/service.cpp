#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>
#include <cstring>

class speed_service {
public:
    speed_service() : speed_kph_(0.0f) {
        app_ = vsomeip::runtime::get()->create_application("Service");
        app_->init();

        app_->register_state_handler([this](vsomeip::state_type_e state) {
            if (state == vsomeip::state_type_e::ST_REGISTERED) {
                app_->offer_service(SERVICE_ID, INSTANCE_ID);
                std::cout << "Service: Service offered." << std::endl;
            }
        });

        app_->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&speed_service::on_request, this, std::placeholders::_1));
    }

    void start() {
        app_->start();
    }

private:
    void on_request(const std::shared_ptr<vsomeip::message> &request) {
        auto req_payload = request->get_payload();
        auto data_ptr = req_payload->get_data();
        auto size = req_payload->get_length();

        if (size >= 1) {
            char cmd = static_cast<char>(data_ptr[0]);
            if (cmd == '+') speed_kph_ += 5.0f;
            else if (cmd == '-') speed_kph_ -= 5.0f;
            if (speed_kph_ < 0.0f) speed_kph_ = 0.0f;

            std::cout << "Service: command [" << cmd << "], new speed = " << speed_kph_ << " km/h" << std::endl;
        }

        // 응답 생성
        std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(request);
        std::vector<vsomeip::byte_t> resp_vec(sizeof(float));
        std::memcpy(resp_vec.data(), &speed_kph_, sizeof(float));
        auto payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(resp_vec);
        response->set_payload(payload);
        app_->send(response);
    }

    std::shared_ptr<vsomeip::application> app_;
    float speed_kph_;

    const vsomeip::service_t SERVICE_ID = 0x1234;
    const vsomeip::instance_t INSTANCE_ID = 0x5678;
    const vsomeip::method_t METHOD_ID = 0x0421;
};

int main() {
    speed_service svc;
    svc.start();
    return 0;
}
