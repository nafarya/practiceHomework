#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <qthread.h>
#include <QPoint>
#include <QLine>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QStringList lineList;
    QStringListModel *model;

    QPointF prevPoint;

    bool drawing = false;
    double scaleFactor = 1.15;
    double curLineDist = 0;

    void drowLine(QPointF pos);
    void mousePressEvent(QMouseEvent* ev);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
