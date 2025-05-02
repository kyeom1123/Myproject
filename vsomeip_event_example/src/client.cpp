#include "speed_subscriber.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <regex>

// 간단한 JSON 파싱 함수
bool parse_hex_value(const std::string& str, uint16_t& result) {
    try {
        // "0x" 또는 "0X" 접두사가 있는 16진수 문자열을 파싱
        std::regex hex_pattern("^0[xX]([0-9a-fA-F]+)$");
        std::smatch matches;
        
        if (std::regex_match(str, matches, hex_pattern)) {
            std::stringstream ss;
            ss << std::hex << matches[1].str();
            uint32_t temp;
            ss >> temp;
            result = static_cast<uint16_t>(temp);
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[Client] JSON 파싱 오류: " << e.what() << std::endl;
        return false;
    }
}

speed_subscriber::speed_subscriber() {
    if (!load_configuration()) {
        throw std::runtime_error("Failed to load configuration");
    }

    app_ = vsomeip::runtime::get()->create_application(APP_NAME);
    if (!app_) {
        throw std::runtime_error("Failed to create vsomeip application");
    }

    if (!app_->init()) {
        throw std::runtime_error("Failed to initialize vsomeip application");
    }

    app_->register_state_handler([this](vsomeip::state_type_e state) {
        if (state == vsomeip::state_type_e::ST_REGISTERED) {
            // request_service와 subscribe는 void를 반환하므로 반환값 체크를 제거
            app_->request_service(SERVICE_ID, INSTANCE_ID);
            app_->subscribe(SERVICE_ID, INSTANCE_ID, EVENTGROUP_ID);
            std::cout << "[Client] Successfully subscribed to vehicle speed event group" << std::endl;
        }
    });

    app_->register_message_handler(SERVICE_ID, INSTANCE_ID, EVENT_ID,
        std::bind(&speed_subscriber::on_event, this, std::placeholders::_1));
}

speed_subscriber::~speed_subscriber() {
    if (app_) {
        app_->stop();
        app_->unsubscribe(SERVICE_ID, INSTANCE_ID, EVENTGROUP_ID);
        app_->release_service(SERVICE_ID, INSTANCE_ID);
    }
}

void speed_subscriber::start() {
    // start는 void를 반환하므로 반환값 체크를 제거
    app_->start();
}

void speed_subscriber::on_event(const std::shared_ptr<vsomeip::message> &msg) {
    if (!msg) {
        std::cerr << "[Client] Received null message" << std::endl;
        return;
    }

    auto payload = msg->get_payload();
    if (!payload) {
        std::cerr << "[Client] Message has no payload" << std::endl;
        return;
    }

    const vsomeip::byte_t* data = payload->get_data();
    auto size = payload->get_length();

    if (size != sizeof(float)) {
        std::cerr << "[Client] Invalid payload size: " << size << " bytes" << std::endl;
        return;
    }

    float speed;
    std::memcpy(&speed, data, sizeof(float));
    std::cout << "[Client] Received speed = " << speed << " km/h" << std::endl;
}

bool speed_subscriber::load_configuration() {
    try {
        std::ifstream config_file("config/client_config.json");
        if (!config_file.is_open()) {
            std::cerr << "[Client] Failed to open configuration file" << std::endl;
            return false;
        }

        std::string line;
        std::map<std::string, std::string> config;
        
        // 간단한 JSON 파싱
        while (std::getline(config_file, line)) {
            // 주석 제거
            size_t comment_pos = line.find("//");
            if (comment_pos != std::string::npos) {
                line = line.substr(0, comment_pos);
            }
            
            // 키-값 쌍 추출
            std::regex key_value_pattern("\"([^\"]+)\"\\s*:\\s*\"([^\"]+)\"");
            std::smatch matches;
            
            if (std::regex_search(line, matches, key_value_pattern)) {
                std::string key = matches[1].str();
                std::string value = matches[2].str();
                config[key] = value;
            }
        }
        
        // 설정값 적용
        if (config.find("service_id") != config.end() && 
            config.find("instance_id") != config.end() && 
            config.find("eventgroup_id") != config.end() && 
            config.find("event_id") != config.end()) {
            
            uint16_t service_id, instance_id, eventgroup_id, event_id;
            
            if (!parse_hex_value(config["service_id"], service_id) ||
                !parse_hex_value(config["instance_id"], instance_id) ||
                !parse_hex_value(config["eventgroup_id"], eventgroup_id) ||
                !parse_hex_value(config["event_id"], event_id)) {
                std::cerr << "[Client] Invalid hex values in configuration" << std::endl;
                return false;
            }
            
            SERVICE_ID = service_id;
            INSTANCE_ID = instance_id;
            EVENTGROUP_ID = eventgroup_id;
            EVENT_ID = event_id;
            
            return true;
        } else {
            std::cerr << "[Client] Missing required configuration values" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "[Client] Configuration error: " << e.what() << std::endl;
        return false;
    }
}

int main() {
    try {
        speed_subscriber client;
        client.start();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "[Client] Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
