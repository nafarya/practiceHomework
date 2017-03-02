//#include "mainwindow.h"
#include <QApplication>
#include <QString>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <iostream>

#include <QFile>
#include <QTextStream>

using namespace cv;

const char* filename = "black.tif";

void detectLines() {
    IplImage* src = 0;
    IplImage* dst = 0;
    IplImage* color_dst = 0;
    src = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

    // хранилище памяти для хранения найденных линий
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i = 0;


    dst = cvCreateImage( cvGetSize(src), 8, 1);
    color_dst = cvCreateImage( cvGetSize(src), 8, 3);

    // детектирование границ
    cvCanny( src, dst, 50, 200, 3 );

    // конвертируем в цветное изображение
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );

    // нахождение линий
    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10);

    // нарисуем найденные линии
    freopen("lines.txt", "w", stdout);
    for( i = 0; i < lines->total; i++ ){
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
            std::cout << sqrt((line[0].x - line[1].x) * (line[0].x - line[1].x) + (line[0].y - line[1].y) * (line[0].y - line[1].y)) << std::endl;
            cvLine(color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0);
    }
    cvNamedWindow("Source", CV_WINDOW_NORMAL );
    cvShowImage("Source", src);

    cvNamedWindow("Hough", CV_WINDOW_NORMAL );
    cvShowImage("Hough", color_dst);

    cvWaitKey(0);

    cvReleaseMemStorage(&storage);
    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    cvReleaseImage(&color_dst);
    cvDestroyAllWindows();
}

void detectCircles() {
    IplImage* image = 0;
    // имя картинки задаётся первым параметром
    // получаем картинку (в градациях серого)
    image = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

    // загрузим оригинальное изображении
    IplImage* src = cvLoadImage(filename);

    // хранилище памяти для кругов
    CvMemStorage* storage = cvCreateMemStorage(0);
    // сглаживаем изображение
    cvSmooth(image, image, CV_GAUSSIAN, 5, 5);
    // поиск кругов
    CvSeq* results = cvHoughCircles(
            image,
            storage,
            CV_HOUGH_GRADIENT,
            10,
            image->width / 5
            );
    // пробегаемся по кругам и рисуем их на оригинальном изображении
    for( int i = 0; i < results->total; i++ ) {
            float* p = (float*) cvGetSeqElem(results, i);
            CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
            cvCircle(src, pt, cvRound(p[2]), CV_RGB(0xff, 0, 0));
    }
    cvNamedWindow( "cvHoughCircles", CV_WINDOW_NORMAL );
    cvShowImage( "cvHoughCircles", src);

    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseMemStorage(&storage);
    cvReleaseImage(& image);
    cvReleaseImage(&src);
    cvDestroyAllWindows();
}

void detectCurvedLines() {
        Mat src = imread(filename, 0);
        Mat dst, cdst;
        Canny(src, dst, 50, 200, 3);
        cvtColor(dst, cdst, COLOR_GRAY2BGR);
    #if 0
        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
    #else
        std::vector<Vec4i> lines;
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        freopen("curvedlines.txt", "w", stdout);
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            Point p(l[0], l[1]);
            Point p1(l[2], l[3]);
            std::cout << sqrt((p.x - p1.x) * (p.x - p1.x) + (p.y - p1.y) * (p.y - p1.y)) << std::endl;
            line(cdst, p, p1, Scalar(0, 0, 255), 3, LINE_AA);
        }
    #endif
        cvNamedWindow( "source", CV_WINDOW_NORMAL );
        imshow("source", src);

        cvNamedWindow( "detected curved lines", CV_WINDOW_NORMAL );
        imshow("detected curved lines", cdst);

        waitKey();

}

int main()
{

    detectLines();
//    detectCircles();
    detectCurvedLines();
    return 0;
}
