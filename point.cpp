#include "point.hpp"
void Point::movePoint()
{
     if (target_x_ < curr_x_)
        curr_x_ -= velocity_;
    else if (target_x_ > curr_x_)
        curr_x_ += velocity_;
    if (target_y_ < curr_y_)
        curr_y_ -= velocity_;
    else if (target_y_ > curr_y_)
        curr_y_ += velocity_;
}
