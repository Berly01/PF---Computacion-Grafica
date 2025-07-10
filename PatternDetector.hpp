#pragma once
#include <opencv2/opencv.hpp>

class PatternDetector {
private:
    cv::Size patternSize;
    std::vector<cv::Point2f> corners;
    bool patternFound = false;

public:
    PatternDetector(int rows = 7, int cols = 7);

    bool detect(const cv::Mat& frame);

    bool is_pattern_found() const;

    const std::vector<cv::Point2f>& get_corners() const;
};
