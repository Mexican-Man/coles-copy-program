#include "dataTransfer.h"
#include "colours.h"
using namespace std;

DataTransfer * transferPtr;

void DataTransfer::setSource(const string& s) {
    src = s;
}

void DataTransfer::setDestination(const string& d) {
    dst = d;
}

void DataTransfer::getSourceSize() {
    system("touch output.txt 2>> /dev/null");
    system(("\"" + root + "usr/bin/du\" -sk \"" + src + "\" >> output.txt 2>> /dev/null").c_str());
    ifstream t("output.txt");
    t >> srcBytes;
    system("rm output.txt 2>> /dev/null");
}

bool DataTransfer::copy() {
    if (filesize) {
        cout << "Determining disk space required...\n";
        getSourceSize();
    }

    transferPtr = this;
    currentFolder = getFolderName(src);
    currentFolderSize = stof(to_string(srcBytes));
    copyfile_state_t state = copyfile_state_alloc();
    uint32_t callback = COPYFILE_STATE_STATUS_CB;
    copyfile_state_set(state, callback, (void * ) & callbackFunction);
    copyfile_flags_t flags = COPYFILE_RECURSIVE | COPYFILE_DATA;
    int result = copyfile(src.c_str(), dst.c_str(), state, flags);
    printStatus(true);
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

bool DataTransfer::copySystem() {
    vector<string> list = getFiles(src);

    for (int i = 0; i < list.size(); i++) {
        if (list[i].find("Applications") != std::string::npos) {} else if (list[i].find("Library") != std::string::npos) {} else if (list[i].find("System") != std::string::npos) {} else if (list[i].find("Users") != std::string::npos) {} else {
            list.erase(list.begin() + i);
            i--;
        }
    }
    bool isError = false;

    for (int i = 0; i < list.size(); i++) {
        src = root + list[i];
        bool status = copy();

        if (!status) {
            isError = true;
        }
    }
    if (isError) {
        return false;
    } else {
        return true;
    }
}

uint32_t callbackFunction(int what, int stage, copyfile_state_t state, const char* src, const char* dst, void* ctx) {
    transferPtr -> dstBytes += transferPtr -> bytesTransferred;
    copyfile_state_get(state, COPYFILE_STATE_COPIED, & transferPtr -> bytesTransferred);

    /*
    if (what == COPYFILE_RECURSE_FILE)
    {
    }
    else if (what == COPYFILE_RECURSE_DIR)
    {

    }
    else if (what == COPYFILE_RECURSE_DIR_CLEANUP)
    {

    }
    else if (what == COPYFILE_RECURSE_ERROR)
    {
    }*/

    if (stage == COPYFILE_START) {
        // Before the copying starts
    } else if (stage == COPYFILE_FINISH && what == COPYFILE_RECURSE_FILE) {
        // After the copying has finished
        transferPtr -> printStatus(false);
    } else if (stage == COPYFILE_ERR) {
        // If it encounters an error
        transferPtr -> errorCount++;
        return COPYFILE_SKIP;
    }

    return COPYFILE_CONTINUE;
}

void DataTransfer::printStatus(bool done) {
    totalByteCount += bytesTransferred;
    cout << fixed << setprecision(2) << "\rCopying " << currentFolder << " ~ " << CYAN << stof(to_string(totalByteCount)) / 1073741824 << RESET << "/" << BLUE;

    if (filesize) {
        cout << currentFolderSize / 1048576;
    } else {
        cout << "unknown";
    }
    
    cout << GREEN << "GB       " << RESET;

    if (done) {
        cout << fixed << setprecision(2) << "\rCopying " << currentFolder << " - " << ITALIC << "Complete            \n" << RESET;
    }
}

const string volumes = "/Volumes/";

bool Session::promptBool(const string& message, const char& rtrue, const char& rfalse) {
    printDelimiter();
    string r = "";
    cout << message;
    getline(cin, r);

    while (!cin.good() || r == "") {
        cout << BOLD RED << "Invalid answer!\n" << RESET;
        cin.clear();
        r = "";
        cout << message;
        getline(cin, r);
    }

    if (tolower(r.at(0)) == rtrue) {
        cout << "\n";
        return true;
    } else if (tolower(r.at(0)) == rfalse) {
        cout << "\n";
        return false;
    } else if (tolower(r.at(0)) == 'q') {
        cout << "\n";
        exit(0);
    } else {
        cout << BOLD RED << "Invalid answer!\n" << RESET;
        promptBool(message, rtrue, rfalse);
    }

    return 0;
}

char Session::promptChar(const string& message, const char r[], const int& s) {
    string a = "";
    cout << message << BOLD GREEN;
    getline(cin, a);
    cout << RESET;

    while (!cin.good() || a == "") {
        cout << BOLD RED << "Invalid answer!\n" << RESET;
        cin.clear();
        a = "";
        cout << message << BOLD GREEN;
        getline(cin, a);
        cout << RESET;
    }

    for (int i = 0; i < s; i++) {
        if (r[i] == tolower(a.at(0))) {
                cout << "\n";
                return a.at(0);
            }
        }

        if (tolower(a.at(0)) == 'q') {
            cout << "\n";
            exit(0);
        } else {
            cout << BOLD RED << "Invalid answer!\n" << RESET;
            promptChar(message, r, s);
        }

    return 0;
}

int Session::promptListConfirm(const vector<string>& list, const string& message) {
    for (int i = 1; i <= list.size(); i++) {
        cout << '\t' << BOLD GREEN << i << RESET << ". " << CYAN << list[i - 1] << RESET << '\n';
    }

    cout << message << BOLD GREEN;
    int r = -1;
    string s = "";
    getline(cin, s);
    cout << RESET;

    if (tolower(s.at(0)) == 'q') {
        cout << "\n";
        exit(0);
    }

    try {
        r = stoi(s);
    } catch (const std::exception & ) {
        r = 0;
    }

    while (!cin.good() || s == "" || r > list.size() || r < 0) {
        cout << BOLD RED << "Invalid answer!\n" << RESET;
        cin.clear();
        cout << message << BOLD GREEN;
        getline(cin, s);
        cout << RESET;
        r = stoi(s);
    }

    cout << "\n";

    if (promptBool("Are you sure you want to pick \"" CYAN + list[r - 1] + RESET "\"? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): " BOLD GREEN, 'y', 'n')) {
        cout << RESET;
        return r - 1;
    } else {
        cout << RESET;
        r = -1;
        return promptListConfirm(list, message);
    }
    return 0;
}

int Session::promptList(const vector<string>& list,const string& message) {
    for (int i = 1; i <= list.size(); i++) {
        cout << '\t' << BOLD GREEN << i << RESET << ". " << CYAN << list[i - 1] << RESET << '\n';
    }

    cout << message << BOLD GREEN;
    int r;
    string s = "";
    getline(cin, s);
    cout << RESET;

    if (tolower(s.at(0)) == 'q') {
        cout << "\n";
        exit(0);
    }

    try {
        r = stoi(s);
    } catch (const std::exception & ) {
        r = 0;
    }

    cout << "\n";

    while (!cin.good() || s == "" || r > list.size() || r < 0) {

        cout << BOLD RED << "Invalid answer!\n" << RESET;
        cin.clear();
        cout << message;
        getline(cin, s);
        r = stoi(s);
    }

    if ((r + 1) <= list.size() && r > 0) {
        cout << "\n\n";
        return r - 1;
    }

    return 0;
}

string Session::pickFolder(const string& dir, const string& message) {
    string r;
    try {
        vector<string> l = getFiles(dir);
        r = dir + l[promptListConfirm(l, message)];
        return r;
    } catch (const std::exception & ) {
        return r;
    }
}

void Session::donePrint(int result, bool isBackup, unsigned int errCount) {
    if (result == 0) {
        if (isBackup) {
            cout << GREEN << "\n================================================================================\n\nBackup completed with " << BOLD << transfer -> errorCount << RESET GREEN << " skipped files, and " << BOLD << stof(to_string(transfer -> totalByteCount)) / 1073741824 << "GB " << RESET GREEN << "transferred!\n\n================================================================================\n\n" << RESET;
        } else {
            cout << GREEN << "\n================================================================================\n\nRestore completed with " << BOLD << transfer -> errorCount << RESET GREEN << " skipped files, and " << BOLD << stof(to_string(transfer -> totalByteCount)) / 1073741824 << "GB " << RESET GREEN << "transferred!\n\n================================================================================\n\n" << RESET << "\n\n";
        }
    } else {
        if (isBackup) {
            cout << RED <<  "\n================================================================================\n\nBackup reported as incomplete, with " << BOLD << transfer -> errorCount << RESET RED << " skipped files, and " << BOLD << stof(to_string(transfer -> totalByteCount)) / 1073741824 << "GB " << RESET RED << "transferred!\n Please make sure all files required have been transferred successfully before proceeding.\n\n================================================================================\n\n" << RESET << "\n\n";
        } else {
            cout << RED << "\n================================================================================\n\nRestore reported as incomplete, with " << BOLD << transfer -> errorCount << RESET RED << " skipped files, and " << BOLD << stof(to_string(transfer -> totalByteCount)) / 1073741824 << "GB " << RESET RED << "transferred!\n Please make sure all files required have been transferred successfully before proceeding.\n\n================================================================================\n\n" << RESET << "\n\n";
        }
    }
}

void Session::printDelimiter() {
    cout << "================================================================================\n\n";
}

void Session::backup(char type) {
    cout << RESET;
    if (type) {
        printDelimiter();
        // Select source DRIVE
        transfer -> root = volumes + getFolderName(pickFolder(volumes, "Please input a number to select a " BOLD BLUE "source" RESET " drive: "));
        printDelimiter();

        // Select user
        transfer -> src = pickFolder(transfer -> root + "Users/", "Please select a user to " BOLD BLUE "backup" RESET ": ");
        string user = transfer -> src.substr(transfer -> root.length() - 1);
        printDelimiter();

        // Select backup drive
        transfer -> dst = pickFolder(volumes, "Please select a backup " BOLD BLUE "destination" RESET ": ") + getTagAndDate();
        transfer -> dst += getFolderName(transfer -> src).substr(0, 1);
        transfer -> dst = transfer -> dst.substr(0, transfer -> dst.length() - 1);
        transfer -> dst += "Users/";
        printDelimiter();

        // Make backup folder
        cout << "Attempting to create backup folder\n\n";
        system(("mkdir -p \"" + transfer -> dst + "\" >> /dev/null 2>> /dev/null").c_str());

        // Get source file size
        transfer -> filesize = promptBool("Would you like you check the file size (this can " BOLD BLUE "significantly" RESET " increase wait time on older systems)? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): ", 'y', 'n');
        printDelimiter();

        // Print visual aid
        cout << BLUE << transfer -> src << GREEN << " -> " << BLUE << transfer -> dst << RESET << "\n\n";
        
        // Begin copy
        cout << BOLD RED << "WARNING: due to file compression, the numbers below will almost never line up!\n\n" << RESET;
        donePrint(transfer -> copy(), true, transfer -> errorCount);
    } else {
        printDelimiter();
        // Select source DRIVE
        transfer -> src = volumes + getFolderName(pickFolder(volumes, "Please input a number to select a " BOLD BLUE "source" RESET " drive: "));
        transfer -> root = transfer -> src;
        printDelimiter();

        // Select backup drive
        transfer -> dst = pickFolder(volumes, "Please select a backup " BOLD BLUE "destination " RESET ": ") + getTagAndDate();
        printDelimiter();

        // Make backup folder
        cout << "Attempting to create backup folder\n\n";
        system(("mkdir -p \"" + transfer -> dst + "\" >> /dev/null 2>> /dev/null").c_str());

        // Get source file size
        transfer -> filesize = promptBool("Would you like you check the file size (this can " BOLD BLUE "significantly" RESET " increase wait time on older systems)? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): ", 'y', 'n');
        printDelimiter();
        
        // Print visual aid
        cout << BLUE << transfer -> src << GREEN << " -> " << BLUE << transfer -> dst << RESET << "\n\n";

        // Begin copy
        cout << BOLD RED << "WARNING: due to file compression, the numbers below will almost never line up!\n\n" << RESET;
        donePrint(transfer -> copySystem(), true, transfer -> errorCount);
    }
}

void Session::restore(bool isUser) {
    cout << RESET;
    if (isUser) {
        printDelimiter();
        // Select source drive
        transfer -> root = pickFolder(volumes, "Please input a number to select which drive to " BOLD BLUE "restore from " RESET ": ");
        printDelimiter();

        // Select source folder
        transfer -> src = pickFolder(transfer -> root + "/", "Please select the corresponding " BOLD BLUE "tag #/folder" RESET " name: ") + "/Users/";
        printDelimiter();

        // Select source user folder
        transfer -> src = pickFolder(transfer -> src, "Please select a user " BOLD BLUE "to restore" RESET ": ") + "/";
        printDelimiter();

        // Select destination drive
        transfer -> dst = volumes + getFolderName(pickFolder(volumes, "Please select a drive to " BOLD BLUE "restore to" RESET " (usually Macintosh HD): "));
        printDelimiter();

        // Select destination user folder
        transfer -> dst = pickFolder(transfer -> dst + "/Users/", "Please select a user " BOLD BLUE "to restore" RESET ": ") + "/";
        printDelimiter();

        // Get source file size
        transfer -> filesize = promptBool("Would you like you check the file size (this can " BOLD BLUE "significantly" RESET " increase wait time on older systems)? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): ", 'y', 'n');
        printDelimiter();

        // Print visual aid
        cout << BLUE << transfer -> src << GREEN << " -> " << BLUE << transfer -> dst << RESET << "\n\n";
        
        // Begin copy
        cout << BOLD RED << "WARNING: due to file compression, the numbers below will almost never line up!\n\n" << RESET;
        donePrint(transfer -> copy(), true, transfer -> errorCount);
    } else {
        printDelimiter();
        // Select source drive
        transfer -> root = pickFolder(volumes, "Please input a number to select which drive to " BOLD BLUE "restore from" RESET ": ") + "/";
        printDelimiter();

        // Select source folder
        transfer -> src = pickFolder(transfer -> root, "Please select the corresponding " BOLD BLUE "tag #/folder" RESET " name: ");
        printDelimiter();

        // Select destination drive
        transfer -> dst = volumes + getFolderName(pickFolder(volumes, "Please select a drive to " BOLD BLUE "restore to" RESET " (usually Macintosh HD): "));

        // Get source file size
        transfer -> filesize = promptBool("Would you like you check the file size (this can " BOLD BLUE "significantly" RESET " increase wait time on older systems)? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): ", 'y', 'n');
        printDelimiter();

        // Print visual aid
        cout << BLUE << transfer -> src << GREEN << " -> " << BLUE << transfer -> dst << RESET << "\n\n";
        
        // Begin copy
        cout << BOLD RED << "WARNING: due to file compression, the numbers below will almost never line up!\n\n" << RESET;
        donePrint(transfer -> copy(), true, transfer -> errorCount);
    }
}

void Session::migrate() {
    cout << RESET;
    
    // Select source drive
    transfer -> src = pickFolder(volumes, "Please input a number to select which drive to " BOLD BLUE "migrate from " RESET ": ") + "/";
    printDelimiter();

    // Select source user folder
    transfer -> src = pickFolder(transfer -> src + "Users/", "Please select a user to " BOLD BLUE "migrate from" RESET ": ") + "/";
    printDelimiter();

    // Select destination drive
    transfer -> dst = volumes + getFolderName(pickFolder("/Volumes/", "Please select a drive to " BOLD BLUE "migrate to" RESET " (usually Macintosh HD):"));
    printDelimiter();

    // Select destination user folder
    transfer -> dst = pickFolder(transfer -> dst, "Please select a user to " BOLD BLUE "migrate to" RESET ": ") + "/";
    printDelimiter();

    // Get source file size
    transfer -> filesize = promptBool("Would you like you check the file size (this can " BOLD BLUE "significantly" RESET " increase wait time on older systems)? (" BOLD GREEN "y" RESET "/" BOLD RED "n" RESET "): ", 'y', 'n');
    printDelimiter();

    // Print visual aid
    cout << BLUE << transfer -> src << GREEN << " -> " << BLUE << transfer -> dst << RESET << "\n\n";
    
    // Begin copy
    cout << BOLD RED << "WARNING: due to file compression, the numbers below will almost never line up!\n\n" << RESET;
    donePrint(transfer -> copy(), false, transfer -> errorCount);
}

void Session::tools(int type) {
    switch (type) {
    case 0:
        // Warning
        cout << BOLD RED << "WARNING: migrating data may take an undetermined amount of time. This tool works best when ran BEFORE a fresh install of macOS!\n\n" << RESET;

        // Create folders on new drive
        cout << "Creating destination folders\n";
        system("mkdir /Volumes/Macintosh\\ HD/Applications > /dev/null 2>> /dev/null");
        system("mkdir /Volumes/Macintosh\\ HD/Users > /dev/null 2>> /dev/null");

        // Migrating data
        cout << "Migrating /Applications/ folder\n";
        //donePrint(copyFiles(checkPath(transfer->src), checkPath(transfer->dst), errors), false, errors);
        system(("mv /Volumes/Macintosh\\ SSD/Applications /Volumes/Macintosh\\ HD/Applications >> /dev/null 2>> /dev/null"));
        cout << "Migrating /Users/ folder\n";
        system(("mv /Volumes/Macintosh\\ SSD/Users /Volumes/Macintosh\\ HD/Users >> /dev/null 2>> /dev/null"));

        // Delete old folders
        cout << "Deleting old folders\n";
        system(("rm -rf /Volumes/Macintosh\\ SSD/Applications trunk >> /dev/null 2>> /dev/null"));
        system(("rm -rf /Volumes/Macintosh\\ SSD/Users trunk >> /dev/null 2>> /dev/null"));
        cout << "Creating symbolic links\n";
        system(("ln -s /Volumes/Macintosh\\ HD/Applications /Volumes/Macintosh\\ SSD/ >> /dev/null 2>> /dev/null"));
        system(("ln -s /Volumes/Macintosh\\ HD/Users /Volumes/Macintosh\\ SSD/ >> /dev/null 2>> /dev/null"));
        donePrint(0, false, 0);
        break;
    }
}

void Session::run(char type) {
    switch (type) {
        default:
            cout << BOLD RED << "Invalid answer!\n" << RESET;
            break;
        case 'b':
            backup(promptBool("Do you want to do a user backup (" BOLD GREEN "u" RESET ") or a full disk copy (" BOLD GREEN "f" RESET "): " BOLD GREEN, 'u', 'f'));
            break;
        case 'r':
            restore(promptBool("Do you want to restore from a user backup (" BOLD GREEN "u" RESET ") or a full disk copy (" BOLD GREEN "f" RESET "): " BOLD GREEN, 'u', 'f'));
            break;
        case 'm':
            cout << BOLD RED << "WARNING: this will only migrate the user. All other applications must be moved manually!\n\n" << RESET;
            migrate();
            break;
        case 't':
            vector < string > list(1);
            list[0] = "Offload Applications and Users from SSD to HD";
            tools(promptList(list, "Please select an option:"));
            break;
    }
}

string Session::getTagAndDate() {
    string r1 = "";
    string r2 = "";
    printDelimiter();
    cout << "Please enter the current " BOLD BLUE "tag #" RESET " (if none is present, use client\'s full name): ";
    getline(cin, r2);
    cout << "\n";
    time_t now = time(0);
    cout << "The current reported date is " << BOLD BLUE << std::put_time(std::localtime( & now), "%m") << "-" << std::put_time(std::localtime( & now), "%d") << "-" << std::put_time(std::localtime( & now), "%Y") << RESET << ". " << BOLD CYAN << "Press enter if this is correct" << RESET << ", or enter the correct date in MM-DD-YYYY format: ";
    getline(cin, r1);
    if (r1 == "") {
        stringstream ss;
        ss << std::put_time(std::localtime( & now), "%m") << "-" << std::put_time(std::localtime( & now), "%d") << "-" << std::put_time(std::localtime( & now), "%Y");
        r1 = ss.str();
    }
    cout << "\n";
    return "/" + r1 + " " + r2 + "/";
}
