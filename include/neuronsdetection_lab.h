#ifndef NEURONSDETECTION_LAB_H
#define NEURONSDETECTION_LAB_H

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

struct EmptySliderMove
{
    EmptySliderMove():min(-1),max(256){}
    int min;
    int max;
};

class neuronsDetection_lab
{
private:
    cv::Mat image;
    std::vector<cv::Mat> planes;

    cv::Mat L_range;
    cv::Mat a_range;
    cv::Mat b_range;

    cv::Mat lab_and;
    cv::Mat calc1;
    cv::Mat calc2;

    short L_min;
    short a_min;
    short b_min;

    short L_max;
    short a_max;
    short b_max;

    unsigned int L[256], a[256], b[256];

    unsigned short int delite_val;
    unsigned short int erode_val;

    bool should_calc_resultingImg;
    bool should_calc_histogramms;
    void calc_histogramms();
public:
    void set_L_min(short new_L_min);
    void set_a_min(short new_a_min);
    void set_b_min(short new_b_min);

    void set_L_max(short new_L_max);
    void set_a_max(short new_a_max);
    void set_b_max(short new_b_max);

    void setAllParametrs(
        short new_L_min,
        short new_L_max,
        short new_a_min,
        short new_a_max,
        short new_b_min,
        short new_b_max);

    std::vector<short> getParamArr();
    EmptySliderMove get_L_empty_move();
    EmptySliderMove get_a_empty_move();

    cv::Mat getImage();
    cv::Mat getGrayImage();

    void erode(unsigned short int dimension);
    void delite(unsigned short int dimension);

    std::vector<double> get_pct_haracteristic();

    neuronsDetection_lab(cv::Mat image);
    cv::Mat getResultingBinImg();
    std::vector<unsigned int> getHistogramms();
    ~neuronsDetection_lab();
};

#endif // NEURONSDETECTION_LAB_H
