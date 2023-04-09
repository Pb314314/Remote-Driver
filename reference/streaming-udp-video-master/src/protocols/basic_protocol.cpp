#include "protocols/basic_protocol.h"

#include <vector>
#include <iostream>
namespace udp_streaming_video {

std::vector<unsigned char> BasicProtocolData::PackageData() const {
  return video_frame_.GetJPEG();
}

void BasicProtocolData::UnpackData(
    const std::vector<unsigned char>& raw_bytes) {

  video_frame_ = VideoFrame(raw_bytes);

}
void BasicProtocolData::hconcat_ProtocolData(BasicProtocolData Protocal_data){
  cv::Mat temp;
  cv::hconcat(this->video_frame_.Frame_image_info(),Protocal_data.GetImage().Frame_image_info(),temp);
  this->video_frame_.change_frame(temp);
}
void BasicProtocolData::vconcat_ProtocolData(BasicProtocolData Protocal_data){
  cv::Mat temp;
  cv::vconcat(this->video_frame_.Frame_image_info(),Protocal_data.GetImage().Frame_image_info(),temp);
  this->video_frame_.change_frame(temp);
}
}  // namespace udp_streaming_video
