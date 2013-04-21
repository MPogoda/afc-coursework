#include "convex_hull.hxx"
#include <algorithm>

using namespace afc;
using std::vector;
using std::deque;
using std::sort;
using std::sqrt;
using std::swap;
using std::min_element;

namespace {
const bool bottomComparator(const QPointF& a, const QPointF& b)
{
    const bool result = (a.y() < b.y()) ? true :
      ((a.y() == b.y()) && (a.x() < b.x()) ? true : false);
    return result;
}

const
double ccw( const QPointF& pt1, const QPointF& pt2, const QPointF& pt3)
{
    return (pt2.x() - pt1.x()) * (pt3.y() - pt1.y())
            - (pt2.y() - pt1.y()) * (pt3.x() - pt1.x());
}

class AngleComparator
{
    QPointF m_central;

public:

    AngleComparator( const QPointF& c );

    const bool operator()( const QPointF& a, const QPointF& b) const;
    const double angle( const QPointF& x) const;
};

AngleComparator::AngleComparator( const QPointF& central)
    : m_central(central) { }

const bool AngleComparator::operator()( const QPointF& a, const QPointF& b) const
{
    return angle(a) < angle(b);
}

inline const double AngleComparator::angle( const QPointF& other) const
{
    const QPointF t( other.x() - m_central.x(), other.y() - m_central.y() );
    // TODO : check whether std::acos is needed.
    return t.x() / sqrt(t.x() * t.x() + t.y() * t.y() );
}

}

const double Convex_Hull::orient(const QPointF& p3)
{
    const auto p1 = m_stack.back();
    m_stack.pop_back();
    const auto result = ccw(p1, m_stack.back(), p3);
    m_stack.push_back(p1);
    return result;
}

std::deque<QPointF> Convex_Hull::compute(std::vector<QPointF> points)
{
    auto min = min_element(points.begin(), points.end(), bottomComparator);
    swap(*min, *points.begin());
    sort(points.begin() + 1, points.end(), AngleComparator(points[0]));

    auto it = points.begin();

    for (std::size_t i( 0 ); i != 3; ++i, ++it)
        m_stack.push_back(*it);

    while (points.end() != it)
    {
        const auto& point = *it;
        while (orient(point) < 1e-9)
            m_stack.pop_back();
        m_stack.push_back(point);

        ++it;
    }

    return m_stack;
}
