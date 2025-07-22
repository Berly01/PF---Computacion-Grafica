 #include "HandGestureDetector.hpp"


 int HandGestureDetector::detectGesture(const cv::Mat& frame) {
    
    cv::Mat hsv, mask;

    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower(0, 0, 0);
    cv::Scalar upper(20, 100, 100);
    cv::inRange(hsv, lower, upper, mask);
    //cv::imshow("a",mask);

    cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
 
    if (contours.empty()) return 0; 
    size_t maxContourIdx = 0;
    double maxArea = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            maxContourIdx = i;
        }
    }
    auto& contour = contours[maxContourIdx];
    std::vector<int> hull;
    cv::convexHull(contour, hull, false, false);
    std::vector<cv::Vec4i> defects;
    if (hull.size() > 3) {
        cv::convexityDefects(contour, hull, defects);
    }

    int fingerCount = 0;
    for (size_t i = 0; i < defects.size(); ++i) {
        float depth = defects[i][3] / 256.0f;
        if (depth > 10) fingerCount++;
    }

    if (fingerCount == 0) {
        return 1; 
    } else if (fingerCount >= 4) {
        return 2;
    } else if (fingerCount >= 1 && fingerCount <= 3) {
        return 3; 
    }
    return 0;
 }