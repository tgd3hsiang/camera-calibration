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
    Size board_size = Size(6,9);
    TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.001); 
    

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
            
            imshow("Camera Calibration", img);
            l = waitKey(0);
        }
    }
    int squareWidth = 10 ;
    int squareHeight = 10; 
    for (int t = 0; t < imgNum; t++) {
        vector<Point3f> obj;
        for (int i = 0; i < board_size.height; i++) {
            for (int j = 0; j < board_size.width; j++) {
                obj.push_back(Point3f(j * squareWidth, i * squareHeight, 0));
            }
        }
        objP.push_back(obj);
    }

    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs = Mat(1, 5, CV_32FC1); 
    vector<Mat> tvecs;
    vector<Mat> rvecs;
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