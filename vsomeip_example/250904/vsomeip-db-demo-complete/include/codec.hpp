#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace codec {
using bytes = std::vector<uint8_t>;

inline void put_u8(bytes& out, uint8_t v){ out.push_back(v); }
inline void put_u16(bytes& out, uint16_t v){
    out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>(v & 0xFF));
}
inline void put_u32(bytes& out, uint32_t v){
    out.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>(v & 0xFF));
}
inline void put_u64(bytes& out, uint64_t v){
    for(int i=7;i>=0;--i) out.push_back(static_cast<uint8_t>((v >> (8*i)) & 0xFF));
}
inline void put_bytes(bytes& out, const uint8_t* p, std::size_t n){
    out.insert(out.end(), p, p+n);
}
inline void put_string(bytes& out, const std::string& s){
    put_u32(out, static_cast<uint32_t>(s.size()));
    out.insert(out.end(), s.begin(), s.end());
}
}
