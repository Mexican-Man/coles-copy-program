#include "copy.h"
#include "files.h"
using namespace std;

const string volumes = "/Volumes/";

bool promptBool(const string& message, const char& rtrue, const char& rfalse)
{
	string r = "";
	cout << message;
	getline(cin, r);

	while (!cin.good() || r == "") {
		cout << "Invalid answer!\n\n";
		cin.clear();
		r = "";
		cout << message;
		getline(cin, r);
	}

	if (tolower(r.at(0)) == rtrue)
	{
		cout << "\n";
		return true;
	}

	else if (tolower(r.at(0)) == rfalse)
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
		promptBool(message, rtrue, rfalse);
	}

	return 0;
}
char promptChar(const string& message, const char& r1, const char& r2, const char& r3)
{
	string r = "";
	cout << message;
	getline(cin, r);

	while (!cin.good() || r == "") {
		cout << "Invalid answer!\n\n";
		cin.clear();
		r = "";
		cout << message;
		getline(cin, r);
	}

	if (tolower(r.at(0)) == r1)
	{
		cout << "\n";
		return r1;
	}

	else if (tolower(r.at(0)) == r2)
	{
		cout << "\n";
		return r2;
	}

	else if (tolower(r.at(0)) == r3)
	{
		cout << "\n";
		return r3;
	}

	else if (tolower(r.at(0)) == 'q')
	{
		cout << "\n";
		exit(0);
	}

	else {
		cout << "Invalid answer!\n";
		promptChar(message, r1, r2, r3);
	}

	return 0;
}
int promptListConfirm(const vector<string>& list, const string& message)
{
	for (int i = 1; i <= list.size(); i++)
	{
		cout << '\t' << i << ". " << unformatPath(list[i-1]) << '\n';
	}

	cout << message;
	int r = -1;
    string s = "";
	getline(cin, s);

	if (tolower(s.at(0)) == 'q')
	{
		cout << "\n";
		exit(0);
	}

	try
	{
		r = stoi(s);
	}
	catch (const std::exception&)
	{
		r = 0;
	}

	while (!cin.good() || s == "" || r > list.size() || r < 0)
	{
		cout << "Invalid answer!\n\n";
		cin.clear();
		cout << message;
		getline(cin, s);
		r = stoi(s);
	}

	if (promptBool("Are you sure you want to pick \"" + list[r - 1] + "\"? (y/n) ", 'y', 'n'))
	{
		cout << "\n\n";
		return r - 1;
	}

	else 
	{
        r = -1;
		return promptListConfirm(list, message);
	}

	return 0;
}
int promptList(const vector<string>& list, const string& message)
{
	for (int i = 1; i <= list.size(); i++)
	{
		cout << '\t' << i << ". " << list[i - 1] << '\n';
	}

	cout << message;
	int r;
	string s = "";
	getline(cin, s);

	if (tolower(s.at(0)) == 'q')
	{
		cout << "\n";
		exit(0);
	}

	try
	{
		r = stoi(s);
	}
	catch (const std::exception&)
	{
		r = 0;
	}

	while (!cin.good() || s == "" || r > list.size() || r < 0) {

		cout << "Invalid answer!\n\n";
		cin.clear();
		cout << message;
		getline(cin, s);
		r = stoi(s);
	}

	if ((r + 1) <= list.size() && r > 0)
	{
		cout << "\n\n";
		return r - 1;
	}

	return 0;
}
string pickFolder(const string& dir, const string& message)
{
    try
    {
        vector<string> l = getFiles(dir);
        string r = formatPath(dir + l[promptListConfirm(l, message)]);
        return r;
    }
    catch (const std::exception&)
    {
        cout << "Error reading folders from the file system. Please report this error, then proceed with the repair manually!";
        exit(0);
    }
}
void donePrint(int result, bool isBackup, unsigned int errCount)
{
	if (result == 0)
	{
		if (isBackup)
		{
			cout << "\n\033[32m================================================================================\n\nBackup complete with " << errorCount << " skipped files!\n\n================================================================================\n\n\033[0m\n\n";
		}
		else
		{
			cout << "\n\033[32m================================================================================\n\nRestore complete with " << errorCount << " skipped files!\n\n================================================================================\n\n\033[0m\n\n";
		}
	}
	else
	{
		if (isBackup)
		{
			cout << "\n\033[31m================================================================================\n\nBackup reported as incomplete! Please make sure all files required have been transferred successfully before proceeding.\n\n================================================================================\n\n\033[0m\n\n";
		}
		else
		{
			cout << "\n\033[31m================================================================================\n\nRestore reported as incomplete! Please make sure all files required have been transferred successfully before proceeding.\n\n================================================================================\n\n\033[0m\n\n";
		}
	}
}
void backup(char type)
{
	if (type)
	{
        string orig = volumes + "Macintosh HD/Users/";
		cout << "================================================================================\n\n";
		string fromDir = pickFolder(volumes + "Macintosh HD/Users/", "Please input a number to select a user to backup: ");
        string user = fromDir.substr(orig.length());
		cout << "================================================================================\n\n";
		string toDir = pickFolder(volumes, "Please input a number to select which drive to backup \033[1mto\033[0m (ex: Geek Squad Backup): ") + getTagAndDate();
		cout << "================================================================================\n\n";
        toDir += getFolderName(fromDir).substr(0,1);
        toDir = toDir.substr(0, toDir.length()-1);
        system(("mkdir " + checkPath(toDir) + " >> /dev/null 2>> /dev/null").c_str());
        toDir += user.substr(1);
		cout << "\n\n\033[34m" << checkPath(fromDir) << " -> " << checkPath(toDir) << "\033[0m\n\n";
		cout << "Attempting to create backup folder\n";
		system(("mkdir " + checkPath(toDir) + " >> /dev/null 2>> /dev/null").c_str());
		cout << "Determining disk space required...\n";
		donePrint(copyFiles(checkPath(fromDir), checkPath(toDir), errorCount), true, errorCount);
	}
	else 
	{
		cout << "================================================================================\n\n";
        string fromDir = volumes + "Macintosh HD/";
		string toDir = pickFolder(volumes, "Please input a number to select which drive to backup \033[1mto\033[0m (ex: Geek Squad Backup): ") + getTagAndDate();
		cout << "================================================================================\n\n";
		cout << "\n\n\033[34m" << checkPath(fromDir) << " -> " << checkPath(toDir) << "\033[0m\n\n";
		cout << "Attempting to create backup folder\n";
		system(("mkdir " + toDir + " 2>> /dev/null").c_str());
		toDir += "FULL\\ SYSTEM\\ BACKUP/";
		system(("mkdir " + checkPath(toDir) + " > /dev/null 2> /dev/null").c_str());
		cout << "Determining disk space required...\n";
		int errors;
		donePrint(copySystem(checkPath(fromDir), checkPath(toDir), true, errorCount), true, errorCount);
	}
	// Time Machine through terminal is way too shady and doesn't seem to work on most installs of Mac OS X. Full backup should be used instead;
	/*else
	{
		cout << "================================================================================\n\n\033[31mWarning: Your version of Mac OS may or may not support Time Machine functions via terminal through Recovery Mode. It is recommended that you do a full backup instead!\033[0m\n\n";
		string toDir = pickFolder(volumes, "Please input a number to select which drive to backup \033[1mfrom\033[0m. This \033[1mMUST\033[0m be formatted for either \033[1mHFS+\033[0m or \033[1mAPFS\033[0m. \nIf you do not have access to one of these, use Disk Utility to create a Sparse Bundle Disk Image and select that: ");
		cout << "================================================================================\n\n";
		cout << "\n\nMacintosh HD -> Time Machine @ " << toDir << "\n\n";
		
		system("cd /");
		cout << "Attempting to enable ownership of volume\n";
		system(("sudo diskutil enableOwnership " + toDir).c_str());
		cout << "Beginning to copy. This may take some time!\n";
		system(("sudo tmutil setdestination " + toDir).c_str());
		system("sudo tmutil startbackup -b");
		int errors;
		donePrint(0, true, errors);
	}*/
}
void restore(bool isUser)
{
	if (isUser)
	{
		cout << "================================================================================\n\n";
		string fromDir = pickFolder(volumes, "Please select which drive to restore \033[1mfrom\033[0m (ex: Geek Squad Backup): ") + "/";
		cout << "================================================================================\n\n\n\n";
		fromDir = pickFolder(fromDir, "Please select the corresponding tag #/folder name: ") + "/";
		cout << "================================================================================\n\n\n\n";
		fromDir = pickFolder(fromDir, "Please input a number to select a user to restore \033[1mfrom\033[0m: ") + "/";
		cout << "================================================================================\n\n\n\n";
		string toDir = volumes + "Macintosh\\ HD/Users/";
		toDir = pickFolder(toDir, "Please input a number to select a user to restore \033[1mto\033[0m: ") + "/";
		cout << "================================================================================\n\n\n\n";
		cout << "\n\n\033[34m" << checkPath(fromDir) << " -> " << checkPath(toDir) << "\033[0m\n\n";
		cout << "Determining disk space required...\n";
		int errors;
		donePrint(copyFiles(checkPath(fromDir), checkPath(toDir), errors), false, errors);
	}
	else
	{
		cout << "\n";
		string fromDir = pickFolder(volumes, "Please select which drive to restore \033[1mfrom\033[0m (ex: Geek Squad Backup): ") + "/";
		cout << "================================================================================\n\n\n\n";
		fromDir = pickFolder(fromDir, "Please select the corresponding tag #/folder name: ") + "/FULL\\ SYSTEM\\ BACKUP/";
		cout << "================================================================================\n\n\n\n";
		string toDir = "/";
		cout << "\n\n\033[34m" << checkPath(fromDir) << " -> " << checkPath(toDir) << "\033[0m\n\n";
		if (promptBool("Do you want to copy /System/? (Not recommended unless computer currently boots fine) (y/n): ", 'y', 'n'))
		{
			cout << "Determining disk space required...\n";
			int errors;
			donePrint(copySystem(checkPath(fromDir), checkPath(toDir), true, errors), false, errors);
		}
		else
		{
			cout << "Determining disk space required...\n";
			int errors;
			donePrint(copySystem(checkPath(fromDir), checkPath(toDir), false, errors), false, errors);
		}
	}
}
void tools(int type)
{
    switch (type)
    {
        case 0:
            cout << "Moving /Applications/ folder\n";
            system(("mv /Volumes/Macintosh\\ SSD/Applications /Volumes/Macintosh\\ HD/Applications >> /dev/null 2>> /dev/null"));
            cout << "Moving /Users/ folder\n";
            system(("mv /Volumes/Macintosh\\ SSD/Users /Volumes/Macintosh\\ HD/Users >> /dev/null 2>> /dev/null"));
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
void run(char type)
{
	if (type == 'b')
	{
		backup(promptBool("Do you want to do a user backup (u) or a full disk copy (f): ", 'u', 'f'));
	}
	else if (type == 'r')
	{
		restore(promptBool("Do you want to restore from a user backup (u) or a full disk copy (f): ", 'u', 'f'));
	}
    else if (type == 't')
    {
        vector<string> list(1);
        list[0] = "Offload Applications and Users from SSD to HD";
        tools(promptList(list,"Please select an option:"));
    }
    else {
        cout << "Error: Invalid!\n";
    }
}
void startSequence()
{
	system("stty -echoctl");
	system("cd /");
	cout	<< "   _____      _      _        _____ _____  \n"
			<< "  / ____|    | |    ( )      / ____|  __ \\ \n"
			<< " | |     ___ | | ___|/ ___  | |    | |__) |\n"
			<< " | |    / _ \\| |/ _ \\ / __| | |    |  ___/ \n"
			<< " | |___| (_) | |  __/ \\__ \\ | |____| |     \n"
			<< "  \\_____\\___/|_|\\___| |___/  \\_____|_|     \n  "
			<< "v1.5 ------------------------------------------------------\n"
			<< "by Cole Crouter                                            \n"
			<< "================================================================================\n\n";
	vector<string> test = getFiles("/Volumes/");
	bool mounted = false;
	system("cd /");

	for (int i = 0; i < test.size(); i++)
	{
        if (test[i].find("Macintosh HD") != std::string::npos)
		{
			mounted = true;
			cout << "\033[31mWARNING: It is HIGHLY recommended that you run this program within Recovery Mode! Running through the desktop will only allow you to backup and restore the user currently logged in. \033[0m\n\n================================================================================\n\n";
		}
	}

	if (!mounted)
	{
		system("diskutil mountDisk disk0");
		test = getFiles("/Volumes/");

		for (int i = 0; i < test.size(); i++)
		{
			if (test[i].find("Macintosh HD") != std::string::npos)
			{
				mounted = true;
			}
		}
	}

	if (!mounted)
	{
		cout << "\033[31mWARNING: Macintosh HD failed to mounted, please open Disk Utility and mount Macintosh HD, then try again. If the main drive is not named Macintosh HD, please rename the drive in Disk Utility (right-click or control-click on the drive in the sidebar). If the main disk is locked with a password, please mount the disk in Disk Utility, enter the password and try again.\033[0m\n\n================================================================================\n\n";
		system("/Applications/Utilities/Disk\\ Utility.app/Contents/MacOS/Disk\\ Utility");
		exit(0);
	}

	cout << "Enter \033[34mq\033[0m into any menu to quit or restart the program!\n\n";
}
int main()
{
	startSequence();
	do
	{
		char type = promptChar("\nDo you want to create a backup (b), restore files onto this computer (r), or use tools (t)? ", 'b', 'r', 't');
		cout << "================================================================================\n\n";
		run(type);
	} while (promptBool("Do you want to do another transfer? (y/n) ", 'y', 'n'));

	cout << "Performing closing operations...\n";
	system("stty echoctl");
	return 0;

}
