#ifndef FILES
#define FILES

#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <limits>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <copyfile.h>
using namespace std;

extern std::string currentSrc;
extern std::string currentDst;
extern long int srcBytes;
extern long int dstBytes;
extern off_t currentBytes;
extern off_t* currentBytesPtr;
extern long int totalByteCount;
extern unsigned int errorCount;

// - Adds backslash before spaces in 's' order to be compatible with Mac OS terminal
string formatPath(const string& s);

// - Removes backslash before spaces in 's' order to be compatible with native functions
string unformatPath(const string& s);

// - Makes sure a path is a valid path
string checkPath(const string& s);

// - Prompts the user, then creates an appropriate folder name for a backup
string getTagAndDate();

// - Creates a vector containing all the folders in 'dir'
vector<string> getFiles(const string& dir);

// - Trims a file path and returns only the folder name
string getFolderName(const string& dir);

// - Used to print progress of file transfer, meant for use in callback function 'callbackFunction'
void printStatus(bool done);

// - Callback function to deal with Apple's COPYFILE library
uint32_t callbackFunction(int what, int stage, copyfile_state_t state, const char * src, const char * dst, void * ctx);

// - Main function for copying files via Apple's COPYFILE library
int copyCore(const string& fromDir, const string& toDir, unsigned int errCount);

// - Copies all files in a 'dir' except for 'except'
int copySystem(const string& fromDir, const string& toDir, const bool copySystem, unsigned int errCount);

// - Copies all files in a 'dir'
int copyFiles(const string& fromDir, const string& toDir, int errCount);

#endif
