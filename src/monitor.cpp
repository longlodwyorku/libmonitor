#include "../include/monitor.hpp"
#include <cstdio>

class fp_closer {
public:
  fp_closer(FILE* file) : file(file) {}
  ~fp_closer() {
    fclose(file);
  }
private:
  FILE* file;
};

float monitor::get_cpu_usage() {
  FILE* file = fopen("/proc/stat", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  unsigned long long user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;
  if (10 != fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice)) {
    return -2;
  }
  unsigned long long total = user + nice + system + idle + iowait + irq + softirq + steal;
  if (total == 0) {
    return 0.0;
  }

  return ((double)(total - idle)) / total;
}

float monitor::get_memory_usage() {
  FILE* file = fopen("/proc/meminfo", "r");
  if (file == NULL) {
    return -1;
  }
  fp_closer closer(file);

  unsigned long long total = 0, free = 0, available = 0;
  if(3 != fscanf(file, "MemTotal: %llu kB\nMemFree: %llu kB\nMemAvailable: %llu kB", &total, &free, &available)) {
    return -2;
  }
  if (total == 0) {
    return 0.0;
  }
  double used = total - available;

  return used / total;
}
