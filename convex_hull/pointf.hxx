#pragma once

namespace afc
{
class PointF
{
    double m_x;
    double m_y;
public:
    PointF( const double x = 0.0, const double y = 0.0) : m_x(x), m_y(y) { }

    const double& x() const { return m_x; }
    const double& y() const { return m_y; }
    double& x() { return m_x; }
    double& y() { return m_y; }
};

class AngleComparator
{
    PointF m_central;

public:

    AngleComparator( const PointF& c );

    const bool operator()( const PointF& a, const PointF& b) const;
    const double angle( const PointF& x) const;
};

//const bool bottomComparator( const PointF& a, const PointF& b);

//const double ccw(const PointF& pt1, const PointF& pt2, const PointF& pt3);

}
