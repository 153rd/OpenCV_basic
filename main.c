#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

int             g_slider_pos = 0;
CvCapture*      g_capture    = NULL;
void onTrackbarSlide(int pos)
{
    cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);
}

//TODO: Split into multiple files.
int main(int argc, char** argv )
{
    int c, clr, n2;
    char n[4];
    printf("Choose which camera you want to see, 0 is default: \n");
    c = (getchar()-48);

    if((clr = getchar()) != '\n' && c != EOF)
    {
        printf(" cleared..\n");
    };

    printf("Now type the amount of frames you would like to capture: \n");
    fgets(n, 10, stdin);
    n2 = atoi(n);

    printf("Showing %i frames on camera %i \n",n2, c);
    //TODO: Hier constants van maken: ENABLE_CANNY, etc..
    displayCameraInput(c, n2, 0, 0, 20, 5);


    return 0;
}

/*
// TODO (Everhard#1#08/05/14): @todo:  ...
// Pointers maken van functies, waardoor ze vanuit console aangeroepen kunnen worden
//Dit zorgt ervoor dat er geen switch statements nodig zijn.

TODO
Enable/disable canny edge detection

Description:
Function that can read an image stream from a camera, selected by the user.
It will also write to file "camera capture.mpeg" in the Release-folder.


*/
void displayCameraInput(int camera_id, int frame_numbers, int canny_enable, double tresh_low, double tresh_high, int aperture)
{
    int frameCounter, fps;
    //Image variable to store frame
    IplImage* cappedFrame;

    CvCapture* capture = cvCaptureFromCAM(camera_id);
    CvSize size = cvSize((int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),
                         (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));

    CvVideoWriter *writer = cvCreateVideoWriter("Camera Capture.mpeg", CV_FOURCC('M','P','E','G'), 16, size, 1);
    CvVideoWriter *cannyWriter = cvCreateVideoWriter("Canny Camera Capture.mpeg", CV_FOURCC('M','P','E','G'), 16, size, 1);
    //Channels
    IplImage* cannyFrame_c0 = cvCreateImage(size, IPL_DEPTH_8U, 1);

    if(capture == NULL)
    {
        printf("Camera was not found.. \n");
    }

    for(frameCounter = 0;frameCounter < frame_numbers;frameCounter++)
    {
        cappedFrame = cvQueryFrame(capture);
        if(cappedFrame == NULL)
        {
            printf("Frame is null, breaking. \n");
            break;
        }

        if (canny_enable == 0)
        {
            cvWriteFrame(writer, cappedFrame);
            cvShowImage("CameraWindow", cappedFrame);
        }
        //Default enable
        //if(canny_enable == 1)
        //{
            //cvSplit(cappedFrame, cannyFrame_c0, NULL, NULL, NULL);
            cvCvtColor(cappedFrame, cannyFrame_c0, CV_RGB2GRAY);
            cvSmooth(cannyFrame_c0, cannyFrame_c0, CV_GAUSSIAN, 15, 7, 100, 0);

            cvCanny(cannyFrame_c0, cannyFrame_c0, tresh_low, tresh_high, 3);
            cvWriteFrame(cannyWriter, cannyFrame_c0);
            cvShowImage("CannyWindow", cannyFrame_c0);
        //}
        char c = cvWaitKey(33);
        if(c == 27) break;
        fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    }
    cvReleaseCapture(&capture);
    cvDestroyAllWindows();
}
void displayTestVideo(char* videoName)
{
    IplImage* frame;
    cvNamedWindow("VideoWindow", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateFileCapture(videoName);

    g_capture = cvCreateFileCapture(videoName);
    int frames = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
    if( frames != 0)
    {
        cvCreateTrackbar("position", "VideoWindow", &g_slider_pos, frames,onTrackbarSlide);
    }
    while(1)
    {
        frame = cvQueryFrame( capture);
        if( !frame ) break;
        cvShowImage( "VideoWindow", frame);
        char c = cvWaitKey(33);
        if (c == 27) break;

    }
    cvReleaseCapture( &capture);
    cvDestroyWindow( "VIdeoWindow");

}

void displayTestImage(char* ImageName)
{
    printf("Opening image.. \n");
    IplImage* img = cvLoadImage( ImageName, 1 );
    printf("Image Opened \n");
    cvNamedWindow( "displayTestImage", CV_WINDOW_AUTOSIZE );
    cvShowImage("displayTestImage", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyWindow("displayTestImage");
}
