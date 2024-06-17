#pragma once
#include <cstddef>
#include <utility>

namespace monitor {
  int sample_cpu_usage(std::pair<size_t, size_t> &s);
  int sample_memory_usage(std::pair<size_t, size_t> &s);
  float get_cpu_usage(size_t us);
  float get_memory_usage(size_t us);
}
