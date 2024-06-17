#include "../include/monitor.hpp"
#include <cstdint>
#include <cstdio>
#include <unistd.h>

class fp_closer {
public:
  fp_closer(FILE* file) : file(file) {}
  ~fp_closer() {
    fclose(file);
  }
private:
  FILE* file;
};

float monitor::get_cpu_usage(size_t us) {
  std::pair<size_t, size_t> s1, s2;
  if (sample_cpu_usage(s1) != 0) {
    return -1;
  }
  usleep(us);
  if (sample_cpu_usage(s2) != 0) {
    return -1;
  }
  uint64_t total = s2.second - s1.second;
  if (total == 0) {
    return 0;
  }
  return ((double)(s2.first - s1.first)) / total;
}

float monitor::get_memory_usage(size_t us) {
  std::pair<size_t, size_t> s1, s2;
  if (sample_memory_usage(s1) != 0) {
    return -1;
  }
  usleep(us);
  if (sample_memory_usage(s2) != 0) {
    return -1;
  }
  uint64_t total = s2.second - s1.second;
  if (total == 0) {
    return 0;
  }
  return ((double)(s2.first - s1.first)) / total;
}

int monitor::sample_cpu_usage(std::pair<uint64_t, uint64_t> &s) {
  FILE* file = fopen("/proc/stat", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  uint64_t user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;
  if (10 != fscanf(file, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice)) {
    return -2;
  }
  size_t total = user + nice + system + idle + iowait + irq + softirq + steal;
  s = std::make_pair(total - idle, total);
  return 0;
}

int monitor::sample_memory_usage(std::pair<uint64_t, uint64_t> &s) {
  FILE* file = fopen("/proc/meminfo", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  uint64_t total = 0, free = 0, available = 0;
  if(3 != fscanf(file, "MemTotal: %lu kB\nMemFree: %lu kB\nMemAvailable: %lu kB", &total, &free, &available)) {
    return -2;
  }
  uint64_t used = total - available;
  s = std::make_pair(used, total);
  return 0;
}
