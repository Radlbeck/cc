/*
 */
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

string getWiFi(void)
{
	string buff = "Error :(";
	char* cmd   = "nm-tool";
	
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
        char buffer[128];
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
	    		buff += buffer;
	}
	pclose(pipe);
	
	return buff;
}

int main(int argc, char* argv[])
{
	cout << getWiFi() << endl;

	return 0;
} 
