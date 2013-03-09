#pragma once
#include <vector>
#include <deque>
#include <QPointF>

namespace afc
{
class Convex_Hull
{
    std::deque< QPointF > m_stack;
    const double orient( const QPointF& p);
public:
    std::deque< QPointF > compute(std::vector< QPointF > m_points);

};

}
