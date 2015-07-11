#include <QCoreApplication>
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

//#define DO_PLIKU

int main (int argc, char * argv[])
{
    VideoCapture cap(1);
    if (!cap.isOpened())
        return -1;

    Mat img, img90, img270, rotationMat90, rotationMat270;
#ifdef DO_PLIKU
    cap>>img;
    VideoWriter output;
    output.open("test.avi",-1,5,Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH)/2,(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)/2),true);
    if(output.isOpened()!=true){
        return -1;
    }
#endif
    HOGDescriptor hog;
    hog.load("detektor1.txt");

    namedWindow("img", CV_WINDOW_AUTOSIZE);
    while (true)
    {
        cap >> img;
        pyrDown(img,img);

        Size rotatedSize(img.size().height,img.size().width);
        Point center(img.size().width/2, img.size().height/2);
        //Macierz translacji, żeby się mieścił obrócony obraz w nowej macierzy
        double t90[3][3]={
            {1.0,0.0,-img.size().width/2.0+img.size().height/2.0},
            {0.0,1.0,+img.size().height/2.0-img.size().width/2.0},
            {0.0,0.0,1.0}
        };
        Mat translationMat90(3,3,CV_64FC1,t90);
        double t270[3][3]={
                    {1,0,-img.size().height/2.0+img.size().width/2.0},
                    {0,1,+img.size().width/2.0-img.size().height/2.0},
                    {0,0,1}
                };
        Mat translationMat270(3,3,CV_64FC1,t270);
        rotationMat90 = getRotationMatrix2D( center, 90, 1 )*translationMat90;
        rotationMat270 = getRotationMatrix2D( center, 270, 1 )*translationMat270;
        warpAffine( img, img90, rotationMat90, rotatedSize );
        warpAffine( img, img270, rotationMat270, rotatedSize );

        if (!img.data)
            continue;

        vector<Rect> results90, results270;
        hog.detectMultiScale(img90, results90, 0, Size(8,8), Size(32,32), 1.05, 2);
        hog.detectMultiScale(img270, results270, 0, Size(8,8), Size(32,32), 1.05, 2);

        if(results90.size()){
            cout<<"90: " << results90.size() << "\n";
        }
        if(results270.size()){
            cout<<"270: " << results270.size() << "\n";
        }
        //Różnymi kolorami zaznaczane są wyniki otrzymane dla kąta 90 i 270
        for(unsigned i=0; i<results90.size(); i++){
            Point tl=results90[i].tl();
            Point br=results90[i].br();
            //rectangle(img90, tl, br, cv::Scalar(0,255,0), 2);
            tl=Point(img.size().width-tl.y,tl.x);
            br=Point(img.size().width-br.y,br.x);
            rectangle(img, tl, br, cv::Scalar(0,255,0), 2);
        }
        for(unsigned i=0; i<results270.size(); i++){
            Point tl=results270[i].tl();
            Point br=results270[i].br();
            //rectangle(img270, tl, br, cv::Scalar(255,0,0), 2);
            tl=Point(tl.y,img.size().height-tl.x);
            br=Point(br.y,img.size().height-br.x);
            rectangle(img, tl, br, cv::Scalar(255,0,0), 2);
        }

        //imshow("90", img90);
        //imshow("270", img270);
        imshow("img", img);
#ifdef DO_PLIKU
        output << img;
#endif
        if (waitKey(20) >= 0)
            break;
    }
    return 0;
}
