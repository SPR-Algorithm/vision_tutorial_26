#include "ToolUtils.h"


float ToolUtils::correctAngle(const RotatedRect& r_rect) {
    float angle = r_rect.angle;
    Size2f size = r_rect.size;
    
    if (size.width > size.height) {
        swap(size.width, size.height);
        angle += 90.0f;
    }
    angle = fmod(angle, 90.0f);
    if (angle < 0) angle += 90.0f;
    return min(angle, 90.0f - angle);
}
float ToolUtils::calcDistance(const Point2f& p1, const Point2f& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrt(dx*dx + dy*dy);
}
void ToolUtils::sortVerticesByY(Point2f pts[4], Point2f& top, Point2f& bottom) {
    vector<pair<float, int>> y_values;
    for (int i = 0; i < 4; i++) {
        y_values.push_back({pts[i].y, i});
    }
    sort(y_values.begin(), y_values.end());
    top = (pts[y_values[0].second] + pts[y_values[1].second]) * 0.5f;
    bottom = (pts[y_values[2].second] + pts[y_values[3].second]) * 0.5f;
}