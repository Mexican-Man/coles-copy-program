#ifndef COPY
#define COPY

#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <limits>
#include <copyfile.h>
#include <sys/stat.h>
using namespace std;

typedef int
(*copyfile_callback_t)(int what, int stage, copyfile_state_t state, const char * src, const char * dst,
	void * ctx);

std::string currentSrc;
std::string currentDst;
long int dstBytes;
long int srcBytes;
long int* srcBytesPtr;
off_t currentBytes;
off_t* currentBytesPtr = &currentBytes;
long int totalByteCount;
unsigned int errorCount = 0;

// - Prompts the user with 'message' to choose between 'rtrue' and 'rfalse'
bool promptBool(const string& message, const char& rtrue, const char& rfalse);

// - Prompts the user with 'message' to choose between 'r1', 'r2' and 'r3'
char promptChar(const string& message, const char& r1, const char& r2, const char& r3);

// - Prompts the user with 'message' to pick an item in 'list' then to confirm with yes or no
int promptListConfirm(const vector<string>& list, const string& message);

// - Prompts the user with 'message' to pick an item in 'list'
int promptList(const vector<string>& list, const string& message);

// - Prompts the user with 'message' to pick a folder in 'dir'
string pickFolder(const string& dir, const string& message);

// - Used after a transfer has finished, tells user information about transfer (failed, succeeded, error count, etc)
void donePrint(int result, bool isBackup, unsigned int errorCount);

#endif
