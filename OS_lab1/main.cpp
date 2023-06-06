#include <windows.h>
#include <iostream>
#include <fstream>
#include "employee.h"
#include <vector>

void startProcess(char *buffer)
{

	// initialize new process
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	if (!CreateProcess(NULL, buffer, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		std::cout << "\nERROR: Unable to open process\n";
		std::cout << "Press any key to exit...";
		system("pause");
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void printInfo(std::vector<employee> records)
{
	std::cout << records[0].num << " " << records[0].name << " " << records[0].hours;
	for (int i = 1; i < records.size(); i++)
	{
		std::cout << std::endl
				  << records[i].num << " " << records[i].name << " " << records[i].hours;
	}
}

int main()
{
	char binaryFileName[256];

	std::cout << "Enter binary file name: ";
	std::cin.getline(binaryFileName, 256);
	int recordsNum = 0;
	std::cout << "Enter the number of records: ";
	std::cin >> recordsNum;
	std::cin.ignore();

	char buffer[1000];
	sprintf_s(buffer, "%s %s %d", "creator.exe", binaryFileName, recordsNum);
	startProcess(buffer);

	// read info from binary file
	std::cout << "Information in binary file:" << std::endl;
	std::ifstream fin(binaryFileName, std::ios::binary);
	fin.read(reinterpret_cast<char *>(&recordsNum), sizeof(int));
	
	std::vector<employee> records(recordsNum);

	//read an array
	for (int i = 0; i < recordsNum; i++)
	{
		fin.read(reinterpret_cast<char *>(&records[i]), sizeof(employee));
	}

	printInfo(records);

	fin.close();

	//initialize file for repoter and start reporter
	char outputFileName[256];
	std::cout << "\n\nEnter output file name: ";
	std::cin.getline(outputFileName, 256);
	double salaryPerHour = 0.0;
	std::cout << "Enter salary value per one hour: ";
	std::cin >> salaryPerHour;

	buffer[0] = '\0';
	sprintf_s(buffer, "%s %s %s %lf", "reporter.exe", binaryFileName, outputFileName, salaryPerHour);
	startProcess(buffer);

	std::ifstream finReportInfo(outputFileName);

	//read info from output file
	std::cout << std::endl
			  << "Information in output file:" << std::endl;
	char str[1000];
	for (int i = 0; i < recordsNum; i++)
	{
		finReportInfo.getline(str, 1000);
		for (int i = 0; str[i] != '\0'; i++)
			printf("%c", str[i]);
		std::cout << std::endl;
	}
	finReportInfo.close();
	return 0;
}