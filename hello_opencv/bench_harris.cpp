#include <opencv2/opencv.hpp>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct BenchResult {
    std::string name;
    double total_ms;
    double avg_ms;
    double min_ms;
    double max_ms;
    int num_features;
};

static BenchResult RunBenchmark(const cv::Mat &gray,
                                int blockSize,
                                int ksize,
                                double k,
                                double threshold,
                                const std::string &label,
                                int warmup,
                                int iterations) {
    cv::Mat dst, dst_norm;

    for (int i = 0; i < warmup; ++i) {
        cv::cornerHarris(gray, dst, blockSize, ksize, k);
        cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX);
    }

    double min_ms = 1e9, max_ms = 0, total_ms = 0;
    int last_feature_count = 0;

    for (int i = 0; i < iterations; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        cv::cornerHarris(gray, dst, blockSize, ksize, k);
        cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX);
        auto t1 = std::chrono::high_resolution_clock::now();

        int count = 0;
        for (int r = 0; r < dst_norm.rows; ++r) {
            const float *row = dst_norm.ptr<float>(r);
            for (int c = 0; c < dst_norm.cols; ++c) {
                if (row[c] > threshold) {
                    ++count;
                }
            }
        }

        double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        total_ms += elapsed_ms;
        if (elapsed_ms < min_ms) min_ms = elapsed_ms;
        if (elapsed_ms > max_ms) max_ms = elapsed_ms;
        last_feature_count = count;
    }

    return {label, total_ms, total_ms / iterations, min_ms, max_ms, last_feature_count};
}

static void PrintBenchHeader() {
    std::cout << std::left << std::setw(42) << "Benchmark"
              << std::right << std::setw(10) << "Total(ms)"
              << std::setw(10) << "Avg(ms)"
              << std::setw(10) << "Min(ms)"
              << std::setw(10) << "Max(ms)"
              << std::setw(10) << "Features"
              << "\n";
    std::cout << std::string(92, '-') << "\n";
}

static void PrintBenchResult(const BenchResult &r) {
    std::cout << std::left << std::setw(42) << r.name
              << std::right << std::fixed << std::setprecision(2)
              << std::setw(10) << r.total_ms
              << std::setw(10) << r.avg_ms
              << std::setw(10) << r.min_ms
              << std::setw(10) << r.max_ms
              << std::setw(10) << r.num_features
              << "\n";
}

int main(int argc, char **argv) {
    const char *image_path = (argc > 1) ? argv[1] : "chess_board.jpg";
    int warmup = (argc > 2) ? std::atoi(argv[2]) : 5;
    int iterations = (argc > 3) ? std::atoi(argv[3]) : 50;

    cv::Mat src = cv::imread(image_path, cv::IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: cannot load " << image_path << std::endl;
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    std::cout << "=== Harris Corner Detection Benchmark ===\n";
    std::cout << "Image: " << image_path
              << " (" << gray.cols << "x" << gray.rows << ")\n";
    std::cout << "Warmup: " << warmup << "  Iterations: " << iterations << "\n\n";

    PrintBenchHeader();

    // Vary blockSize
    for (int bs : {2, 3, 5, 7}) {
        auto label = "blockSize=" + std::to_string(bs) + " ksize=3 k=0.06";
        auto r = RunBenchmark(gray, bs, 3, 0.06, 150, label, warmup, iterations);
        PrintBenchResult(r);
    }

    std::cout << "\n";
    PrintBenchHeader();

    // Vary Sobel aperture (ksize)
    for (int ks : {3, 5, 7}) {
        auto label = "blockSize=2 ksize=" + std::to_string(ks) + " k=0.06";
        auto r = RunBenchmark(gray, 2, ks, 0.06, 150, label, warmup, iterations);
        PrintBenchResult(r);
    }

    std::cout << "\n";
    PrintBenchHeader();

    // Vary Harris free parameter k
    for (double k : {0.02, 0.04, 0.06, 0.10, 0.15}) {
        std::ostringstream oss;
        oss << "blockSize=2 ksize=3 k=" << std::fixed << std::setprecision(2) << k;
        auto r = RunBenchmark(gray, 2, 3, k, 150, oss.str(), warmup, iterations);
        PrintBenchResult(r);
    }

    std::cout << "\n";
    PrintBenchHeader();

    // Vary threshold
    for (double th : {50, 100, 150, 200}) {
        auto label = "blockSize=2 ksize=3 k=0.06 th=" + std::to_string(static_cast<int>(th));
        auto r = RunBenchmark(gray, 2, 3, 0.06, th, label, warmup, iterations);
        PrintBenchResult(r);
    }

    std::cout << "\n";
    PrintBenchHeader();

    // Multi-scale: resize image and benchmark
    for (double scale : {0.25, 0.5, 1.0, 2.0}) {
        cv::Mat resized;
        cv::resize(gray, resized, cv::Size(), scale, scale);
        auto label = "scale=" + std::to_string(static_cast<int>(scale * 100)) + "% (" +
                     std::to_string(resized.cols) + "x" + std::to_string(resized.rows) + ")";
        auto r = RunBenchmark(resized, 2, 3, 0.06, 150, label, warmup, iterations);
        PrintBenchResult(r);
    }

    return 0;
}
