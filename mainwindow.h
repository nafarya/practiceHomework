#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <qthread.h>
#include <QPoint>
#include <QLine>
#include <QStringListModel>
#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void zoom(int delta);
    void mouseReleaseEvent(Qt::MouseButton button);


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QStringList lineList;
    QStringListModel *model;
    QVector<double> curLineVecDist;
    QVector<QVector<double> > dists;

    QPointF prevPoint;
    QLine prevLine;

    bool drawing = false;
    double scaleFactor = 1.15;
    double curLineDist = 0;

    void drowLine(QPointF pos);
    void mousePressEvent(QMouseEvent* ev);
    void mouseDoubleClickEvent(QMouseEvent * e );
    void keyPressEvent(QKeyEvent *event);

    void calculateTheTable();



};

#endif // MAINWINDOW_H
