#include "cameraWorker.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/video.hpp>
//#include <opencv2/imgcodecs.hpp>

#include <k4a/k4a.h>
#include <filesystem>

#include <chrono>

#include <thread>

#include <queue>
#include <deque>

#include <iostream>

#include <mutex>
#include <Windows.h>

#define BOOST_USE_WINDOWS_H
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

//using namespace std::chrono;

//using namespace std;
using namespace cv;

namespace fs = std::filesystem;

//cameraWorker::cameraWorker()
//{
//    //open_camera();
//}
//
//cameraWorker::~cameraWorker()
//{
//}

//void cameraWorker::set_categories_path()
//{
//    ;
//}

//void cameraWorker::set_root_path(fs::path path)
//{
//    fs::path depth("DEPTH");
//    fs::path rgb("RGB");
//    fs::path ir("IR");
//    fs::path eyefish("EYEFISH");
//    fs::path night("NIGHT");
//
//    this->depthPath = path / depth;
//    this->rgbPath = path / rgb;
//    this->irPath = path / ir;
//    this->eyefishPath = path / eyefish;
//    this->nightPath = path / night;
//
//    fs::create_directories(depthPath);
//    fs::create_directories(rgbPath);
//    fs::create_directories(irPath);
//    fs::create_directories(eyefishPath);
//    fs::create_directories(nightPath);
//}

//void cameraWorker::start_record()
//{
//    //open_camera();
//    int frame_counter = 0;
//    while (!this->isStop)
//    {
//        capture_images(frame_counter);
//    }
//}

//void cameraWorker::to_stop()
//{
//    this->isStop = true;
//}
//
//void cameraWorker::to_start()
//{
//    this->isStop = false;
//}

//void cameraWorker::start_record()
//{
//
//    // #################### //
//    VideoCapture nightCamera(2);
//    nightCamera.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
//    if (!nightCamera.isOpened())
//    {
//        //fs::create_directories("CameraNotOpened");
//        std::cout << "Camera 1 Not Opened" << std::endl;
//        //info = "Night Camera Not Ready";
//        //exit(-1);
//        while (true) { ; }
//    }
//
//    int nightFrameWidth = nightCamera.get(CAP_PROP_FRAME_WIDTH);
//    int nightFrameHeight = nightCamera.get(CAP_PROP_FRAME_HEIGHT);
//    int codec3 = VideoWriter::fourcc('M', 'J', 'P', 'G');
//
//    fs::path nightVideoBasename("n_out.avi");
//    //fs::path nightVideoName = nightPath / nightVideoBasename;
//
//    //VideoWriter nightVideo(nightVideoName.string(), codec3, 30, Size(nightFrameWidth, nightFrameHeight));
//    VideoWriter nightVideo(nightVideoBasename.string(), codec3, 15, Size(nightFrameWidth, nightFrameHeight));
//
//    // #################### //
//    VideoCapture eyefishCamera(1);
//    eyefishCamera.set(cv::CAP_PROP_FPS, 25.0);
//    eyefishCamera.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
//    if (!eyefishCamera.isOpened())
//    {
//        //fs::create_directories("CameraNotOpened");
//        std::cout << "Camera 2 Not Opened" << std::endl;
//        //info = "Eyefish Camera Not Ready";
//        //exit(-1);
//    }
//    //fps = video.get(cv2.cv.CV_CAP_PROP_FPS)
//
//    //char fileNameCounter2[20];
//
//    int eyefishFrameWidth = eyefishCamera.get(CAP_PROP_FRAME_WIDTH);
//    int eyefishFrameHeight = eyefishCamera.get(CAP_PROP_FRAME_HEIGHT);
//    int codec2 = VideoWriter::fourcc('M', 'J', 'P', 'G');
//
//    fs::path eyefishVideoBasename("e_out.avi");
//    //fs::path eyefishVideoName = eyefishPath / eyefishVideoBasename;
//
//    //VideoWriter eyefishVideo(eyefishVideoName.string(), codec2, 30, Size(eyefishFrameWidth, eyefishFrameHeight));
//    VideoWriter eyefishVideo(eyefishVideoBasename.string(), codec2, 30.0, Size(eyefishFrameWidth, eyefishFrameHeight));
//
//    // #################### //
//    //k4a_capture_t capture;
//
//    //int counter = 0;
//    //for(int count = 0;count<=60;count++)
//    //char cvCounterFileName[20];
//
//
//    //k4a_capture_t capture = NULL;
//    //queue <k4a_capture_t> captureQueue;
//    //vector <k4a_capture_t> captureDeque;
//
//    int total_counter = 200;
//
//    //while (!isStop)
//    std::deque<cv::Mat> nightCameraBuffer;
//    std::deque<cv::Mat> eyefishCameraBuffer;
//    Mat nightImg;
//    Mat eyefishImg;
//    double fps = eyefishCamera.get(cv::CAP_PROP_FPS);
//    std::cout << "FPS is " << fps << std::endl;
//    while (total_counter-- > 0)
//    {
//        //counter++;
//        //Mat nightImg;
//        //Mat eyefishImg;
//        //k4a_capture_t capture = NULL;
//        //qDebug() << QString::fromStdString(to_string(counter));
//        std::cout << std::to_string(total_counter) << std::endl;
//
//        // ###############
//        //Mat nightImg;
//        //sprintf_s(cvCounterFileName, "%.4d.jpg", counter);
//        //fs::path nightImPath = nightPath / fs::path(cvCounterFileName);
//        nightCamera >> nightImg;
//        //nightCameraBuffer.push_front(nightImg);
//
//        nightVideo.write(nightImg);
//        //cv::imwrite(nightImPath.string(), nightImg);
//
//        // ###############
//        //Mat eyefishImg;
//        //sprintf_s(cvCounterFileName, "%.4d.jpg", counter);
//        //fs::path eyefishImPath = eyefishPath / fs::path(cvCounterFileName);
//        eyefishCamera >> eyefishImg;
//        //eyefishCameraBuffer.push_front(eyefishImg.clone());
//        eyefishVideo.write(eyefishImg);
//        //cv::imwrite(eyefishImPath.string(), eyefishImg);
//
//        // ###############
//        //k4a_device_get_capture(azureKinect, &capture, 10);
//
//        //k4a_capture_t queueCapture = capture;
//        //k4a_capture_reference(capture);
//        //captureQueue.push(capture);
//        //captureDeque.emplace_back(capture);
//        //k4a_capture_release(capture);
//        //QThread::msleep(30);
//    }
//
//    int eyefishNumFrames = eyefishCameraBuffer.size();
//    int nightNumFrames = nightCameraBuffer.size();
//
//    //while (eyefishNumFrames-- > 0)
//    //{
//    //    eyefishVideo.write(eyefishCameraBuffer[eyefishNumFrames]);
//    //    //nightVideo.write(nightCameraBuffer[eyefishNumFrames]);
//    //}
//
//    eyefishVideo.release();
//    nightVideo.release();
//    std::cout << "Record Done" << std::endl;
//    //k4a_capture_release(capture);
//
//    //int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
//
//    //fs::path rgbVideoBasename("out.avi");
//    //fs::path rgbVideoName = rgbPath / rgbVideoBasename;
//    //VideoWriter video(rgbVideoName.string(), codec, 15, Size(1280, 720));
//
//    //k4a_image_t image;
//    //int capture_saving_counter = 0;
//    //Mat colorMat;
//    //Mat rgbMat;
//    //Mat depthMat;
//
//    //char rgbCounterFileName[20];
//    //char depthCounterFileName[20];
//    //char irCounterFileName[20];
//
//    //uint8_t* colorBuffer;
//    //uint8_t* depthBuffer;
//
//    //k4a_capture_t targetCapture;
//
//    //int dequeCounter = captureDeque.size();
//    ////while (!captureQueue.empty())
//    //while (dequeCounter-- > 0)
//    //{
//    //    //targetCapture = captureQueue.front();
//    //    //captureQueue.pop();
//    //    capture_saving_counter++;
//
//    //    sprintf_s(rgbCounterFileName, "%.4d.jpg", capture_saving_counter);
//    //    fs::path imFile = rgbPath / fs::path(rgbCounterFileName);
//    //    //qDebug() << QString::fromStdString(to_string(capture_saving_counter));
//    //    //image = k4a_capture_get_color_image(targetCapture);
//    //    image = k4a_capture_get_color_image(captureDeque[dequeCounter]);
//    //    if (image == NULL)
//    //    {
//    //        //qDebug() << QString::fromStdString("NULL image");
//    //        cout << "NULL Image" << endl;
//    //        continue;
//    //    }
//    //    //uint8_t* colorBuffer = k4a_image_get_buffer(image);
//    //    colorBuffer = k4a_image_get_buffer(image);
//    //    colorMat = Mat(this->imHeight, this->imWidth, CV_8UC4, colorBuffer, Mat::AUTO_STEP).clone();
//    //    cvtColor(colorMat, rgbMat, COLOR_BGRA2BGR);
//    //    cv::imwrite(imFile.string(), rgbMat);
//    //    video.write(rgbMat);
//    //    k4a_image_release(image);
//
//    //    sprintf_s(depthCounterFileName, "%.4d.jpg", capture_saving_counter);
//    //    fs::path depthFile = depthPath / fs::path(depthCounterFileName);
//    //    //qDebug() << QString::fromStdString(depthFile.string());
//    //    cout << depthFile.string() << endl;
//    //    //image = k4a_capture_get_depth_image(targetCapture);
//    //    image = k4a_capture_get_depth_image(captureDeque[dequeCounter]);
//    //    if (image == NULL)
//    //    {
//    //        continue;
//    //    }
//    //    //uint8_t* depthBuffer = k4a_image_get_buffer(image);
//    //    depthBuffer = k4a_image_get_buffer(image);
//    //    //uint8_t* depth_buffer = k4a_image_get_format(image);
//    //    depthMat = Mat(k4a_image_get_height_pixels(image), k4a_image_get_width_pixels(image), CV_16U, depthBuffer, Mat::AUTO_STEP).clone();
//    //    cv::imwrite(depthFile.string(), depthMat);
//    //    k4a_image_release(image);
//
//    //    //k4a_capture_release(targetCapture);
//    //    k4a_capture_release(captureDeque[dequeCounter]);
//    //}
//    //video.release();
//    ////qDebug() << QString("Conversion Done");
//    //cout << "Conversion Done" << endl;
//}


int main(int argc, char* argv[])
{
    fs::path nightPath = fs::path(argv[1]) / fs::path("NIGHT");
    fs::create_directories(nightPath);
    fs::path nightVideoPath = nightPath / fs::path(std::string(argv[2]) + ".avi");

    std::cout << "*****************************" << std::endl;
    std::cout << nightVideoPath << std::endl;
    std::cout << "*****************************" << std::endl;

    // Define night camera
    VideoCapture nightCamera(1, cv::CAP_DSHOW);
    //nightCamera.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    while (!nightCamera.isOpened())
    {
        std::cout << "Night Camera Not Opened" << std::endl;
        fflush(stdout);
    }
    VideoWriter nightVideo(nightVideoPath.string(),
        VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
        Size(int(nightCamera.get(CAP_PROP_FRAME_WIDTH)),
             int(nightCamera.get(CAP_PROP_FRAME_HEIGHT))));

    std::cout << "Night camera opened" << std::endl;

    // Access shared memory to be read
    shared_memory_object shm(open_only, "is_stop_capture", read_only);
    mapped_region region(shm, read_only);

    // Wait to be unlocked
    std::cout << "Waiting for Mutex to be unlocked" << std::endl;
    HANDLE syncMutexCameraWorker = CreateMutex(NULL, false, L"syncMutexCameraWorker");
    WaitForSingleObject(syncMutexCameraWorker, INFINITE);
    // Sleep(1000);
    //char* mem = static_cast<char*>(region.get_address());
    std::cout << "Mutex unlocked" << std::endl;

    int frameCounter = 0;
    while (true)
    {
        std::cout << "Sampling No." << frameCounter++ << " frame counter" << std::endl;

        Mat nightImg;

        nightCamera >> nightImg;
        nightVideo.write(nightImg);

        char* mem = static_cast<char*>(region.get_address());
        for (std::size_t i = 0; i < region.get_size(); ++i)
        {
            if (*mem++ != 1)
            {
                nightCamera.release();
                nightVideo.release();
                ExitProcess(0);
            }
        }
    }

    //CloseHandle(syncMutexCameraWorker);

    //cameraWorker camera;
    //camera.start_record();
    //while (true)
    //{
    //    ;
    //}
    //ExitProcess(4);
    //shared_memory_object::remove("is_stop_capture");
    return 0;
}

