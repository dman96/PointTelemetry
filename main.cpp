#include "point_manager.hpp"

#include <bits/getopt_ext.h>
#include <getopt.h>

#include <iostream>

int main(int argc, char **argv) {

  int option;
  int option_index{0};

  unsigned int number_points;
  unsigned int telemetry_port;
  unsigned int server_port;

  static struct option long_options[] = {
      {"number_points", required_argument, nullptr, 'n'},
      {"telemetry_port", required_argument, nullptr, 't'},
      {"server_port", required_argument, nullptr, 'p'},
      {nullptr, 0, nullptr, 0} // Required for the end of the options array
  };

  while ((option = getopt_long_only(argc, argv, "", long_options,
                                    &option_index)) != -1) {
    switch (option) {
    case 'p':
      server_port = std::stoi(optarg);
      break;
    case 'n':
      number_points = std::stoi(optarg);
      break;
    case 't':
      telemetry_port = std::stoi(optarg);
      break;
    default:
      std::cout << "Unknown option" << std::endl;
      return 1;
    }
  }

  PointManager manager(number_points, telemetry_port, server_port);

  // TODO: start publishing points telemetry_port
  // TODO: start server
}
