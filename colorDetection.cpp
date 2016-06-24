#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>


#define WAITPHOTO 500
#define	START_PIN 10
#define COLOR1_PIN 27
#define COLOR2_PIN 28
#define READY_PIN 4
#define ERROR_PIN 29
#define SIMULATION_PIN 6

using namespace std;
using namespace cv;

void doDetect (void)
{
  Mat src, dst;
  VideoCapture cap;
  /// Load image
  //src = imread( "pic.jpg", 1 );

  //delay(WAITPHOTO);

  if(!cap.open(0)) {
    printf("kein Foto moeglich");
  }

  cap >> src;

  if( !src.data )
    { }

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImageR( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  Mat histImageB( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  Mat histImageG( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );


  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImageB.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImageG.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImageR.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImageB, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImageG, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImageR, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  imwrite("orginal.jpg",src);
  imwrite( "PicR.jpg", histImageR );
  imwrite("PicG.jpg", histImageG);
  imwrite("PicB.jpg", histImageB);

  float peakRed=0.0, peakGreen=0.0, peakBlue=0.0;
  int positionRed=0, positionGreen=0, positionBlue=0;
  bool blueXred = false;
  bool fallBlue = false;

  for (int j=0;j<255;j++) {
    if(r_hist.at<float>(j) > peakRed) {
       positionRed=j;
       peakRed=r_hist.at<float>(j);
    }
    if(g_hist.at<float>(j) > peakGreen) {
      positionGreen=j;
      peakGreen=g_hist.at<float>(j);
    }
    if(b_hist.at<float>(j) > peakBlue) {
      positionBlue=j;
      peakBlue=b_hist.at<float>(j);
    }
    if(b_hist.at<float>(j)+5 < peakBlue) {
      fallBlue = true;
    }
    if(b_hist.at<float>(j)-5 > peakBlue) {
      fallBlue = false;;
    }

    if(b_hist.at<float>(j) - r_hist.at<float>(j) <5 && b_hist.at<float>(j) - r_hist.at<float>(j) >-5) {
      if (fallBlue) {
        blueXred = true;
      }
    }
    //printf("%f",r_hist<float>(j));
  }

  printf("Peak Red: %f $$ Position: %d ????",peakRed,positionRed);
  printf("Peak Blue: %f $$ Position: %d ????",peakBlue,positionBlue);
  printf("Peak Green: %f $$ Position: %d ????",peakGreen,positionGreen);


  if (positionRed < 208 && positionRed >138 && positionBlue < 216 && positionBlue > 156 && positionGreen <212 && positionGreen > 152) {
    printf("Grey");
    digitalWrite (COLOR1_PIN, HIGH);
    digitalWrite (COLOR2_PIN, HIGH);
  } else if (positionRed < 266 && positionRed >196 && positionBlue < 210 && positionBlue > 140 && positionGreen <159 && positionGreen > 89) {
    printf("Red");
    digitalWrite (COLOR1_PIN, LOW);
    digitalWrite (COLOR2_PIN, LOW);
    delay(200);
  } else if (positionRed < 100 && positionRed > 50 && positionBlue < 180 && positionBlue > 130 && positionGreen < 170 && positionGreen >120) {
    printf("Blue");
    digitalWrite (COLOR1_PIN, HIGH);
    digitalWrite (COLOR2_PIN, LOW);

  } else if (positionRed < 188 && positionRed > 148 && positionBlue < 192 && positionBlue > 132 && positionGreen < 226 && positionGreen >166) {
    printf("Green");
    digitalWrite (COLOR1_PIN, LOW);
    digitalWrite (COLOR2_PIN, HIGH);
  } else if (positionBlue < 255/4 && positionGreen < positionRed+20) {
    printf("Red");
    digitalWrite (COLOR1_PIN, LOW);
    digitalWrite (COLOR2_PIN, LOW);
    delay(200);
  }

  digitalWrite (READY_PIN, HIGH);
  digitalWrite (SIMULATION_PIN, HIGH);
}

/**
 * @function main
 */
int main( int argc, char** argv )
{
  digitalWrite (SIMULATION_PIN, HIGH);
  if (wiringPiSetup () < 0)
    {
       printf ("Unable to setup wiringPi") ;
       return 1 ;
    }
    pinMode (COLOR1_PIN, OUTPUT);
    pinMode (COLOR2_PIN, OUTPUT);
    pinMode (READY_PIN, OUTPUT);
    pinMode (40, OUTPUT);
    pinMode (SIMULATION_PIN, OUTPUT);
    pinMode (START_PIN, INPUT);
 // while (true) {

    //if (wiringPiISR (START_PIN, INT_EDGE_RISING, &doDetect) < 0)
    //{
    //   printf ("Unable to setup ISR") ;
    //   return 1 ;
    //}
doDetect();
  //}

  return 0;
}
