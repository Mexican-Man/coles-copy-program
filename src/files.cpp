#include "files.h"
using namespace std;

string formatPath(const string& s)
{
	string str = s;
	string from = " ";
	string to = "\\ ";
	size_t pos = str.find(from);
	int i = 0;

	while (pos != std::string::npos)
	{
		str.replace(pos + i, from.size(), to);
		i++;
		pos = s.find(from, pos + from.size());
	}
	
	if (str.at(0) != '/')
	{
		str.insert(0, "/");
	}

	return str;
}
string unformatPath(const string& s)
{
	string str = s;
	string from = "\\ ";
	string to = " ";
	size_t pos = str.find(from);

	while (pos != std::string::npos)
	{
		str.replace(pos, from.size(), to);
		pos = str.find(from, pos + from.size());
	}

	if (str.at(str.size() - 1) != '/')
	{
		str += "/";
	}

	return str;
}
string checkPath(const string& s)
{
	string str = s;
	string from = "//";
	string to = "/";
	size_t pos = str.find(from);
	int i = 0;

	while (pos != std::string::npos)
	{
		str.replace(pos - i, from.size(), to);
		pos = s.find(from, pos + from.size());
		i++;
	}

	from = "\\\\";
	to = "\\";
	pos = str.find(from);
	i = 0;

	while (pos != std::string::npos)
	{
		str.replace(pos - i, from.size(), to);
		pos = s.find(from, pos + from.size());
		i++;
	}

	if (str.at(str.size() - 1) != '/')
	{
		str += "/";
	}

	return str;
}
string getTagAndDate()
{
	string r1 = "";
	string r2 = "";
	cout << "================================================================================\n\n";
	cout << "Please enter the current tag # (if none is present, use client\'s full name): ";
	getline(cin, r2);
	time_t now = time(0);
	cout << "Please enter the date in MM-DD-YYYY format. The current reported date is " << std::put_time(std::localtime(&now), "%m") << "-" << std::put_time(std::localtime(&now), "%d") << "-" << std::put_time(std::localtime(&now), "%Y") << ": ";
	getline(cin, r1);
	return formatPath("/" + r1 + " " + r2 + "/");
}
vector<string> getFiles(const string& dir)
{
	vector<string> r;
	try {
		DIR* dirp = opendir(unformatPath(dir).c_str());
		struct dirent * dp;
		while (((dp = readdir(dirp)) != NULL)) {
			string line = dp->d_name;
			if (line.at(0) != '.' && strcmp(dp->d_name, "System Volume Information") && line.find(".") == std::string::npos)
			{
				r.push_back(dp->d_name);
			}
		}
		closedir(dirp);
	}
	catch (const std::exception&)
    {
        cout << "Unable to acquire data from file system. Please proceed with the repair manually!\n";
        exit(0);
    }

	return r;
}
string getFolderName(const string& dir)
{
	string newDir = dir;
    
    if (newDir.at(newDir.size() - 1) == '/')
    {
        newDir.pop_back();
    }
    
	size_t pos = newDir.find_last_of("/");
	newDir = newDir.substr(pos);
	newDir += "/";

	if (strlen(newDir.c_str()) > 31) {
		newDir.resize(31);
	}

	return newDir;
}
void printStatus(bool done)
{
    totalByteCount += currentBytes;
	//cout << srcBytes / 1048576 << '\n' << to_string(srcBytes) << '\n' << stof(to_string(srcBytes)) / 1048576 << '\n';
	cout << fixed << setprecision(2) << "\rCopying " << getFolderName(currentSrc) << " ~ " << stof(to_string(totalByteCount)) / 1073741824 << "/" << stof(to_string(srcBytes)) / 1048576 << "GB";

	if (done) {
		cout << fixed << setprecision(2) << "\rCopying " << getFolderName(currentSrc) << " - " << "\033[32mComplete\033[0m                                                \n";
	}
}
uint32_t callbackFunction(int what, int stage, copyfile_state_t state, const char * src, const char * dst, void * ctx)
{
	copyfile_state_get(state, COPYFILE_STATE_COPIED, currentBytesPtr);

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

	if (stage == COPYFILE_START)
	{
		// Before the copying starts
	}
	else if (stage == COPYFILE_FINISH && what == COPYFILE_RECURSE_FILE)
	{
		// After the copying has finished
		printStatus(false);
	}
	else if (stage == COPYFILE_ERR)
	{
		// If it encounters an error
		errorCount++;
		return COPYFILE_SKIP;
	}

	return COPYFILE_CONTINUE;
}
int copyCore(const string& fromDir, const string& toDir, unsigned int errCount)
{
    dstBytes = 0;
    srcBytes = 0;
    currentBytes = 0;
	totalByteCount = 0;
	currentSrc = checkPath(fromDir);
	currentDst = checkPath(toDir);
	errorCount = 0;
    srcBytes = 0;

	system("touch output.txt 2>> /dev/null");
	system(("/Volumes/Macintosh\\ HD/usr/bin/du -sk \"" + currentSrc + "\" >> output.txt 2>> /dev/null").c_str());
	ifstream t("output.txt");
    t >> srcBytes;
    system("rm output.txt 2>> /dev/null");
    
    /*struct stat file_stats;
    DIR *dirp;
    struct dirent *dent;
    dirp = opendir(currentSrc.c_str());
    dent = readdir(dirp);
    srcBytes = file_stats.st_size;
    closedir(dirp);*/
    
    cout << "Beginning copy\n";

    copyfile_state_t state = copyfile_state_alloc();
	uint32_t callback = COPYFILE_STATE_STATUS_CB;
    copyfile_state_set(state, callback, (void*)&callbackFunction);
	copyfile_flags_t flags = COPYFILE_RECURSIVE | COPYFILE_DATA;
	int result = copyfile(fromDir.c_str(), toDir.c_str(), state, flags);
	printStatus(true);
	return result;
}
int copySystem(const string& fromDir, const string& toDir, const bool copySystem, unsigned int errCount)
{
	vector<string> list = getFiles(fromDir);
	system("cd /");
    
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].find("Applications") != std::string::npos)
        {
		}
        else if (list[i].find("Library") != std::string::npos)
        {
        }
        else if (list[i].find("System") != std::string::npos)
        {
        }
        else if (list[i].find("Users") != std::string::npos)
        {
        }
        else
        {
            list.erase(list.begin() + i);
            i--;
        }
        
        if (list[i].find("System") != std::string::npos && !copySystem)
        {
            list.erase(list.begin() + i);
        }
	}
	bool isError = false;
	int errCountTotal = 0;

	for (int i = 0; i < list.size(); i++)
	{
		int status = copyCore(unformatPath(fromDir + '/' + list[i]), unformatPath(toDir + '/' + list[i]), errCount);
		errCountTotal += errCount;

		if (status != 0) {
			isError = true;
		}
	}

	if (isError) {
		return 0;
	}
	else {
		return -1;
	}
}
int copyFiles(const string& fromDir, const string& toDir, int errCount)
{
	system("cd /");
	return copyCore(unformatPath(fromDir), unformatPath(toDir), errCount);
}
