#include "util/util.h"

#include <iostream>
#include <sstream>
#include <vector>
namespace udp_streaming_video {
namespace util {

int ProcessPortParam(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "need Port: ./Receiver 8080" << std::endl;
    return -1;
  }
  std::istringstream ss(argv[1]);//Port
  int port;
  if (!(ss >> port)) {
    std::cerr << "The port number must be an integer." << std::endl;
    return -1;
  }
  if (port < 0 || port > 65535) {
    std::cerr << "Port number must be between 0 and 65535." << std::endl;
    return -1;
  }

  return port;
}

std::vector<int> Sender_ProcessPortParam(int argc, char** argv) {
  std::vector<int> result;
  if (argc < 4) {
    std::cerr << "Port Ip Resolution." << std::endl;
    return result;
  }
  std::istringstream ss(argv[1]);//Port
  std::istringstream ss2(argv[2]);//Ip
  std::istringstream ss3(argv[3]);//Resolution
  int port,Ip,Resolution;
  if (!(ss >> port)) {
    std::cerr << "The port number must be an integer." << std::endl;
    return result;
  }
  if (port < 0 || port > 65535) {
    std::cerr << "Port number must be between 0 and 65535." << std::endl;
    return result;
  }
  if (!(ss2 >> Ip)) {
    std::cerr << "The Ip must be an integer." << std::endl;
    return result;
  }
  if (!(ss3 >> Resolution)) {
    std::cerr << "The Resolution must be an integer." << std::endl;
    return result;
  }
  result.push_back(port);
  result.push_back(Ip);
  result.push_back(Resolution);
  return result;
}
}  // namespace util
}  // namespace udp_streaming_video
