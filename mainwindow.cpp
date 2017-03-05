#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(640,640);
    scene = new QGraphicsScene(this);

    ui->graphicsView->resize(500, 500);
    scene->addPixmap(QPixmap("black.tif"));
    ui->graphicsView->setScene(scene);
    scene->setFocus();

    model = new QStringListModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drowLine(QPointF pos)
{
    if (drawing) {
        QLine line = QLine(prevPoint.x(), prevPoint.y(), pos.x(), pos.y());
        scene->addLine(line, QPen(Qt::red, 1, Qt::SolidLine));
        curLineDist += sqrt(pow(prevPoint.x() - pos.x(), 2) + pow(prevPoint.y() - pos.y(), 2));
    }
}

void MainWindow::mousePressEvent(QMouseEvent* ev){
    QPoint origin = ui->graphicsView->mapFromGlobal(QCursor::pos());
    QPointF relativeOriginPos = ui->graphicsView->mapToScene(origin);

    if (ev->button() == Qt::LeftButton){
        drowLine(relativeOriginPos);
        prevPoint = relativeOriginPos;
        drawing = true;
    }
    else if (ev->button() == Qt::RightButton){
        drowLine(relativeOriginPos);
        drawing = false;
        lineList.append(QString::number(lineList.length() + 1) + ": " + QString::number(curLineDist));
        model->setStringList(lineList);
        ui->listView->setModel(model);
        curLineDist = 0;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    if (event->key() == Qt::Key_Plus) {
        ui->graphicsView-> scale(scaleFactor, scaleFactor);
    }
    if (event->key() == Qt::Key_Minus) {

        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

}



