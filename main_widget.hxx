#pragma once

#include <QVector>
#include <QWidget>
#include <vector>
#include <deque>

class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QStandardItemModel;
class QTableView;
class QwtPlot;
class QwtPlotCurve;
class QwtPlotMarker;
class QwtSymbol;
class QPointF;

class  Main_Widget : public QWidget
{
  Q_OBJECT
public:
    Main_Widget(QWidget *parent = nullptr);
    ~Main_Widget();

private:
    QDoubleSpinBox     *m_xSpin;
    QDoubleSpinBox     *m_ySpin;
    QLabel             *m_length;
    QPushButton        *m_addPointButton;
    QPushButton        *m_runButton;
    QStandardItemModel *m_inputModel;
    QStandardItemModel *m_outputModel;
    QTableView         *m_inputView;
    QTableView         *m_outputView;
    QwtPlot            *m_plot;
    QwtPlotCurve       *m_curve;

    QVector< QPointF > m_points;
    QVector< QPointF > m_convexHull;
    std::vector< QwtPlotMarker* > m_markers;
    std::vector< QwtSymbol*     > m_symbols;

    void generateConvexHullModel( const std::deque< QPointF >& convexHull );
    void setupUI();
private slots:
    void addPoint();
    void run();
};

