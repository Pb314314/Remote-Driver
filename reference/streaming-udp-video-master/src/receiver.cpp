// This program opens a socket that listens to incoming UDP packets. When a
// video frame packet is received, it will be decoded and displayed in a GUI
// window.

#include <iostream>
#include <vector>
#include <thread>

#include "protocols/basic_protocol.h"
#include "receiver/receiver_socket.h"
#include "util/util.h"

using udp_streaming_video::BasicProtocolData;
using udp_streaming_video::ReceiverSocket;

int main(int argc, char** argv) {
  const int port = udp_streaming_video::util::ProcessPortParam(argc, argv);
  if (port < 0) {
    return -1;
  }
  const ReceiverSocket socket_0(port);
  const ReceiverSocket socket_1(8081);//
  const ReceiverSocket socket_2(8082);//
  const ReceiverSocket socket_3(8083);//
  if (!socket_0.BindSocketToListen()) {
    std::cerr << "Could not bind socket." << std::endl;
    return -1;
  }
  if (!socket_1.BindSocketToListen()) {
    std::cerr << "Could not bind socket." << std::endl;
    return -1;
  }
  if (!socket_2.BindSocketToListen()) {
    std::cerr << "Could not bind socket." << std::endl;
    return -1;
  }
  if (!socket_3.BindSocketToListen()) {
    std::cerr << "Could not bind socket." << std::endl;
    return -1;
  }
  std::cout << "Listening on port " << port << "." << std::endl;
  BasicProtocolData protocol_data_0;
  BasicProtocolData protocol_data_1, protocol_data_2, protocol_data_3;
  
  while (true) {  // TODO: break out cleanly when done.
    /* std::vector<unsigned char> big_image_data;
    std::vector<unsigned char> vec1 = socket.GetPacket();
    std::vector<unsigned char> vec2 = socket_1.GetPacket(); */
  
    /* big_image_data.insert(big_image_data.end(),vec1.begin(),vec1.end());
    
		big_image_data.insert(big_image_data.end(),vec2.begin(),vec2.end()); */
    
    //std::cout << "Sizeof vec1: " << sizeof(vec1) << " Sizeof vec2: " << sizeof(vec2)<<"Sizeof vec3: "<<sizeof(big_image_data)<<std::endl;
    //big_image_data.insert(big_image_data.end(),vec2.begin(),vec2.end());
    std::thread t0(&BasicProtocolData::UnpackData, &protocol_data_0,socket_0.GetPacket());
    std::thread t1(&BasicProtocolData::UnpackData, &protocol_data_1,socket_1.GetPacket());
    std::thread t2(&BasicProtocolData::UnpackData, &protocol_data_2,socket_2.GetPacket());
    std::thread t3(&BasicProtocolData::UnpackData, &protocol_data_3,socket_3.GetPacket());
    t0.join();
    t1.join();
    std::thread t4(&BasicProtocolData::hconcat_ProtocolData, &protocol_data_0,protocol_data_1);
    t2.join();
    t3.join();
    std::thread t5(&BasicProtocolData::hconcat_ProtocolData, &protocol_data_2,protocol_data_3);
    t4.join();
    t5.join();
    //protocol_data_2.hconcat_ProtocolData(protocol_data_3);
    protocol_data_0.vconcat_ProtocolData(protocol_data_2);
    //t2.join();
    //t3.join();
    //protocol_data.UnpackData(socket.GetPacket());
    //protocol_data_1.UnpackData(socket_1.GetPacket());
    //protocol_data_2.UnpackData(socket.GetPacket());
    //protocol_data_3.UnpackData(socket_1.GetPacket());
    
    
    //protocol_data.UnpackData(big_image_data);//将char在video_frame中转化为Mat
    //图片需要的就是Mat的数据显示，将Mat显示出来就成了视频。我将多个Mat合起来成一张图片
    
    //protocol_data_2.hconcat_ProtocolData(protocol_data_3);
    //protocol_data.vconcat_ProtocolData(protocol_data_2);
    //需要将多个protocol_data合成一个大的protocol_data，或者将多个frame合成一个大的frame
    protocol_data_0.GetImage().Display();
  }
  return 0;
}
