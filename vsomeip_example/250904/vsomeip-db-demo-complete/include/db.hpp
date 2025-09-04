#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace ethdb {

struct Field {
    std::string name;
    std::string type;
};

struct Method {
    std::string name;
    uint16_t id;
    std::vector<Field> request_fields;
    std::vector<Field> response_fields;
};

struct Event {
    std::string name;
    uint16_t id;
    uint16_t eventgroup;
    std::vector<Field> payload_fields;
};

struct Service {
    uint16_t service;
    uint16_t instance;
    uint8_t major;
    uint8_t minor;
    std::string reliable;
    std::string unreliable;
    std::vector<Method> methods;
    std::vector<Event> events;
};

struct Database { Service svc; };

Database load(const std::string& path);

} // namespace ethdb
