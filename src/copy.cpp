#include "files.h"
#include "colours.h"
using namespace std;

const string volumes = "/Volumes/";


void startSequence() {
    system("osascript -e \'tell application \"Terminal\" to set current settings of selected tab of front window to first settings set whose name is \"Pro\"\'");
	system("stty -echoctl");
	system("cd /");
	cout	<< "   _____      _      _        _____ _____  \n"
			<< "  / ____|    | |    ( )      / ____|  __ \\ \n"
			<< " | |     ___ | | ___|/ ___  | |    | |__) |\n"
			<< " | |    / _ \\| |/ _ \\ / __| | |    |  ___/ \n"
			<< " | |___| (_) | |  __/ \\__ \\ | |____| |     \n"
			<< "  \\_____\\___/|_|\\___| |___/  \\_____|_|     \n  "
			<< "v1.6 ------------------------------------------------------\n"
			<< "by " << BOLD << CYAN << "Cole Crouter" << RESET << "\n"
			<< "================================================================================\n\n";
	system("cd /");

	cout << "Enter " << BOLD << BLUE << "q" << RESET << " into any menu to quit or restart the program!\n\n";
}
int main()
{
	startSequence();
	do
	{
		DataTransfer* t = new DataTransfer();
		Session* s = new Session();
        s->transfer = t;
        char options[] = {'b', 'r', 'm', 't'};
        char type = s->promptChar("\nDo you want to create a backup (" BOLD GREEN "b" RESET "), restore files onto this computer (" BOLD GREEN "r" RESET "), do a user migration (" BOLD GREEN "m" RESET "), or use tools (" BOLD GREEN "t" RESET "): ", options, 4);
		s->run(type);
	} while (promptFinished());

	cout << "Performing closing operations...\n";
	system("stty echoctl");
	return 0;

}
