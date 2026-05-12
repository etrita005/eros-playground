#include <opencv2/opencv.hpp>

int main(int, char **) {
    cv::Mat src = cv::imread("chess_board.jpg", cv::IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: cannot load chess_board.jpg" << std::endl;
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::Mat dst, dst_norm, dst_norm_scaled;
    cv::cornerHarris(gray, dst, 2, 3, 0.06);

    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    for (int i = 0; i < dst_norm.rows; i++) {
        for (int j = 0; j < dst_norm.cols; j++) {
            if (static_cast<int>(dst_norm.at<float>(i, j)) > 150) {
                cv::circle(src, cv::Point(j, i), 5, cv::Scalar(0, 0, 255), 2);
            }
        }
    }

    cv::imwrite("chess_board_harris.jpg", src);
    return 0;
}
