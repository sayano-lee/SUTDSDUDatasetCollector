// dummyProc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mutex>
#include <filesystem>

#include <fstream>

#include <Windows.h>

#define BOOST_USE_WINDOWS_H
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;
namespace fs = std::filesystem;

//#include <Windows.h>

PROCESS_INFORMATION start_record(fs::path videoPath)
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	//DWORD returnCode;

	//std::string strTimeStamp = ' ' + timeStamp.string();
	//std::string strCategoryStamp = ' ' + categoryStamp.string();

	std::string strVideoPath = ' ' + videoPath.string();

	//wchar_t command[] = L"dummyProc.exe";
	std::wstring wide_command = L"k4arecorder.exe";
	std::wstring wide_videoPath = std::wstring(strVideoPath.begin(), strVideoPath.end());
	//std::wstring wide_categoryStamp = std::wstring(strCategoryStamp.begin(), strCategoryStamp.end());
	std::wstring wide_commandline = wide_command + L' ' + wide_videoPath;
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
		&pi);

	return pi;
};

int main(int argc, char* argv[])
{
	//std::string timeStamp = argv[1];
	//std::string categoryStamp = argv[2];

	fs::path timeStamp = fs::path(argv[1]);
	fs::path categoryStamp = fs::path(argv[2]);

	fs::path kinectPath = timeStamp / fs::path("KINECT");
	fs::create_directories(kinectPath);

	std::string mkvPath = (kinectPath / fs::path(categoryStamp)).string() + ".mkv";
	std::string txtPath = (kinectPath / fs::path(categoryStamp)).string() + ".txt";

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	std::string strVideoPath = ' ' + mkvPath;

	std::wstring wide_command = L"k4arecorder.exe";
	std::wstring wide_videoPath = std::wstring(strVideoPath.begin(), strVideoPath.end());
	std::wstring wide_commandline = wide_command + L' ' + L"-l 10 " + wide_videoPath;

	wchar_t* command = const_cast<wchar_t*>(wide_commandline.c_str());

	bool break_flag = false;

	// Access shared memory to be read
	shared_memory_object shm(open_only, "is_stop_capture", read_only);
	mapped_region region(shm, read_only);

	// Start Clock Tik
	//std::clock_t start, end;
	double start, end;
	//std::time_t start, end;

	// Wait to be unlocked
	std::cout << "Waiting for Mutex to be unlocked" << std::endl;
	HANDLE syncMutexKinect = CreateMutex(NULL, false, L"syncMutexKinect");
	WaitForSingleObject(syncMutexKinect, INFINITE);
	// Sleep(1000);
	std::cout << "Mutex unlocked" << std::endl;
	start = clock();
	//start = time(&start);

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
		&pi);

	while (true)
	{
		char* mem = static_cast<char*>(region.get_address());
		for (std::size_t i = 0; i < region.get_size(); ++i)
		{
			//std::cout << "Print from dummyProc..." << std::endl;
			if (*mem++ != 1)
			{
				//time(&end);
				end = clock();
				//std::cout << "Print from IN dummyProc..." << std::endl;
				//break_flag = true;
				//AttachConsole(pi.dwProcessId);
				//while (true)
				//{
				//	std::cout << int(pi.dwProcessId) << std::endl;
				//}
				//SetConsoleCtrlHandler(NULL, TRUE);
				//GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
				//FreeConsole();
				//std::cout << "2 Exits line 104" << std::endl;
				std::ofstream outfile(txtPath);
				//outfile << difftime(start, end); 
				double time_diff = (end - start) / CLOCKS_PER_SEC;
				outfile << time_diff;
				//outfile << double (end - start) / CLOCKS_PER_SEC;
				outfile.close();
				//std::cout << end - start << std::endl;
				//while (true){;}
				ExitProcess(0);
			}
		}
	}

	CloseHandle(syncMutexKinect);
	//ExitProcess(0);
	shared_memory_object::remove("is_stop_capture");
	//std::cout << "Programm Exits line 117" << std::endl;
	return 0;
}
