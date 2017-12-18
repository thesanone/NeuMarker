#ifndef NEURONSMARKER_H
#define NEURONSMARKER_H

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

struct OneNeuron
{
    cv::Rect rect;
    int point_counter;
    cv::Point center;
};

class NeuronsMarker
{
    cv::Mat bin_image;
    std::vector<OneNeuron> neuronsList;
    std::vector<cv::Rect> rectList;
    void calculateNeuList();
    void calculateRectList();
    void calculateCharacteristics();
    unsigned short minArea, minArea_2;
    bool should_calc_neuList,
         should_calc_characteristics,
         should_calc_rectList;

    cv::Point center_mass_1;
    cv::Point center_mass_0;
    int rad_1, rad_0;
    float areaSeparator;

public:
    NeuronsMarker(cv::Mat bin_img);
    void set_min_areas(unsigned short minArea_1, unsigned short minArea_2);
    void change_bin_img(cv::Mat bin_img);
    std::vector<OneNeuron> getNeuronsList();
    std::vector<cv::Rect> getRectsList();
    cv::Mat getMarkedImg(cv::Mat img, bool should_mark_center);
    std::vector<int> get_characteristics();
    ~NeuronsMarker();
    unsigned short getMinArea_1() const;
    unsigned short getMinArea_2() const;
    float getAreaSeparator() const;
    void setAreaSeparator(float value);
};

#endif // NEURONSMARKER_H
