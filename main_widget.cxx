#include "main_widget.hxx"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QListView>
#include <QItemSelectionModel>
#include <QList>
#include <QPointF>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QFileDialog>

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

    QHBoxLayout *pointButtonsLayout = new QHBoxLayout;
    pointButtonsLayout->addWidget( m_addPointButton );
    pointButtonsLayout->addWidget( m_delPointButton );

    QHBoxLayout *fileButtonsLayout = new QHBoxLayout;
    fileButtonsLayout->addWidget( m_openFileButton );
    fileButtonsLayout->addWidget( m_saveFileButton );

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout( fileButtonsLayout );
    leftLayout->addLayout( spinsLayout );
    leftLayout->addLayout( pointButtonsLayout );
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
  , m_delPointButton( new QPushButton( tr( "Delete Point" ), this) )
  , m_openFileButton( new QPushButton( tr( "Open File" ), this) )
  , m_saveFileButton( new QPushButton( tr( "Save File"), this) )
  , m_runButton(      new QPushButton( tr( "Run" ), this) )
  , m_inputModel(     new QStringListModel( this ) )
  , m_outputModel(    new QStringListModel( this ) )
  , m_inputSelectionModel( new QItemSelectionModel( m_inputModel, this ))
  , m_inputView(      new QListView( this ) )
  , m_outputView(     new QListView( this ) )
  , m_plot(           new QwtPlot( tr( "Illustration" ), this) )
  , m_curve(          new QwtPlotCurve( tr( "Solution" ) ) )
{
    setupUI();
    m_inputView->setSelectionModel( m_inputSelectionModel );
    m_plot->setAutoReplot();
    m_plot->setAxisAutoScale( 0 );
    m_plot->setAxisAutoScale( 1 );
    m_plot->setAxisScale( QwtPlot::xBottom, -5, 20 );
    m_plot->setAxisScale( QwtPlot::yLeft, -5, 20 );

    m_curve->attach( m_plot );

    connect( m_addPointButton, SIGNAL(clicked()), this, SLOT(addPoint()) );
    connect( m_delPointButton, SIGNAL(clicked()), this, SLOT(delPoint()) );
    connect( m_runButton, SIGNAL(clicked()), this, SLOT(run()) );
    connect( m_openFileButton, SIGNAL(clicked()), this, SLOT(openFile()) );
    connect( m_saveFileButton, SIGNAL(clicked()), this, SLOT(saveFile()) );
}

Main_Widget::~Main_Widget()
{
    qDeleteAll( m_markers );
    delete m_curve;
}

void Main_Widget::generateConvexHullModel( const std::deque<QPointF>& convexHull )
{
    QStringList output;

    QVector< QPointF > samples;
    samples.reserve( convexHull.size() );

    auto prev( convexHull.back() );
    auto length( 0.0 );

    for (const auto& point : convexHull)
    {
        output << QString( "( %0; %1 )" ).arg( point.x(), 0, 'g', 2).arg( point.y(), 0, 'g', 2);

        samples << point;

        const auto dx = point.x() - prev.x();
        const auto dy = point.y() - prev.y();
        length += sqrt( dx * dx + dy * dy );
        prev = point;
    }

    m_outputModel->setStringList( output );

    samples.push_back( samples.front() );
    m_curve->setSamples( samples );

    m_length->setText( QString::number( length, 'g', 2));
}

void Main_Widget::clearOutput()
{
    m_curve->setSamples( QVector< QPointF>() );
    m_outputModel->setStringList( QStringList() );
    m_length->setText( "0" );
}

void Main_Widget::run()
{
    if ( 3 > m_points.size() )
        return;
    Convex_Hull ch;
    const auto& convexHull = ch.compute( m_points.toVector().toStdVector() );
    generateConvexHullModel( convexHull );

}

void Main_Widget::addPoint(const QPointF &point)
{
    m_points.push_back( point );
    m_markers.push_back( new QwtPlotMarker );
    QwtPlotMarker *marker = m_markers.back();

    marker->setSymbol( new QwtSymbol( QwtSymbol::Diamond, Qt::red, Qt::NoPen, QSize(10, 10) ) );
    marker->setValue( point );
    marker->attach( m_plot );

    m_inputModel->insertRow( m_inputModel->rowCount() );
    QModelIndex index = m_inputModel->index( m_inputModel->rowCount() - 1);
    m_inputModel->setData(index, QString( "( %0; %1 )" )
                          .arg( point.x(), 0, 'g', 2)
                          .arg( point.y(), 0, 'g', 2)
                          );
}

void Main_Widget::addPoint()
{
    const QPointF pointToAdd( m_xSpin->value(), m_ySpin->value( ) );

    if (m_points.contains( pointToAdd ))
        return; // do nothing

    clearOutput();

    addPoint( pointToAdd );
}

void Main_Widget::delPoint()
{
    const int row = m_inputSelectionModel->currentIndex().row();

    if (-1 == row)
        return;

    const auto x = m_points.at( row ).x();
    const auto y = m_points.at( row ).y();
    m_points.removeAt( row );


//    m_markers.at( row )->detach();
    delete m_markers.at( row );
    m_markers.removeAt( row );

    m_inputModel->removeRow( row );

    clearOutput();

    m_plot->replot();

    m_xSpin->setValue( x );
    m_ySpin->setValue( y );

}
#include <QFile>
#include <QDataStream>
void Main_Widget::openFile()
{
    const QString fileName = QFileDialog::getOpenFileName( this, tr("Open File"));
    if (fileName.isNull())
        return;

    QFile file( fileName );
    if (!file.open( QIODevice::ReadOnly ) )
    {
        //throw
        return;
    }

    QDataStream in( &file );
    in.setVersion( QDataStream::Qt_4_8 );

    QVector< QPointF > points;

    in >> points;
    file.close();

    for (auto marker : m_markers)
        delete marker;
    m_markers.clear();

    m_inputModel->removeRows(0, m_points.size() );

    clearOutput();

    m_points.clear();
    m_points.reserve( points.size() );

    for (const auto& point : points )
    {
        addPoint( point );
    }
}


void Main_Widget::saveFile()
{
    const QString fileName = QFileDialog::getSaveFileName( this, tr("Save File") );
    if (fileName.isNull())
        return;

    QFile file( fileName );
    if (!file.open( QIODevice::WriteOnly ))
    {
        //throw error;
        return;
    }

    QDataStream out( &file );
    out.setVersion( QDataStream::Qt_4_8 );

    out << m_points;

    file.close();
}
