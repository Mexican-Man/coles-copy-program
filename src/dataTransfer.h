#ifndef DATATRANSFER
#define DATATRANSFER

#include <vector>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <copyfile.h>
#include "files.h"
using namespace std;

typedef int
(*copyfile_callback_t)(int what, int stage, copyfile_state_t state, const char * src, const char * dst,
	void * ctx);

class DataTransfer {
public:
    std::string root;
    std::string src;
    std::string dst;
    std::string currentFolder;
    float currentFolderSize;
    long int srcBytes;
    long int dstBytes;
    off_t bytesTransferred;
    long int totalByteCount;
    unsigned int errorCount;
    bool done;
    bool filesize;
    
    DataTransfer() {
        src = "";
        dst = "";
        root = "";
        currentFolder = "";
        currentFolderSize = 0;
        srcBytes = 0;
        dstBytes = 0;
        bytesTransferred = 0;
        totalByteCount = 0;
        errorCount = 0;
        done = false;
    }
    
    // - Set source path
    void setSource(const string& s);
    
    // - Set destination path
    void setDestination(const string& d);
    
    // - Get file size of destination
    void getSourceSize();
    
    // - Main function for copying files via Apple's COPYFILE library
    bool copy();

    // - Initiate copy on all important system folders
    bool copySystem();
    
    // - Callback for printing the details of a transfer in progress
    void printStatus(bool done);
};

// - Callback function to deal with Apple's COPYFILE library
uint32_t callbackFunction(int what, int stage, copyfile_state_t state, const char * src, const char * dst, void * ctx);

class Session {
public:
    DataTransfer* transfer;
    
    Session() {
        transfer = nullptr;
    }
    
    ~Session() {
        delete transfer;
    }
    
    // - Prompts the user with 'message' to choose between 'rtrue' and 'rfalse'
    bool promptBool(const string& message, const char& rtrue, const char& rfalse);
    
    // - Prompts the user with 'message' to choose between an array 'r' of characters
    char promptChar(const string& message, const char* r, const int& s);
    
    // - Prompts the user with 'message' to pick an item in 'list' then to confirm with yes or no
    int promptListConfirm(const vector<string>& list, const string& message);
    
    // - Prompts the user with 'message' to pick an item in 'list'
    int promptList(const vector<string>& list, const string& message);
    
    // - Prompts the user with 'message' to pick a folder in 'dir'
    string pickFolder(const string& dir, const string& message);
    
    // Prints summary for end of transfer
    void donePrint(int result, bool isBackup, unsigned int errCount);
    
    // Prints a line delimiter thing
    void printDelimiter();
    
    // Prompts the user what type of backup they want to do
    void backup(char type);
    
    // Prompts the user what type of restore they want to do
    void restore(bool isUser);

    // Initiates a migration from an old drive
    void migrate();
    
    // Other misc tools
    void tools(int type);
    
    // Main menu
    void run(char type);
    
    string getTagAndDate();
};

#endif
