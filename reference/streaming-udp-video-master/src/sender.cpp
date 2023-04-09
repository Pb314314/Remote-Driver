// This program attempts to gather video frames from a webcam or connected
// camera, encodes them into a compressed format (JPEG), and sends them over UDP
// to a receiver of the specified port number and address. The frames should be
// decodable on the other side and reconstructed into images.

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "protocols/basic_protocol.h"
#include "sender/sender_socket.h"
#include "util/util.h"
#include "video/video_capture.h"
#include "video/video_frame.h"

using udp_streaming_video::BasicProtocolData;
using udp_streaming_video::SenderSocket;
using udp_streaming_video::VideoCapture;
using udp_streaming_video::VideoFrame;
using std::vector;
//a little change

void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data, int index);
void send_package(const SenderSocket socket, VideoCapture video_capture, BasicProtocolData protocol_data, int index){
    
    while(true){
      cv::Mat Origin_Mat = video_capture.GetFrameFromCamera().Frame_image_info();
      int rows = Origin_Mat.rows;
      int cols = Origin_Mat.cols;
      vector<vector<int>> row_vec = {{0,rows/2},{0,rows/2},{rows/2,rows},{rows/2,rows}};
      vector<vector<int>> col_vec = {{0,cols/2},{cols/2,cols},{0,cols/2},{cols/2,cols}};
      cv::Mat Sub_Mat(Origin_Mat,cv::Range(row_vec[index][0],row_vec[index][1]),cv::Range(col_vec[index][0],col_vec[index][1]));
      VideoFrame sub_frame(Sub_Mat);
      protocol_data.SetImage(sub_frame);//给frame中的video_frame中的mat赋值
      socket.SendPacket(protocol_data.PackageData());
      //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}
int main(int argc, char** argv) {
  const int port = udp_streaming_video::util::ProcessPortParam(argc, argv);
  if (port < 0) {
    return -1;   
  }
  std::string ip_address = "127.0.0.1";  // Localhost
  if (argc > 2) {  // First arg is the port number.
    ip_address = std::string(argv[2]);
  }

  const SenderSocket socket_0(ip_address, port);//创建Socket
  const SenderSocket socket_1(ip_address, 8081);//创建Socket
  const SenderSocket socket_2(ip_address, 8082);//创建Socket
  const SenderSocket socket_3(ip_address, 8083);//创建Socket
  std::cout << "Sending to " << ip_address
            << " on port " << port << "." << std::endl;

  VideoCapture video_capture(false, 0.5);//false, 0.25
  BasicProtocolData protocol_data;
  std::thread threads[4];//创建4个线程
  threads[0] = std::thread(send_package, socket_0, video_capture, protocol_data, 0);
  threads[1] = std::thread(send_package, socket_1, video_capture, protocol_data, 1);
  threads[2] = std::thread(send_package, socket_2, video_capture, protocol_data, 2);
  threads[3] = std::thread(send_package, socket_3, video_capture, protocol_data, 3);
  threads[0].join();
  threads[1].join();
  threads[2].join();
  threads[3].join();
  /* while (true) {  // TODO: break out cleanly when done.
    send_package(socket, video_capture, protocol_data);
    protocol_data.SetImage(video_capture.GetFrameFromCamera());//给frame中的video_frame中的mat赋值
    socket.SendPacket(protocol_data.PackageData());
  } */
  return 0;
}
