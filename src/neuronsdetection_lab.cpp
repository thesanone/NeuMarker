#include "neuronsdetection_lab.h"
//#include <QTime>
//#include <QDebug>

void neuronsDetection_lab::calc_histogramms()
{
    should_calc_histogramms = false;
    for(int i = 0; i < 256; i++)
    {
        L[i] = 0;
        a[i] = 0;
        b[i] = 0;
    }

    for(int y(lab_and.rows - 1); y >= 0; --y)
    {
        unsigned char *const scanLine_L( planes[0].ptr<unsigned char>(y) );
        unsigned char *const scanLine_a( planes[1].ptr<unsigned char>(y) );
        unsigned char *const scanLine_b( planes[2].ptr<unsigned char>(y) );

        for(int x(lab_and.cols - 1); x >= 0 ; --x)
        {
            L[scanLine_L[x]]++;
            a[scanLine_a[x]]++;
            b[scanLine_b[x]]++;
        }
    }

}

void neuronsDetection_lab::set_L_min(short new_L_min)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    L_min = new_L_min;

    cv::threshold(planes[0], calc1, L_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[0], calc2, L_max, 255, CV_THRESH_BINARY_INV);

    if(L_min < L_max)
        cv::bitwise_and(calc1, calc2, L_range);
    else
        cv::bitwise_or(calc1, calc2, L_range);
}

void neuronsDetection_lab::set_a_min(short new_a_min)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    a_min = new_a_min;
    cv::threshold(planes[1], calc1, a_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[1], calc2, a_max, 255, CV_THRESH_BINARY_INV);
    if(a_min < a_max)
        cv::bitwise_and(calc1, calc2, a_range);
    else
        cv::bitwise_or(calc1, calc2, a_range);
}

void neuronsDetection_lab::set_b_min(short new_b_min)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    b_min = new_b_min;
    cv::threshold(planes[2], calc1, b_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[2], calc2, b_max, 255, CV_THRESH_BINARY_INV);
    if(b_min < b_max)
        cv::bitwise_and(calc1, calc2, b_range);
    else
        cv::bitwise_or(calc1, calc2, b_range);
}

void neuronsDetection_lab::set_b_max(short new_b_max)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    b_max = new_b_max;
    cv::threshold(planes[2], calc1, b_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[2], calc2, b_max, 255, CV_THRESH_BINARY_INV);
    if(b_min < b_max)
        cv::bitwise_and(calc1, calc2, b_range);
    else
        cv::bitwise_or(calc1, calc2, b_range);
}

void neuronsDetection_lab::set_a_max(short new_a_max)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    a_max = new_a_max;
    cv::threshold(planes[1], calc1, a_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[1], calc2, a_max, 255, CV_THRESH_BINARY_INV);
    if(a_min < a_max)
        cv::bitwise_and(calc1, calc2, a_range);
    else
        cv::bitwise_or(calc1, calc2, a_range);
}

void neuronsDetection_lab::set_L_max(short new_L_max)
{
    if(!should_calc_resultingImg)
        should_calc_resultingImg = true;

    L_max = new_L_max;

    cv::threshold(planes[0], calc1, L_min, 255, CV_THRESH_BINARY);
    cv::threshold(planes[0], calc2, L_max, 255, CV_THRESH_BINARY_INV);

    if(L_min < L_max)
        cv::bitwise_and(calc1, calc2, L_range);
    else
        cv::bitwise_or(calc1, calc2, L_range);
}

void neuronsDetection_lab::setAllParametrs(short new_L_min, short new_L_max, short new_a_min, short new_a_max, short new_b_min, short new_b_max)
{
    L_min = new_L_min;
    this->set_L_max(new_L_max);
    a_min = new_a_min;
    this->set_a_max(new_a_max);
    b_min = new_b_min;
    this->set_b_max(new_b_max);
}

std::vector<short> neuronsDetection_lab::getParamArr()
{
    int myshort[] = {L_min, L_max, a_min, a_max, b_min, b_max};
    return std::vector<short> (myshort, myshort + 6 );
}

EmptySliderMove neuronsDetection_lab::get_L_empty_move()
{
    if(should_calc_histogramms) this->calc_histogramms();
    EmptySliderMove result;
    for(int i = 0; i < 256 && L[i] == 0; ++i) result.min++;
    for(int i = 256; i > 0 && L[i] == 0; --i) result.max--;
    return result;
}

EmptySliderMove neuronsDetection_lab::get_a_empty_move()
{
    if(should_calc_histogramms) this->calc_histogramms();
    EmptySliderMove result;
    for(int i = 0; i < 256 && a[i] == 0; ++i) result.min++;
    for(int i = 256; i > 0 && a[i] == 0; --i) result.max--;
    return result;
}

cv::Mat neuronsDetection_lab::getImage()
{
    return image;
}

cv::Mat neuronsDetection_lab::getGrayImage()
{
    return planes[0];
}

void neuronsDetection_lab::erode(unsigned short int dimension)
{
    cv::Mat kernel(cv::Size(dimension, dimension), CV_8UC1);
    kernel.setTo(cv::Scalar(1));
    cv::erode(lab_and, lab_and, kernel);
    should_calc_resultingImg = false;

}

void neuronsDetection_lab::delite(unsigned short int dimension)
{
    cv::Mat kernel(cv::Size(dimension, dimension), CV_8UC1);
    kernel.setTo(cv::Scalar(1));
    cv::dilate(lab_and, lab_and, kernel);
    should_calc_resultingImg = false;
}

std::vector<double> neuronsDetection_lab::get_pct_haracteristic()
{
    unsigned int mass[4];

    mass[0] = 0;
    for(int y(L_range.rows - 1); y >= 0; --y)
    {
        unsigned char *const scanLine( L_range.ptr<unsigned char>(y) );

        for(int x(L_range.cols - 1); x >= 0 ; --x)
            if(scanLine[x] > 0) mass[0]++;
    }

    mass[1] = 0;
    for(int y(a_range.rows - 1); y >= 0; --y)
    {
        unsigned char *const scanLine( a_range.ptr<unsigned char>(y) );

        for(int x(a_range.cols - 1); x >= 0 ; --x)
            if(scanLine[x] > 0) mass[1]++;
    }

    mass[2] = 0;
    for(int y(b_range.rows - 1); y >= 0; --y)
    {
        unsigned char *const scanLine( b_range.ptr<unsigned char>(y) );

        for(int x(b_range.cols - 1); x >= 0 ; --x)
            if(scanLine[x] > 0) mass[2]++;
    }

    mass[3] = 0;
    for(int y(lab_and.rows - 1); y >= 0; --y)
    {
        unsigned char *const scanLine( lab_and.ptr<unsigned char>(y) );

        for(int x(lab_and.cols - 1); x >= 0 ; --x)
            if(scanLine[x] > 0) mass[3]++;
    }
    unsigned int dim = lab_and.rows*lab_and.cols;
    std::vector<double> res;
    for(int i = 0; i < 4; i++)
    {
        res.push_back((double)mass[i]/dim);
        //res.push_back( -(res[i]*log2(res[i]) + (1-res[i])*log2((1-res[i]))));
    }

    return res;
}


neuronsDetection_lab::neuronsDetection_lab(cv::Mat image)
{
    L_min = -1;
    a_min = -1;
    b_min = -1;
    erode_val = 3;
    delite_val = 3;
    should_calc_histogramms = true;

    cv::Mat Lab;

    this->image = image.clone();

//    QTime t;
//    t.start();
    cv::cvtColor(this->image, Lab, CV_BGR2Lab);
//    qDebug("BGR2Lab: %d ms", t.elapsed());
    cv::split(Lab, planes);

    this->set_L_max(256);
    this->set_a_max(256);
    this->set_b_max(256);
}

cv::Mat neuronsDetection_lab::getResultingBinImg()
{
    if(should_calc_resultingImg)
    {
        cv::bitwise_and(L_range, a_range, lab_and);
        cv::bitwise_and(b_range, lab_and, lab_and);
    }
    return lab_and;
}

std::vector<unsigned int> neuronsDetection_lab::getHistogramms()
{
    if(should_calc_histogramms)
        this->calc_histogramms();

    std::vector<unsigned int> res;
    for(short i = 0; i < 256; i++)
        res.push_back(L[i]);
    for(short i = 0; i < 256; i++)
        res.push_back(a[i]);
    for(short i = 0; i < 256; i++)
        res.push_back(b[i]);

    return res;
}

neuronsDetection_lab::~neuronsDetection_lab()
{
    planes.clear();
}



