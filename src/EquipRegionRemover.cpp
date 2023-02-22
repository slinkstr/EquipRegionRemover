using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nShow)
{
    // getting environment variable for program files x86
    char* pf86;
    size_t requiredSize;

    getenv_s(&requiredSize, NULL, 0, "programfiles(x86)");
    if (requiredSize == 0)
    {
        printf("Could not find Program Files (x86).\n");
        exit(1);
    }

    pf86 = (char*)malloc(requiredSize * sizeof(char));
    if (!pf86)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    getenv_s(&requiredSize, pf86, requiredSize, "programfiles(x86)");

    // using environment variable to open items_game.txt
    ostringstream itemsgame;
    itemsgame << pf86 << "\\Steam\\steamapps\\common\\Team Fortress 2\\tf\\scripts\\items\\items_game.txt";
    ifstream infile;
    infile.open(itemsgame.str());

    // opening the output file items_custom.txt
    ostringstream itemscustom;
    itemscustom << pf86 << "\\Steam\\steamapps\\common\\Team Fortress 2\\tf\\scripts\\items\\items_custom.txt";
    ofstream outfile;
    outfile.open(itemscustom.str());

    // performing operations on open file
    string line;
    int i = 0;
    while (getline(infile, line))
    {
        int foundPos = line.find("\"equip_region\"");

        if (foundPos == string::npos)
        {
            outfile << line << endl;
        }
        else
        {
            // "equip_region" followed by a list of regions
            // we want to skip all of those lines
            if (line.length() - foundPos == 14)
            {
                do
                {
                    // keep advancing until we hit the closing curly brace
                    getline(infile, line);
                } while (line.find("}") == string::npos);
            }

            // otherwise we can be content with just ignoring the line
            i++;
        }
    }

    // construct output message
    ostringstream oss;
    oss << "Removed " << i << " equip regions.";

    // convert output message to wstring
    wstring matches;
    string s = oss.str();
    matches.assign(s.begin(), s.end());

    // convert wstring output to LPCWSTR
    LPCWSTR output;
    output = matches.c_str();

    // alert user
    MessageBox(0,
        output,
        L"EquipRegionRemover",
        MB_OK);

    // exit
    infile.close();
    outfile.close();
}