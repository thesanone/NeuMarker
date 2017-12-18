#include "neuronsmarker.h"

NeuronsMarker::NeuronsMarker(cv::Mat bin_img)
{
    bin_image = bin_img.clone();
    minArea = 60;
    minArea_2 = 120;
    areaSeparator = 0.4f;
    should_calc_neuList = true;
    should_calc_characteristics = true;
    should_calc_rectList = true;
}

void NeuronsMarker::set_min_areas(unsigned short minArea_1, unsigned short minArea_2)
{
    if(minArea_1>0 && minArea_2>0)
    {
        this->minArea = minArea_1;
        this->minArea_2 = minArea_2;
        should_calc_characteristics = true;
        should_calc_neuList = true;
        should_calc_rectList = true;
    }
}

void NeuronsMarker::change_bin_img(cv::Mat bin_img)
{
    bin_image = bin_img.clone();
    should_calc_neuList = true;
    should_calc_characteristics = true;
    should_calc_rectList = true;
}

void NeuronsMarker::calculateNeuList()
{
    should_calc_neuList = false;
    neuronsList.clear();
    cv::Mat copyhsv = bin_image.clone();
    int part =(int) (copyhsv.rows*areaSeparator);
    for (int y = 0; y<copyhsv.rows; y++)
    {
        for (int x = 0; x<copyhsv.cols; x++)
        {
            int value = copyhsv.at<uchar>(y, x);
            if (value == 255)
            {
                cv::Rect rect;
                int count = floodFill(copyhsv, cv::Point(x, y), cv::Scalar(200), &rect);
                if (count >= minArea_2 || (count >= minArea && y < part))
                {
                    OneNeuron neu;
                    neu.rect = rect;
                    neu.point_counter = count;
                    long Xs = 0, Ys = 0;
                    for(int i = 0; i < rect.width; i++)
                        for(int j = 0; j < rect.height; j++)
                        {
                            if((int)copyhsv.at<uchar>(rect.y+j, rect.x+i) > 0)
                            {
                                Xs += i;
                                Ys += j;
                            }
                        }
                    neu.center = cv::Point((int) Xs/count + rect.x, (int) Ys/count + rect.y);
                    neuronsList.push_back(neu);
                }
            }
        }
    }
}

void NeuronsMarker::calculateRectList()
{
    should_calc_rectList = false;
    rectList.clear();
    cv::Mat copyhsv = bin_image.clone();
    int part =(int) (copyhsv.rows*0.4);
    for (int y = 0; y<copyhsv.rows; y++)
    {
        for (int x = 0; x<copyhsv.cols; x++)
        {
            int value = copyhsv.at<uchar>(y, x);
            if (value == 255)
            {
                cv::Rect rect;
                int count = cv::floodFill(copyhsv, cv::Point(x, y), cv::Scalar(200), &rect,cv::Scalar(),cv::Scalar(),8);
                if (count >= minArea_2 || (count >= minArea && y < part))
                {
                    rectList.push_back(rect);
                }
            }
        }
    }
}

void NeuronsMarker::calculateCharacteristics()
{
    if(should_calc_neuList)
        this->calculateNeuList();
    should_calc_characteristics = false;
    unsigned long long Xs = 0, Ys = 0, Xs_0 = 0, Ys_0 = 0, total_points_counter = 0;
    for (std::vector<OneNeuron>::iterator i = neuronsList.begin(); i != neuronsList.end(); i++)
    {
        OneNeuron neu = *i;
        Xs += (neu.center.x*neu.point_counter);
        Xs_0 += neu.center.x;
        Ys += (neu.center.y*neu.point_counter);
        Ys_0 += neu.center.y;
        total_points_counter += neu.point_counter;
    }
    Xs /= total_points_counter;
    Xs_0 /= neuronsList.size();
    Ys /= total_points_counter;
    Ys_0 /= neuronsList.size();

    center_mass_1 = cv::Point((int)Xs, (int)Ys);
    center_mass_0 = cv::Point((int)Xs_0, (int)Ys_0);
    int** dist = new int* [2];
    dist[0] = new int[neuronsList.size()];
    dist[1] = new int[neuronsList.size()];

    int* dist_0 = new int[neuronsList.size()];
    int j = 0;

    for (std::vector<OneNeuron>::iterator i = neuronsList.begin(); i != neuronsList.end(); i++)
    {
        OneNeuron neu = *i;
        int dx =(int) (Xs-neu.center.x), dy =(int) (Ys-neu.center.y);
        dist[0][j] = (int)sqrt(dx*dx+dy*dy);
        dist[1][j] = neu.point_counter;
        j++;
    }

    unsigned long long rad = 0, _rad = 0;

    for(unsigned int i = 0; i < neuronsList.size(); i++)
    {
        rad += dist[0][i]*dist[1][i];
        _rad += dist[0][i];
    }
    rad /= total_points_counter;
    _rad /= neuronsList.size();

    rad_1 = (int)rad; rad_0 = (int)_rad;

    delete[] dist[0];
    delete[] dist[1];
    delete[] dist_0;
}

float NeuronsMarker::getAreaSeparator() const
{
    return areaSeparator;
}

void NeuronsMarker::setAreaSeparator(float value)
{
    areaSeparator = value;
}

unsigned short NeuronsMarker::getMinArea_2() const
{
    return minArea_2;
}

unsigned short NeuronsMarker::getMinArea_1() const
{
    return minArea;
}

std::vector<OneNeuron> NeuronsMarker::getNeuronsList()
{
    if(should_calc_neuList)
        this->calculateNeuList();

    return neuronsList;
}

std::vector<cv::Rect> NeuronsMarker::getRectsList()
{
    if(should_calc_rectList)
        this->calculateRectList();

    return rectList;
}

cv::Mat NeuronsMarker::getMarkedImg(cv::Mat img, bool should_mark_center)
{
    if(should_calc_neuList)
        this->calculateNeuList();

    if(should_calc_characteristics && should_mark_center)
        this->calculateCharacteristics();

    cv::Mat markedImg = img.clone();

    if(markedImg.size == bin_image.size)
    {
        for (std::vector<OneNeuron>::iterator i = neuronsList.begin(); i != neuronsList.end(); i++)
        {
            OneNeuron neu = *i;
            /*cv::line(markedImg, center_mass_1, neu.center, CV_RGB(0,255,255), 1, 8);
            cv::line(markedImg, center_mass_0, neu.center, CV_RGB(255,255,0), 1, 8);
            cv::rectangle(markedImg, neu.rect, cv::Scalar(0, 255, 0), 1);
            cv::circle(markedImg, neu.center, 5, cv::Scalar(0, 255, 0), 2);*/

            cv::rectangle(markedImg, neu.rect, CV_RGB(255,0,0), 1);
        }
        if(should_mark_center)
        {
            cv::circle(markedImg, center_mass_1, rad_1, CV_RGB(0,255,255), 8);
            cv::circle(markedImg, center_mass_0, rad_0, CV_RGB(255,255,0), 8);
            cv::circle(markedImg, center_mass_1, 20, CV_RGB(0,255,255), 8);
            cv::circle(markedImg, center_mass_0, 20, CV_RGB(255,255,0), 8);
        }
    }
    return markedImg;
}

std::vector<int> NeuronsMarker::get_characteristics()
{
    if(should_calc_neuList)
        this->calculateCharacteristics();

    int myshort[] = {center_mass_0.x, center_mass_0.y, rad_0, center_mass_1.x, center_mass_1.y, rad_1};
    return std::vector<int> (myshort, myshort + 6 );

}

NeuronsMarker::~NeuronsMarker()
{
    neuronsList.clear();
}
