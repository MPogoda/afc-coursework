#pragma once

#include <QList>
#include <QWidget>
#include <vector>
#include <deque>

class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QStringListModel;
class QItemSelectionModel;
class QListView;
class QwtPlot;
class QwtPlotCurve;
class QwtPlotMarker;
class QPointF;

class  Main_Widget : public QWidget
{
  Q_OBJECT
public:
    Main_Widget(QWidget *parent = nullptr);
    ~Main_Widget();

private:
    QDoubleSpinBox      *m_xSpin;
    QDoubleSpinBox      *m_ySpin;
    QLabel              *m_length;
    QPushButton         *m_addPointButton;
    QPushButton         *m_delPointButton;
    QPushButton         *m_openFileButton;
    QPushButton         *m_saveFileButton;
    QPushButton         *m_runButton;
    QStringListModel    *m_inputModel;
    QStringListModel    *m_outputModel;
    QItemSelectionModel *m_inputSelectionModel;
    QListView           *m_inputView;
    QListView           *m_outputView;
    QwtPlot             *m_plot;
    QwtPlotCurve        *m_curve;

    QList< QPointF > m_points;
    QList< QwtPlotMarker* > m_markers;

    void generateConvexHullModel( const std::deque< QPointF >& convexHull );
    void setupUI();
    void clearOutput();
    void addPoint( const QPointF& point );
private slots:
    void addPoint();
    void delPoint();
    void run();
    void openFile();
    void saveFile();
};

