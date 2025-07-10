#include "PatternDetector.hpp"

PatternDetector::PatternDetector(int rows, int cols)
    : patternSize(cols, rows) {
}

bool PatternDetector::detect(const cv::Mat& frame) {
    patternFound = false;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Detectar el patrón de ajedrez
    patternFound = cv::findChessboardCorners(gray, patternSize, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

    if (patternFound) {
        // Refinar las esquinas
        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
    }

    return patternFound;
}

bool PatternDetector::is_pattern_found() const {
    return patternFound;
}

const std::vector<cv::Point2f>& PatternDetector::get_corners() const {
    return corners;
}
