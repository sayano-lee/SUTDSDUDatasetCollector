#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_collectorController.h"
//#include "eyefishWorker.h"
//#include  "cameraWorker.h"

#include <k4a/k4a.h>

#include <string>
#include <filesystem>

#include <thread>
#include <QThread>

#include <Windows.h>

#include <mutex>

//#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

//using namespace std;
namespace fs = std::filesystem;

class collectorController : public QMainWindow
{
    Q_OBJECT

public:
    collectorController(QWidget* parent = Q_NULLPTR);
    ~collectorController();

private slots:

    //void start_capture();
    //void start_capture_2();
    //void stop_capture();
    //void stop();

    void release_mutex();
    void open_capture();
    void stop();

    //fs::path assemble_category_name();
    fs::path get_category_path();

    void start_preview_fisheye_camera();
    void stop_preview_fisheye_camera();

private:
    Ui::collectorControllerClass ui;
    const int32_t TIMEOUT_IN_MS = 1000;
    std::string DIR_TO_CAPTURED_FRAMES = "CAPTURED_FRAMES_DEBUG";

    fs::path globalTimeStampPath;

    int colorFrameCounter = 0;
    int irFrameCounter = 0;
    int depthFrameCounter = 0;

    PROCESS_INFORMATION pi1;
    PROCESS_INFORMATION pi2;
    PROCESS_INFORMATION pi3;

    bool preview_fisheye_stop;

    HANDLE syncMutexCameraWorkerEyefish;
    HANDLE syncMutexCameraWorker;
    HANDLE syncMutexKinect;

    boost::interprocess::mapped_region region;

    void start_capture_kinect(fs::path timeStamp, fs::path categoryStamp);
    void start_capture_night(fs::path timeStamp, fs::path categoryStamp);
    void start_capture_eyefish(fs::path timeStamp, fs::path categoryStamp);

    //QThread eyefishThread;
    //eyefishWorker eyefishWorker;

    //QThread cameraThread;
    //cameraWorker cameraWorker;

    // Help functions
    //void disable_checkbox(QCheckBox* box);
    //void check_and_disable_checkbox(QCheckBox* box);
    std::string get_time_stamp();
    fs::path get_timestamp_path();


    //fs::path assemble_category_name();

    // Capturing functions
    //void capture_kinect_frames(string timeStampDir);
    //std::thread thread_wrapper_capture_kinect_frames(string time_stamp);

    //void capture_eyefish_frames(string timeStampDir);
    //std::thread thread_wrapper_capture_eyefish_frames(string time_stamp);

    //int capture_night_frames(string timeStampDir);
};
