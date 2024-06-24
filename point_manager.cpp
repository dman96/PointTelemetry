#include "point_manager.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

void PointManager::publishTelemetry(int socket, sockaddr_in &clientAddr, socklen_t clientAddrLen, char *buffer) {
  
  // Infinite loop to send messages
  while (true) {
    memset(buffer, 0, BUFFER_SIZE);

    for (uint i = 0; i < point_vector_.size(); i++)
    {
      packet::rsp_packet response;
      auto position = point_vector_.at(i);
      position.movePoint();
      response.position._idx = i;
      response.position._x = position.getCurrentPose().first;
      response.position._y = position.getCurrentPose().second;
      char *serialResp = serializeResponsePacket(response);
      memcpy(buffer, serialResp, sizeof(serialResp));
    }
  }

}

void PointManager::startServer() {
  int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (udp_socket == -1) {
    perror("Could not start server\n");
    return;
  }

  // Set up the server address structure
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port_);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  //Client Details
  sockaddr_in client_address;
  socklen_t client_addrlen = sizeof(client_address);

  // Bind the socket to the server address
  if (bind(udp_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Could not bind socket\n");
    return;
  }
  
  std::cout << "Server started\n" << std::endl;
  thread_pool.push_back(std::thread(serverRequestThread, udp_socket));

  close(udp_socket);
}

void PointManager::serverRequestThread(int serverSocket)
{
    lock_.lock(); //Lock request thread
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    while (true) {
      char buffer[BUFFER_SIZE];
      auto bytes = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
      packet::rsp_packet response;

      if (bytes < 0) {
        perror("Error when receiving. \n");
        break;
      }

      if (bytes == 0)
      { 
        perror("Client Disconnected\n");
        break;
      }

      if (bytes > 0)
        handleRequest(serverSocket, clientAddr, clientAddrLen, buffer, bytes);
  }

  lock_.unlock();
}

packet::cmd_packet PointManager::deserializePacket(char *buffer, int bufferSize)
{
  //TODO: ADD Error Handling
  packet::cmd_packet command;
  if (bufferSize < sizeof(uint8_t))
  { 
    perror("Request is too small. A command is missing\n");
    return command; //Return empty packet
  }


  memcpy(&command, buffer, sizeof(command));
  
  //Resolve Byte Ordering
  command.command = htons(command.command);
  command.position._idx = htons(command.position._idx);
  command.position._x = htons(command.position._x);
  command.position._y = htons(command.position._y);

  return command;
}

char *PointManager::serializeResponsePacket(packet::rsp_packet &packet)
{
    std::string message;

    packet.status = 0;
    packet.position._idx = ntohs(packet.position._idx);
    packet.position._x = ntohs(packet.position._x);
    packet.position._y = ntohs(packet.position._y);

}

void PointManager::setTargetPosition(int index, int x, int y)
{
    point_vector_.at(index).setTargetPose(x, y);
}

void PointManager::handleRequest(int serverSocket, sockaddr_in &clientAddr, socklen_t clientAddrLen, char *buffer, int bufferSize)
{
  packet::cmd_packet request = deserializePacket(buffer, bufferSize);

  switch (request.command)
  {
    case SET_POSITION:
      setTargetPosition(request.position._idx, request.position._x, request.position._y);
    break;

    case SUBSCRIBE_START:
      thread_pool.push_back(std::thread(publishTelemetry, serverSocket, clientAddr, clientAddrLen));
    break;

    case SUBSCRIBE_STOP:
      thread_pool.pop_back();
    break;

    default:
      break;
  }

  lock_.unlock(); //Unlock after fulfillimg request
}

