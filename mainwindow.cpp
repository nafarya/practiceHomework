#include "mainwindow.h"
#include "ui_mainwindow.h"

class MyScene : public QGraphicsScene {

public :
    MyScene(QObject *parent = Q_NULLPTR) : QGraphicsScene(parent) {}
    ~MyScene(){}

    void wheelEvent(QGraphicsSceneWheelEvent *event) {
        ((MainWindow*)parent())->zoom(event->delta());
        event->accept();
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        ((MainWindow*)parent())->mouseReleaseEvent(event->button());
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(640,640);
    scene = new MyScene(this);

    ui->graphicsView->resize(500, 500);
    scene->addPixmap(QPixmap("black.tif"));
    ui->graphicsView->setScene(scene);
    scene->setFocus();

    model = new QStringListModel(this);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::zoom(int delta)
{
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (delta > 0) {
        ui->graphicsView-> scale(scaleFactor, scaleFactor);
    } else {
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::drowLine(QPointF pos)
{
    if (drawing) {
        QLine line = QLine(prevPoint.x(), prevPoint.y(), pos.x(), pos.y());
        scene->addLine(line, QPen(Qt::red, 1, Qt::SolidLine));
        curLineDist = sqrt(pow(prevPoint.x() - pos.x(), 2) + pow(prevPoint.y() - pos.y(), 2));
        curLineVecDist.push_back(curLineDist);
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
        if (curLineVecDist.size() == 1) {
            drawing = false;
        }
        if (curLineVecDist.size() > 0) {
            scene->removeItem(scene->items().at(0));
            curLineVecDist.pop_back();
        }

    }
    if(ev->button() == Qt::MiddleButton)
    {
        // pointer to your graphics view
        ui->graphicsView->setInteractive(true);
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    }
    ev->accept();
}

void MainWindow::mouseReleaseEvent(Qt::MouseButton button)
{
    if(button == Qt::MiddleButton)
    {
        // pointer to your graphics view
        ui->graphicsView->setInteractive(false);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QPoint origin = ui->graphicsView->mapFromGlobal(QCursor::pos());
    QPointF relativeOriginPos = ui->graphicsView->mapToScene(origin);
    if (e->button() == Qt::LeftButton )
    {
        drowLine(relativeOriginPos);
        calculateTheTable();
        drawing = false;
    }
    e->accept();
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
    if (event->key() == Qt::Key_Backspace) {
        QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
       if (!selected.isEmpty())
       {
          lineList.removeAt(selected.first().row());
          ((QStringListModel*) ui->listView->model())->setStringList(lineList);

          int selectedRow = selected.first().row();
          int x = 0;
          for (int i = dists.size() - 1; i > selectedRow; i--) {
                x += dists[i].size();
          }
          for (int i = x; i < x + dists[selectedRow].size(); i++) {
             scene->removeItem(scene->items().at(0));
          }
          dists.erase(dists.begin() + selected.first().row());

       }
    }
    event->accept();
}

void MainWindow::calculateTheTable()
{
    lineList.clear();
    dists.push_back(curLineVecDist);
    for (int i = 0; i < dists.size(); i++) {
        double sum = 0;
        for (int j = 0; j < dists[i].size(); j++) {
            sum += dists[i][j];
        }
        lineList.append(QString::number(i + 1) + ": " + QString::number(sum));

    }
    model->setStringList(lineList);
    ui->listView->setModel(model);
    curLineDist = 0;
    curLineVecDist.clear();

    double mathExpectedValue = 0;
    for (int i = 0; i < dists.size(); i++) {
        for (int j = 0; j < dists[i].size(); j++) {
            mathExpectedValue += dists[i][j];
        }
    }
    mathExpectedValue /= (dists.size());

    ui->label->setText(QString::number(mathExpectedValue));

    double disp = 0;
    for (int i = 0; i < dists.size(); i++) {
        double curDisp = 0;
        double curSum = 0;
        for (int j = 0; j < dists[i].size(); j++) {
            curSum += dists[i][j];
        }
        curDisp = pow(curSum - mathExpectedValue, 2);
        disp += curDisp;
    }
    ui->label_2->setText(QString::number(sqrt(disp / dists.size())));


}



