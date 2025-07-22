 #ifndef HAND_GESTURE_DETECTOR_HPP
 #define HAND_GESTURE_DETECTOR_HPP
 #include <opencv2/opencv.hpp>
 class HandGestureDetector {
 public:

 int detectGesture(const cv::Mat& frame);
 };
 #endif