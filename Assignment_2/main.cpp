#include <bits/stdc++.h>
#include "helper.cpp"
using namespace std;

struct macroname
{
    int index;
    string name;
    int mdtIndex;
};

struct macrodef
{
    int index;
    string line;
};

int mdtIndex = 100;
int mntIndex = 10;

void createTable(ifstream &fdef, vector<macroname> &mnt, vector<macrodef> &mdt)
{
    string line;
    while (getline(fdef, line))
    {
        if (line.empty())
            continue;

        if (line.find("macro") != -1)
        {
            trim(line);
            line.erase(line.begin(), line.begin() + 5);
            trim(line);

            macroname *mstruct = new macroname();
            mstruct->index = mntIndex++;
            mstruct->name = line;
            mstruct->mdtIndex = mdtIndex;
            mnt.push_back(*mstruct);

            do
            {
                getline(fdef, line);
                if (line.empty())
                    continue;
                macrodef *mdef = new macrodef();
                mdef->index = mdtIndex++;
                trim(line);
                mdef->line = line;
                mdt.push_back(*mdef);
            } while (line.find("mend") == -1);
        }
    }
}

int mdtFind(vector<macrodef> &mdt, int index)
{
    for (int i = 0; i < mdt.size(); i++)
    {
        if (mdt[i].index == index)
        {
            return i;
        }
    }
    return -1;
}
int main()
{
    string inputfile = "../Assignment_2/input.txt";
    string macro_def = "../Assignment_2/macro_def.txt";
    string outputfile = "../Assignment_2/output.txt";

    ifstream fin, fdef;
    fin.open(inputfile);
    fdef.open(macro_def);

    ofstream fout;
    fout.open(outputfile);

    vector<macroname> mnt;
    vector<macrodef> mdt;

    fdef.seekg(0);
    if (fdef.is_open())
    {
        createTable(fdef, mnt, mdt);
    }
    cout << "Index\tMacro\tMDTIndex\n";
    for (auto i : mnt)
    {
        cout << i.index << "\t" << i.name << "\t" << i.mdtIndex << "\n";
    }
    cout << "\nIndex\tCode\n";
    for (auto j : mdt)
    {
        cout << j.index << "\t" << j.line << "\n";
    }
    cout << endl;

    fin.seekg(0);
    fout.seekp(0);
    if (!fin.is_open() || !fout.is_open())
    {
        exit(1);
    }

    string line;
    while (getline(fin, line))
    {
        trim(line);
        bool flag = false;
        for (auto i : mnt)
        {
            if (i.name == line)
            {
                int index = i.mdtIndex;
                int mdt_loc = mdtFind(mdt, index);
                if (mdt_loc == -1) exit(2);
                while (mdt[mdt_loc].line != "mend")
                {
                    fout << mdt[mdt_loc].line <<"\n";
                    mdt_loc++;
                }
                fout << flush;
                flag = true;
                break;
            }
        }
        if (!flag) fout << line << "\n";
    }

    fin.close();
    fout.close();

    return 0;
}