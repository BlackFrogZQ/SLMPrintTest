#include "printDatasDef.h"

using namespace std;
namespace TIGER_PrintDatas
{
    bool pointDatas::operator==(pointDatas &p_coutour)
    {
        return this->x == p_coutour.x && this->y == p_coutour.y;
    }

    bool countourDatas::isClockwise(const vector<pointDatas>& contour)
    {
        double sum = 0.0;
        for (size_t i = 0; i < contour.size(); ++i)
        {
            const pointDatas& p1 = contour[i];
            const pointDatas& p2 = contour[(i + 1) % contour.size()];
            sum += (p2.x - p1.x) * (p2.y + p1.y);
        }
        return sum > 0.0;
    }
}