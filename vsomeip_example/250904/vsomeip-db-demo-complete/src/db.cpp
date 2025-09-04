#include "db.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using boost::property_tree::ptree;

namespace {
uint16_t parse_hex_u16(const std::string& s){
    unsigned v=0;
    std::stringstream ss;
    ss << std::hex << s;
    ss >> v;
    return static_cast<uint16_t>(v);
}
}

namespace ethdb {

Database load(const std::string& path){
    ptree root;
    read_json(path, root);
    Database db{};

    auto svc = root.get_child("service");
    db.svc.service  = parse_hex_u16(svc.get<std::string>("id"));
    db.svc.instance = parse_hex_u16(svc.get<std::string>("instance"));
    db.svc.major = svc.get<uint8_t>("major");
    db.svc.minor = svc.get<uint8_t>("minor");
    db.svc.reliable   = svc.get<std::string>("reliable");
    db.svc.unreliable = svc.get<std::string>("unreliable");

    for (auto &m : svc.get_child("methods")) {
        Method mm{};
        mm.name = m.second.get<std::string>("name");
        mm.id   = parse_hex_u16(m.second.get<std::string>("id"));
        for (auto &f : m.second.get_child("request")) {
            Field fld{f.second.get<std::string>("name"), f.second.get<std::string>("type")};
            mm.request_fields.push_back(fld);
        }
        for (auto &f : m.second.get_child("response")) {
            Field fld{f.second.get<std::string>("name"), f.second.get<std::string>("type")};
            mm.response_fields.push_back(fld);
        }
        db.svc.methods.push_back(mm);
    }

    for (auto &e : svc.get_child("events")) {
        Event ev{};
        ev.name = e.second.get<std::string>("name");
        ev.id   = parse_hex_u16(e.second.get<std::string>("id"));
        ev.eventgroup = parse_hex_u16(e.second.get<std::string>("eventgroup"));
        for (auto &f : e.second.get_child("payload")) {
            Field fld{f.second.get<std::string>("name"), f.second.get<std::string>("type")};
            ev.payload_fields.push_back(fld);
        }
        db.svc.events.push_back(ev);
    }
    return db;
}

} // namespace ethdb
