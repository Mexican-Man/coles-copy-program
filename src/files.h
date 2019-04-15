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
#include "dataTransfer.h"
using namespace std;

// - Prompts the user, then creates an appropriate folder name for a backup
string getTagAndDate();

// - Creates a vector containing all the folders in 'dir'
vector<string> getFiles(const string& dir);

// - Trims a file path and returns only the folder name
string getFolderName(const string& dir);

// - Asks the user if they want to restart
inline bool promptFinished(){
    string r = "";
    cout << "Do you want to do another transfer? (y/n) ";
    getline(cin, r);
    
    while (!cin.good() || r == "") {
        cout << "Invalid answer!\n\n";
        cin.clear();
        r = "";
        cout << "Do you want to do another transfer? (y/n) ";
        getline(cin, r);
    }
    
    if (tolower(r.at(0)) == 'y')
    {
        cout << "\n";
        return true;
    }
    
    else if (tolower(r.at(0)) == 'n')
    {
        cout << "\n";
        return false;
    }
    
    else if (tolower(r.at(0)) == 'q')
    {
        cout << "\n";
        exit(0);
    }
    
    else {
        cout << "Invalid answer!\n";
        promptFinished();
    }
    
    return 0;
}


#endif
