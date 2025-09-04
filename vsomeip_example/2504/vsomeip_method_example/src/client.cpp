#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <cstring>

class speed_client {
public:
    speed_client() {
        app_ = vsomeip::runtime::get()->create_application("Client");
        app_->init();

        app_->register_state_handler([this](vsomeip::state_type_e state) {
            if (state == vsomeip::state_type_e::ST_REGISTERED) {
                app_->request_service(SERVICE_ID, INSTANCE_ID);
                std::cout << "Client: Requested service." << std::endl;
            }
        });

        app_->register_message_handler(SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&speed_client::on_response, this, std::placeholders::_1));
    }

    void start() {
        app_->start();
    }

    void send_command(char cmd) {
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(SERVICE_ID);
        request->set_instance(INSTANCE_ID);
        request->set_method(METHOD_ID);

        std::vector<vsomeip::byte_t> payload_data = { static_cast<vsomeip::byte_t>(cmd) };
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(payload_data);
        request->set_payload(payload);

        std::cout << "Client: Sending command '" << cmd << "'..." << std::endl;
        app_->send(request);
    }

private:
    void on_response(const std::shared_ptr<vsomeip::message> &response) {
        auto payload = response->get_payload();
        const vsomeip::byte_t* data = payload->get_data();
        auto size = payload->get_length();

        if (size == sizeof(float)) {
            float speed;
            std::memcpy(&speed, data, sizeof(float));
            std::cout << "Client: Current speed = " << speed << " km/h" << std::endl;
        }
    }

    std::shared_ptr<vsomeip::application> app_;
    const vsomeip::service_t SERVICE_ID = 0x1234;
    const vsomeip::instance_t INSTANCE_ID = 0x5678;
    const vsomeip::method_t METHOD_ID = 0x0421;
};

int main() {
    speed_client client;
    std::thread t([&]() { client.start(); });

    std::this_thread::sleep_for(std::chrono::seconds(2)); // 서비스 등록 대기

    while (true) {
        std::cout << "Enter '+' or '-' to adjust speed: ";
        char cmd;
        std::cin >> cmd;
        if (cmd == '+' || cmd == '-') {
            client.send_command(cmd);
        } else {
            std::cout << "Invalid command. Use + or -." << std::endl;
        }
    }

    t.join();
    return 0;
}
