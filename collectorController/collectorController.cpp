#include "collectorController.h"
//#include "cameras.h"
//#include "eyefishWorker.h"

#include <k4a/k4a.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <QThread>
#include <QDebug>

#include <filesystem>

#include <qmessagebox.h>

#include <Windows.h>

#include <mutex>

//using namespace std;
using namespace cv;
namespace fs = std::filesystem;
using namespace boost::interprocess;

//template <typename T>
//T fill_cast(int v, int width, char c)
//{
//    T result;
//    std::stringstream inter;
//    inter << std::setw(width) << std::setfill(c) << v;
//    inter >> result;
//    return result;
//}


std::string collectorController::get_time_stamp()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buf[100] = { 0 };
    std::strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", std::localtime(&now));
    return std::string(buf);
};

// Category Name is P(XX)S(XX)G(XX)B(X)H(X)UC(XX)LC(XX)A(XXX)
// P: Unique ID | S: Setup ID | G: Gender | B: Backpack Color
// UC: Upper clothing color/style | LC: Lower clothing color/style | A: Activity Class
fs::path collectorController::get_category_path()
{
    // full length filename, if errors encountered, CHECK THIS!!!
    char filename[80];

    // Identities
    int uniqueID = ui.uniqueIDSpinBox->value();
    int setupID = ui.setupIDSpinBox->value();
    int replication = ui.replicationSpinBox->value();

    // Attributes
    int gender = ui.genderComboBox->currentText().split(' ').takeLast().toInt();
    int backpack = ui.backpackComboBox->currentText().split(' ').takeLast().toInt();
    int hat = ui.hatComboBox->currentText().split(' ').takeLast().toInt();
    int upperClothingColor = ui.upperClothingColorComboBox->currentText().split(' ').takeLast().toInt();
    int upperClothingStyle = ui.upperClothingStyleComboBox->currentText().split(' ').takeLast().toInt();
    int lowerClothingColor = ui.lowerClothingColorComboBox->currentText().split(' ').takeLast().toInt();
    int lowerClothingStyle = ui.lowerClothingStyleComboBox->currentText().split(' ').takeLast().toInt();

    // Attributes 2
    int gender_2 = ui.genderComboBox_2->currentText().split(' ').takeLast().toInt();
    int backpack_2 = ui.backpackComboBox_2->currentText().split(' ').takeLast().toInt();
    int hat_2 = ui.hatComboBox_2->currentText().split(' ').takeLast().toInt();
    int upperClothingColor_2 = ui.upperClothingColorComboBox_2->currentText().split(' ').takeLast().toInt();
    int upperClothingStyle_2 = ui.upperClothingStyleComboBox_2->currentText().split(' ').takeLast().toInt();
    int lowerClothingColor_2 = ui.lowerClothingColorComboBox_2->currentText().split(' ').takeLast().toInt();
    int lowerClothingStyle_2 = ui.lowerClothingStyleComboBox_2->currentText().split(' ').takeLast().toInt();

    // Activity ID
    int activityClass = -1;
    if (ui.singlePersonRadio->isChecked())
    {
        activityClass = ui.singlePersonAction->currentText().split(' ').takeLast().toInt();
    }
    else if (ui.doublePersonsRadio->isChecked())
    {
        activityClass = ui.doublePersonsAction->currentText().split(' ').takeLast().toInt();
    }
    else if (ui.droneRadio->isChecked())
    {
        activityClass = ui.droneAction->currentText().split(' ').takeLast().toInt();
    }
    //string activityClass = ui.
    //sprintf(filename, );
    //qDebug() << QString::fromStdString(this->assemble_category_name().string());
    sprintf(filename, "P%.2dS%.2dG%.2d%.2dB%.2d%.2dH%.2d%.2dUC%.2d%.2d%.2d%.2dLC%.2d%.2d%.2d%.2dA%.3d_v%.2d",
        uniqueID, setupID, 
        gender, gender_2, backpack, backpack_2, hat, hat_2, 
        upperClothingColor, upperClothingColor_2, upperClothingStyle, upperClothingStyle_2,
        lowerClothingColor, lowerClothingColor_2, lowerClothingStyle, lowerClothingStyle_2,
        activityClass, replication);
    //fs::path name(filename);
    fs::path filePath = filename;
    //qDebug() << QString::fromStdString(filePath.string());
    ui.statusBar->showMessage(QString::fromStdString(filePath.string()));
    return filePath;
};

fs::path collectorController::get_timestamp_path()
{
    std::string time = this->get_time_stamp();
    ui.statusBar->showMessage(QString::fromStdString(time), 5000);
    fs::path dirRoot(DIR_TO_CAPTURED_FRAMES);
    fs::path dirTimestamp(time);
    fs::path timeStampPath = dirRoot / dirTimestamp;
    //bool retCodeCreateDir = fs::create_directories(timeStampPath);
    return timeStampPath;
};


void collectorController::start_capture_eyefish(fs::path timeStamp, fs::path categoryStamp)
{
    STARTUPINFO si = { sizeof(STARTUPINFO) };

    std::string strTimeStamp = ' ' + timeStamp.string();
    std::string strCategoryStamp = ' ' + categoryStamp.string();

    std::wstring wide_command = L"cameraWorkerEyefish.exe";
    std::wstring wide_timeStamp = std::wstring(strTimeStamp.begin(), strTimeStamp.end());
    std::wstring wide_categoryStamp = std::wstring(strCategoryStamp.begin(), strCategoryStamp.end());
    std::wstring wide_commandline = wide_command + L' ' + wide_timeStamp + L' ' + wide_categoryStamp;

    wchar_t* command = const_cast<wchar_t*>(wide_commandline.c_str());
    std::cout << command << std::endl;

    bool cProc = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        false,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &this->pi3);
};

void collectorController::start_capture_kinect(fs::path timeStamp, fs::path categoryStamp)
{
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    //DWORD returnCode;

    std::string strTimeStamp = ' ' + timeStamp.string();
    std::string strCategoryStamp = ' ' + categoryStamp.string();

    //wchar_t command[] = L"dummyProc.exe";
    std::wstring wide_command = L"dummyProc.exe";
    std::wstring wide_timeStamp = std::wstring(strTimeStamp.begin(), strTimeStamp.end());
    std::wstring wide_categoryStamp = std::wstring(strCategoryStamp.begin(), strCategoryStamp.end());
    std::wstring wide_commandline = wide_command + L' ' + wide_timeStamp + L' ' + wide_categoryStamp;
    //std::cout << wide_timeStamp.c_str() << endl;
    //wcscat(command, wide_timeStamp.c_str());
    //wcscat(command, wide_categoryStamp.c_str());
    //std::cout << command << endl;
    //std::to_wstring()

    //const wchar_t* command = wide_commandline.c_str();
    wchar_t* command = const_cast<wchar_t*>(wide_commandline.c_str());

    bool cProc = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        false,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &this->pi2);
};

void collectorController::start_capture_night(fs::path timeStamp, fs::path categoryStamp)
{
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    //DWORD returnCode;

    std::string strTimeStamp = ' ' + timeStamp.string();
    std::string strCategoryStamp = ' ' + categoryStamp.string();

    //wchar_t command[] = L"dummyProc.exe";
    std::wstring wide_command = L"cameraWorker.exe";
    std::wstring wide_timeStamp = std::wstring(strTimeStamp.begin(), strTimeStamp.end());
    std::wstring wide_categoryStamp = std::wstring(strCategoryStamp.begin(), strCategoryStamp.end());
    std::wstring wide_commandline = wide_command + L' ' + wide_timeStamp + L' ' + wide_categoryStamp;
    //std::cout << wide_timeStamp.c_str() << endl;
    //wcscat(command, wide_timeStamp.c_str());
    //wcscat(command, wide_categoryStamp.c_str());
    //std::cout << command << endl;
    //std::to_wstring()

    //const wchar_t* command = wide_commandline.c_str();
    wchar_t* command = const_cast<wchar_t*>(wide_commandline.c_str());
    std::cout << command << std::endl;

    bool cProc = CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        false,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &this->pi1);

};

void collectorController::release_mutex()
{
    //DWORD returnCode;
    ReleaseMutex(syncMutexCameraWorkerEyefish);
    ReleaseMutex(syncMutexCameraWorker);
    ReleaseMutex(syncMutexKinect);
    //syncMutexCameraWorker->unlock();
    //ReleaseMutex(syncMutexKinect);
    //WaitForSingleObject(this->pi1.hProcess, INFINITE);
    //GetExitCodeProcess(this->pi1.hProcess, &returnCode);
    //std::cout << "return code is " << returnCode << std::endl;
}

void collectorController::open_capture()
{
    //fs::path timeStamp = get_timestamp_path();
    fs::path categoryStamp = get_category_path();

    syncMutexCameraWorker = CreateMutex(NULL, true, L"syncMutexCameraWorker");
    WaitForSingleObject(syncMutexCameraWorker, INFINITE);

    syncMutexKinect = CreateMutex(NULL, true, L"syncMutexKinect");
    WaitForSingleObject(syncMutexKinect, INFINITE);

    syncMutexCameraWorkerEyefish = CreateMutex(NULL, true, L"syncMutexCameraWorkerEyefish");
    WaitForSingleObject(syncMutexCameraWorkerEyefish, INFINITE);

    start_capture_kinect(this->globalTimeStampPath, categoryStamp);
    start_capture_eyefish(this->globalTimeStampPath, categoryStamp);
    start_capture_night(this->globalTimeStampPath, categoryStamp);

    std::memset(this->region.get_address(), 1, this->region.get_size());
}

void collectorController::stop()
{
    std::memset(this->region.get_address(), 0, this->region.get_size());
    //ui.setupIDSpinBox->setValue(0);
    ui.replicationSpinBox->setValue(0);
    //Sleep(1000);
    TerminateProcess(this->pi1.hProcess, 4);
    TerminateProcess(this->pi2.hProcess, 4);

    //CloseHandle(this->pi1.hThread);
    //CloseHandle(this->pi1.hProcess);

    //CloseHandle(this->pi2.hThread);
    //CloseHandle(this->pi2.hProcess);
    //OpenMutex()
    //ExitProcess()

}

//void collectorController::stop_capture()
//{
//    //eyefishWorker.to_stop();
//    cameraWorker.to_stop();
//    //eyefishThread.quit();
//    //eyefishThread.wait();
//
//    //cameraWorker.to_stop();
//    //cameraThread.quit();
//    //cameraThread.wait();
//}
//
//void collectorController::stop()
//{
//    cameraThread.quit();
//    cameraThread.wait();
//    //cameraThread.deleteLater();
//    ui.setupIDSpinBox->setValue(0);
//    ui.replicationSpinBox->setValue(0);
//}

void collectorController::start_preview_fisheye_camera()
{
    preview_fisheye_stop = false;
    VideoCapture cap(1, cv::CAP_DSHOW);
    while (!preview_fisheye_stop)
    {
        Mat tmp;
        cap >> tmp;
        QPixmap im = QPixmap::fromImage(QImage((unsigned char*)tmp.data, tmp.cols, tmp.rows, QImage::Format_RGB888));
        ui.preImage->setPixmap(im);
    }
}

void collectorController::stop_preview_fisheye_camera()
{
    preview_fisheye_stop = true;
}

collectorController::~collectorController()
{
    //CloseHandle(this->pi1.hThread);
    //CloseHandle(this->pi1.hProcess);

    //CloseHandle(this->pi2.hThread);
    //CloseHandle(this->pi2.hProcess);
    shared_memory_object::remove("is_stop_capture");
    TerminateProcess(this->pi1.hProcess, 4);
    TerminateProcess(this->pi2.hProcess, 4);
    CloseHandle(this->pi1.hThread);
    CloseHandle(this->pi1.hProcess);

    CloseHandle(this->pi2.hThread);
    CloseHandle(this->pi2.hProcess);

    //shared_memory_object::remove("is_stop_capture");
}

collectorController::collectorController(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //qDebug() << QString("main thread id:") << QThread::currentThreadId();
    globalTimeStampPath = get_timestamp_path();
    //eyefishWorker.set_root_path(timeStampPath);

    //syncMutexCameraWorker = CreateMutex(NULL, true, L"syncMutexCameraWorker");
    //WaitForSingleObject(syncMutexCameraWorker, INFINITE);

    //syncMutexKinect = CreateMutex(NULL, true, L"syncMutexKinect");
    //WaitForSingleObject(syncMutexKinect, INFINITE);

    //shared_memory_object::remove("is_stop_capture");

    shared_memory_object shm(create_only, "is_stop_capture", read_write);
    shm.truncate(128);
    this->region = mapped_region(shm, read_write);
    //shared_memory_object::remove("is_stop_capture");

    //QPixmap im = QPixmap("timg.jpg");
    //this->ui.preImage->setPixmap(im);

    //cv::VideoCapture cap(2, cv::CAP_DSHOW);
    //cv::Mat tmp;
    //cap >> tmp;

    //QPixmap im = QPixmap::fromImage(QImage((unsigned char*)tmp.data, tmp.cols, tmp.rows, QImage::Format_RGB888));

    //QPixmap im = QPixmap("./time.jpg");
    //this->ui.preImage->setPixmap(im);
    //cap.release();

    //this->ui.preImage->setText("Test Image placeholder");
    //mapped_region region2(shm, read_write);

    //start();

    //cameraWorker.set_root_path(timeStampPath);

    //eyefishWorker.moveToThread(&eyefishThread);
    //QObject::connect(&eyefishThread, SIGNAL(started()), &eyefishWorker, SLOT(start_record()));

    //cameraWorker.moveToThread(&cameraThread);
    //qDebug() << ui.genderComboBox->currentText().split(' ').takeLast();
    //fs::path filePath = assemble_category_name();
    //qDebug() << QString::fromStdString(filePath.string());

    //cv::setBreakOnError(true);

    //qDebug() << QString::fromStdString(to_string(typeid(ui.genderComboBox->currentText().split(' '))));
    //QObject::connect(ui.debug, SIGNAL(clicked()), this, SLOT(get_category_path()));
    //QObject::connect(&cameraThread, SIGNAL(started()), &cameraWorker, SLOT(start_record()));

    //start_capture();
    //start_capture_2();
    //Console.Write
    collectorController::connect(ui.captureStart, SIGNAL(clicked()), this, SLOT(open_capture()));
    collectorController::connect(ui.recordStart, SIGNAL(clicked()), this, SLOT(release_mutex()));
    collectorController::connect(ui.recordStop, SIGNAL(clicked()), this, SLOT(stop()));

    collectorController::connect(ui.debug, SIGNAL(clicked()), this, SLOT(get_category_path()));

    collectorController::connect(ui.startFisheyePreview, SIGNAL(clicked()), this, SLOT(start_preview_fisheye_camera()));
    collectorController::connect(ui.stopFisheyePreview, SIGNAL(clicked()), this, SLOT(stop_preview_fisheye_camera()));

    //WaitForSingleObject(this-)

    //collectorController::connect(ui.captureStart, SIGNAL(clicked()), this, SLOT(start_capture()));
    //collectorController::connect(ui.captureStart, SIGNAL(clicked()), this, SLOT(start_capture_2()));
    //collectorController::connect(ui.captureStop, SIGNAL(clicked()), this, SLOT(stop_capture()));
    //collectorController::connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(stop()));


    //ExitProcess();

    //CloseHandle(this->pi1.hThread);
    //CloseHandle(this->pi1.hProcess);

    //CloseHandle(this->pi2.hThread);
    //CloseHandle(this->pi2.hProcess);
};
