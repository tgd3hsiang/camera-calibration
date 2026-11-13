#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace cv;
using namespace std;


int main() {
    ifstream fin1;
    fin1.open("testdata.txt");
    vector<vector<Point2f>> imgP;
    vector<vector<Point3f>> objP;
    Mat temp = imread("1_L.jpg");
    Size img_size = temp.size();
    cout << img_size.height << "," << img_size.width << endl;
    Size board_size = Size(6,9);//測資1
    TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.001); //最終佚待還是怎麼樣的
    

    string imgName;
    
    int imgNum = 0;
    while (getline(fin1, imgName)) {
        cout << "imgName : " << imgName << endl;
        imgNum++;
        Mat img = imread(imgName);
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        imshow("Camera Calibration", img);
        int l = waitKey(0);
        vector<Point2f> corners;
        bool rat = findChessboardCorners(gray, board_size, corners);
        if (!rat) {
            cout << "can not find corners\n";
            imgNum--;
        }
        else {
            drawChessboardCorners(img, board_size, corners, true);
            imgP.push_back(corners);
            
            imshow("Camera Calibration", img);//顯示圖片  
            l = waitKey(0);
        }
    }
    int squareWidth = 10 ;
    int squareHeight = 10; //等助教回
    for (int t = 0; t < imgNum; t++) {
        vector<Point3f> obj;
        for (int i = 0; i < board_size.height; i++) {
            for (int j = 0; j < board_size.width; j++) {
                obj.push_back(Point3f(j * squareWidth, i * squareHeight, 0));
            }
        }
        objP.push_back(obj);
    }

    Mat intrinsic = Mat(3, 3, CV_32FC1);//相機內參數，作業中的R
    Mat distCoeffs = Mat(1, 5, CV_32FC1); //畸變係數，作業中的D
    vector<Mat> tvecs;//平移向量
    vector<Mat> rvecs;//旋轉向量
    calibrateCamera(objP, imgP, img_size, intrinsic, distCoeffs, rvecs, tvecs);
    
    ifstream fin2;
    fin2.open("testdata.txt");
    while (getline(fin2, imgName)) {
        Mat dst;
        Mat image = imread(imgName);
        undistort(image, dst, intrinsic, distCoeffs);
        imshow("image", image);
        imshow("undistort image", dst);
        int l = waitKey(0);
    }


    int k = waitKey(0);

    return 0;
}

/*
Mat img = Mat::zeros(Size(800, 600), CV_8UC3);
    img.setTo(255);

    Point p1(100, 100);
    Point  p2(200, 300);
    Point  p3(300, 200);

    vector<Point> pts;
    pts.push_back(p1);
    pts.push_back(p2);
    pts.push_back(p3);
    polylines(img, pts, true, (0, 255, 255),1,8,0);

    Mat img2 = Mat::zeros(Size(800, 600), CV_8UC3);
    img2.setTo(255);

    Point p4(50, 100);
    Point  p5(300, 300);
    Point  p6(400, 200);

    vector<Point> pts2;
    pts2.push_back(p4);
    pts2.push_back(p5);
    pts2.push_back(p6);
    polylines(img2, pts, true, (0, 255, 255), 1, 8, 0);

    if (img.at<Vec3b>(200, 100) != Vec3b(255, 255, 255)) {
        cout << "Nope";
    }
    else {
        cout << "Yes";
    }



    imshow("image", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    if (k == 's')
    {
        imwrite("starry_night.png", img);
    }
    return 0;
    */
