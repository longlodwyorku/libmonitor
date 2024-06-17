#include <iostream>
#include "../include/monitor.hpp"

int main () {
  std::cout << "CPU Usage: " << monitor::get_cpu_usage(100000) << std::endl;
  std::cout << "Memory Usage: " << monitor::get_memory_usage() << std::endl;
  return 0;
}
