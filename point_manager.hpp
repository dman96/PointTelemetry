#pragma once

#include "point.hpp"
#include "packet.h"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <mutex>

class PointManager {
public:
  explicit PointManager(unsigned int n, int telemetry_port, int server_port)
      : telemetry_port_(telemetry_port), server_port_(server_port) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    point_vector_.reserve(n);
    for (auto i = 0; i < n; ++i) {
      point_vector_.emplace_back(std::rand() % 101 - 50,
                                 std::rand() % 101 - 50);
    }
  }

  PointManager() = delete;

  void publishTelemetry(int serverSocket, sockaddr_in &clientAddr, socklen_t clientAdderLen, char *buffer);

  void startServer();

  void handleRequest(int serverSocket, sockaddr_in &clientAddr, socklen_t clientAddrLen, char *buffer, int bufferSize);

  packet::cmd_packet deserializePacket(char *buffer, int bufferSize);
  char *serializeResponsePacket(packet::rsp_packet &packet);
  void setTargetPosition(int index, int x, int y);
  void serverRequestThread(int serverSocket);


private:
  std::vector<Point> point_vector_;
  int telemetry_port_;
  int server_port_;
  std::chrono::milliseconds delay_; //Thread frequency
  std::vector<std::thread> thread_pool;
  std::mutex lock_;
  std::vector<int> subscriberThreadIndicies_;
  
};
