#pragma once
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>

std::vector<unsigned int> parse_vec(const std::string &s);
// std::deque<unsigned int> parse_deque(const std::string &s);
int merge_insert_sort(std::vector<unsigned int> &v);
int merge_insert_sort(std::deque<unsigned int> &v);
ssize_t jacobsthal(unsigned int i);
