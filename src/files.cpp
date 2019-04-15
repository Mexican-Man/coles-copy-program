#include "files.h"
using namespace std;

vector<string> getFiles(const string& dir) {
	vector<string> r;
	try {
		DIR* dirp = opendir(dir.c_str());
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

string getFolderName(const string& dir) {
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

/*int copySystem(const string& transfer.src, const string& transfer.dst, const bool copySystem, unsigned int errCount)
{
	vector<string> list = getFiles(transfer.src);
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
		int status = copyCore(unformatPath(transfer.src + '/' + list[i]), unformatPath(transfer.dst + '/' + list[i]), errCount);
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
}*/
