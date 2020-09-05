#include "cameraWorkerEyefish.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/video.hpp>
//#include <opencv2/imgcodecs.hpp>

#include <filesystem>

#include <chrono>

#include <thread>

#include <queue>
#include <deque>

#include <iostream>

#include <mutex>
#include <Windows.h>

#include <time.h> 

#define BOOST_USE_WINDOWS_H
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

using namespace cv;

namespace fs = std::filesystem;

//cameraWorkerEyefish::cameraWorkerEyefish()
//{
//    //open_camera();
//}
//
//cameraWorkerEyefish::~cameraWorkerEyefish()
//{
//}

int main(int argc, char* argv[])
{
    fs::path eyefishPath = fs::path(argv[1]) / fs::path("EYEFISH");
    fs::create_directories(eyefishPath);
    fs::path eyefishVideoPath = eyefishPath / fs::path(std::string(argv[2]) + ".avi");

    std::cout << "*****************************" << std::endl;
    std::cout << eyefishVideoPath << std::endl;
    std::cout << "*****************************" << std::endl;

    // Define eyefish camera
    VideoCapture eyefishCamera(0, cv::CAP_DSHOW);
    eyefishCamera.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    //eyefishCamera.set(CAP_PROP_FPS, 30.0);
    //eyefishCamera.set(CAP_PROP_FRAME_HEIGHT, 600);
    //eyefishCamera.set(CAP_PROP_FRAME_WIDTH, 800);
    while (!eyefishCamera.isOpened())
    {
        std::cout << "Eyefish Camera Not Opened" << std::endl;
        fflush(stdout);
    }
    VideoWriter eyefishVideo(eyefishVideoPath.string(),
        VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
        Size(int(eyefishCamera.get(CAP_PROP_FRAME_WIDTH)),
             int(eyefishCamera.get(CAP_PROP_FRAME_HEIGHT))));

    std::cout << "Eyefish Camera Opened" << std::endl;

    // Access shared memory to be read
    shared_memory_object shm(open_only, "is_stop_capture", read_only);
    mapped_region region(shm, read_only);

    // Wait to be unlocked
    std::cout << "Waiting for Mutex to be unlocked" << std::endl;
    HANDLE syncMutexCameraWorkerEyefish = CreateMutex(NULL, false, L"syncMutexCameraWorkerEyefish");
    WaitForSingleObject(syncMutexCameraWorkerEyefish, INFINITE);

    std::cout << "Mutex unlocked" << std::endl;

    int frameCounter = 0;
    while (true)
    {
        std::cout << "Sampling No." << frameCounter++ << " frame counter" << std::endl;

        Mat eyefishImg;

        eyefishCamera >> eyefishImg;
        //imwrite("test" + std::to_string(frameCounter) + ".jpg", eyefishImg);
        eyefishVideo.write(eyefishImg);

        char* mem = static_cast<char*>(region.get_address());
        //bool breakFlag = false;
        for (std::size_t i = 0; i < region.get_size(); ++i)
        {
            //std::cout << *mem << std::endl;
            if (*mem++ != 1)
            {
                eyefishCamera.release();
                eyefishVideo.release();
                ExitProcess(0);
            }
        }
    }

    return 0;
}

