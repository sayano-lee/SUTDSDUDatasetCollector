//#include <QObject>
//#include <QThread>
//#include <QDebug>

#include <string>

#include <k4a/k4a.h>

#include <filesystem>

//using namespace std;
namespace fs = std::filesystem;

class cameraWorker
{
	//Q_OBJECT
public:
	cameraWorker();
	~cameraWorker();
	void start_record();

private:
	k4a_device_t azureKinect;
	std::string info;
	volatile bool isStop = false;

	fs::path eyefishPath;
	fs::path nightPath;

	//void capture_images(int counter);
	//void set_saving_path(std::string path);

};
