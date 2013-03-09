#include "main_widget.hxx"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>
#include <QVector>
#include <QPointF>
#include <QGroupBox>
#include <QDoubleSpinBox>

#include "convex_hull/convex_hull.hxx"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

using namespace afc;
using std::vector;
using std::deque;

void Main_Widget::setupUI()
{
    QSizePolicy sizePolicyForLabels( QSizePolicy::Maximum, QSizePolicy::Preferred);

    QLabel *xLabel = new QLabel( tr( "x:" ), this );
    xLabel->setSizePolicy( sizePolicyForLabels );
    xLabel->setBuddy( m_xSpin );
    QLabel *yLabel = new QLabel( tr( "y:" ), this );
    yLabel->setSizePolicy( sizePolicyForLabels );
    yLabel->setBuddy( m_ySpin );

    QHBoxLayout *spinsLayout = new QHBoxLayout;
    spinsLayout->addWidget( xLabel );
    spinsLayout->addWidget( m_xSpin );
    spinsLayout->addWidget( yLabel );
    spinsLayout->addWidget( m_ySpin );

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout( spinsLayout );
    leftLayout->addWidget( m_addPointButton );
    leftLayout->addWidget( m_inputView );
    m_inputView->setModel(m_inputModel);
    m_inputView->setEditTriggers( QAbstractItemView::NoEditTriggers);

    QGroupBox *leftGroupBox = new QGroupBox( tr( "Input" ), this );
    leftGroupBox->setLayout( leftLayout );


    QLabel *lengthLabel = new QLabel( tr( "Optimal length:" ), this );
    lengthLabel->setSizePolicy( sizePolicyForLabels );
    lengthLabel->setBuddy( m_length );
    m_length->setFrameStyle( QFrame::Panel );

    QHBoxLayout *labelsLayout = new QHBoxLayout;
    labelsLayout->addWidget( lengthLabel );
    labelsLayout->addWidget( m_length );

    QVBoxLayout *rightSubLayout = new QVBoxLayout;
    m_outputView->setModel( m_outputModel );
    m_outputView->setEditTriggers( QAbstractItemView::NoEditTriggers);
    rightSubLayout->addLayout( labelsLayout );
    rightSubLayout->addWidget( m_outputView );
    rightSubLayout->addWidget( m_plot );

    QGroupBox *rightGroupBox = new QGroupBox( tr( "Results" ), this );
    rightGroupBox->setLayout( rightSubLayout );

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget( m_runButton );
    rightLayout->addWidget( rightGroupBox );

    QHBoxLayout *mainLayout = new QHBoxLayout( this );
    mainLayout->addWidget( leftGroupBox );
    mainLayout->addLayout( rightLayout );
}

Main_Widget::Main_Widget(QWidget *parent)
  : QWidget(parent)
  , m_xSpin(          new QDoubleSpinBox( this ) )
  , m_ySpin(          new QDoubleSpinBox( this ) )
  , m_length(         new QLabel( "0", this) )
  , m_addPointButton( new QPushButton( tr( "Add Point" ), this) )
  , m_runButton(      new QPushButton( tr( "Run" ), this) )
  , m_inputModel(     new QStandardItemModel( 0, 2, this ) )
  , m_outputModel(    new QStandardItemModel( 0, 2, this ) )
  , m_inputView(      new QTableView( this ) )
  , m_outputView(     new QTableView( this ) )
  , m_plot(           new QwtPlot( tr( "Illustration" ), this) )
  , m_curve(          new QwtPlotCurve( tr( "Solution" ) ) )
{
    setupUI();
    m_plot->setAutoReplot();
    m_plot->setAxisAutoScale( 0 );
    m_plot->setAxisAutoScale( 1 );
    m_plot->setAxisScale( QwtPlot::xBottom, -5, 20 );
    m_plot->setAxisScale( QwtPlot::yLeft, -5, 20 );

    m_curve->attach( m_plot );

    connect( m_addPointButton, SIGNAL(clicked()), this, SLOT(addPoint()) );
    connect( m_runButton, SIGNAL(clicked()), this, SLOT(run()) );
}

Main_Widget::~Main_Widget()
{
//    for (auto symbol : m_symbols)
//        delete symbol;
//    for (auto marker : m_markers)
//        delete marker;
//    delete m_curve;
//    delete m_plot;
}

void Main_Widget::generateConvexHullModel( const std::deque<QPointF>& convexHull )
{
    m_outputModel->removeRows(0, m_outputModel->rowCount());

    QVector< QPointF > samples( convexHull.size() );

    for (auto it = convexHull.begin(); convexHull.end() != it; ++it)
    {
        const QPointF& point = *it;

        m_outputModel->appendRow( { new QStandardItem( QString::number( point.x() ))
                                 , new QStandardItem( QString::number( point.y() ))
                                 }
                               );

        samples << *it;
    }

    samples.push_back( samples.front() );

    double length( 0.0 );
    auto prev = convexHull.front();

    for (auto it = convexHull.begin() + 1; convexHull.end() != it; ++it)
    {
        const auto dx = it->x() - prev.x();
        const auto dy = it->y() - prev.y();
        length += sqrt( dx * dx + dy * dy );

        prev = *it;
    }

    const auto dx = prev.x() - convexHull.front().x();
    const auto dy = prev.y() - convexHull.front().y();

    length += sqrt( dx * dx + dy * dy);

    m_length->setText( QString::number( length ));

    m_curve->setSamples( samples );
}

void Main_Widget::run()
{
    Convex_Hull ch;
    const auto& convexHull = ch.compute( m_points.toStdVector() );
    generateConvexHullModel( convexHull );

}

void Main_Widget::addPoint()
{
    const QPointF pointToAdd( m_xSpin->value(), m_ySpin->value( ) );

    if (m_points.contains( pointToAdd ))
        return; // do nothing

    m_points.push_back( pointToAdd );
    const QPointF& point = m_points.back();
    m_markers.push_back( new QwtPlotMarker );
    QwtPlotMarker& marker = *m_markers.back();
    m_symbols.push_back( new QwtSymbol( QwtSymbol::Diamond, Qt::red, Qt::NoPen, QSize(10, 10) ) );

    marker.setSymbol( m_symbols.back() );
    marker.setValue( point );
    marker.attach( m_plot );

    m_plot->updateAxes();

    m_inputModel->appendRow( { new QStandardItem( QString::number( point.x() ) )
                             , new QStandardItem( QString::number( point.y() ) )
                             }
                           );
}
