#include "LightBar.h"

// 灯条属性计算
LightBar::LightBar(const vector<Point>& contour) {
    m_rect = minAreaRect(contour);
    m_area = contourArea(contour);
    
    m_width = min(m_rect.size.width, m_rect.size.height);
    m_height = max(m_rect.size.width, m_rect.size.height);
    m_ratio = m_height / (m_width + ArmorConfig::EPS);
    m_angleDeviation = ToolUtils::correctAngle(m_rect);
}

// 有效性校验
bool LightBar::isValid() const {
    return (m_area > ArmorConfig::AREA_THRESHOLD && 
            m_ratio > ArmorConfig::RATIO_THRESHOLD && 
            m_ratio < 15.0f && 
            m_angleDeviation < 60.0f);
}