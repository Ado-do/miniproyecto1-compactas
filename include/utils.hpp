#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::vector<uint8_t> read_file_to_vector(const std::string &filepath);
std::vector<uint8_t> get_bwt(const std::vector<uint8_t> &text);
std::vector<std::vector<uint8_t>> generate_patterns(const std::vector<uint8_t> &text, size_t m, size_t cnt);
