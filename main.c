#include <stdio.h>
#include <stdlib.h>
//#include <cv.h>
//
//int main()
//{
//    printf("Hello world!\n");
//    return 0;
//}
#include <cv.h>
#include <highgui.h>

int             g_slider_pos = 0;
CvCapture*      g_capture    = NULL;
void onTrackbarSlide(int pos)
{
    cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);
}


int main(int argc, char** argv )
{
    int c;
    printf("Choose which camera you want to see: \n");
    c = (getchar() - 49);
    displayCameraInput(c);

    return 0;
}
/*
// TODO (Everhard#1#08/05/14): @todo:  ...
// Pointers maken van functies, waardoor ze vanuit console aangeroepen kunnen worden
//Dit zorgt ervoor dat er geen switch statements nodig zijn.

Description:
Function that can read an image stream from a camera, selected by the user.
It will also write to file "camera capture.mpeg" in the Release-folder.
*/

void displayCameraInput(int camera_id)
{
    int frameCounter, fps;
    //Image variable to store frame
    IplImage* cappedFrame;
    //Window to show video feed
    cvNamedWindow("CameraWindow", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCaptureFromCAM(camera_id);

    printf("Capturing from camera %i at %i frames per second \n", camera_id, fps);

    CvSize size = cvSize((int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),
                         (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));
    CvVideoWriter *writer = cvCreateVideoWriter("Camera Capture.mpeg", CV_FOURCC('M','P','E','G'), 16, size, 1);

    if(capture == NULL)
    {
        printf("Camera was not found.\n");
    }
    //Hieronder wat tests om te kijken of 'alles' goed gaat,
    //lees wat eigenschappen van de stream uit.
    for(frameCounter = 0;frameCounter < 200;frameCounter++)
    {
        cappedFrame = cvQueryFrame(capture);
        if(cappedFrame == NULL)
        {
            printf("Frame is null, breaking. \n");
            break;
        }
        cvShowImage("CameraWindow", cappedFrame);
        cvWriteFrame(writer, cappedFrame);
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
