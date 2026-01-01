#ifndef ARMOR_DETECTOR_H
#define ARMOR_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <utility>
#include "ToolUtils.h"
#include "ArmorConfig.h"
#include "LightBar.h"
using namespace cv;
using namespace std;

class ArmorDetector
{
private:
    // 灯条配对有效性校验
    bool isPairValid(const LightBar& bar1, const LightBar& bar2, float& total_score) const;
    // 筛选最优装甲板配对
    int selectBestPair(const vector<pair<LightBar, LightBar>>& all_pairs) const;

public:
    // 核心接口：单帧处理
    void processFrame(const Mat& frame, Mat& result);
};

#endif // ARMOR_DETECTOR_H