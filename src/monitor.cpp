#include "../include/monitor.hpp"
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
  size_t total = s2.first - s1.first;
  if (total == 0) {
    return 0;
  }
  size_t idle = s2.second - s1.second;
  return 1.0 - (float)idle / total;
}

float monitor::get_memory_usage(size_t us) {
  std::pair<size_t, size_t> s1, s2;
  if (sample_memory_usage(s1) != 0) {
    return -1;
  }
  usleep(ms);
  if (sample_memory_usage(s2) != 0) {
    return -1;
  }
  size_t total = s2.first - s1.first;
  if (total == 0) {
    return 0;
  }
  size_t idle = s2.second - s1.second;
  return 1.0 - (float)idle / total;
}

int monitor::sample_cpu_usage(std::pair<size_t, size_t> &s) {
  FILE* file = fopen("/proc/stat", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  size_t user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;
  if (10 != fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice)) {
    return -2;
  }
  size_t total = user + nice + system + idle + iowait + irq + softirq + steal;
  s = std::make_pair(total - idle, total);
  return 0;
}

int monitor::sample_memory_usage(std::pair<size_t, size_t> &s) {
  FILE* file = fopen("/proc/meminfo", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  unsigned long long total = 0, free = 0, available = 0;
  if(3 != fscanf(file, "MemTotal: %llu kB\nMemFree: %llu kB\nMemAvailable: %llu kB", &total, &free, &available)) {
    return -2;
  }
  double used = total - available;
  s = std::make_pair(used, total);
  return 0;
}
