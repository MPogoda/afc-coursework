#include "pointf.hxx"
#include <cmath>

using namespace afc;

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
    return t.x() / std::sqrt(t.x() * t.x() + t.y() * t.y() );
}

