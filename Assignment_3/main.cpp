#include <bits/stdc++.h>
#include "helper.cpp"
using namespace std;

struct macroname
{
    int index;
    int mdtIndex;
};

struct macrodef
{
    int index;
    string line;
};

int mdtIndex = 100;
int mntIndex = 10;

vector<pair<string, string>> Extract_Arguments(string line)
{
    vector<pair<string, string>> args;
    if (line.empty())
    {
        return args;
    }
    string para = "";
    int i = -1;
    do
    {
        i++;
        if (line[i] == ',' || line[i] == '\0')
        {
            pair<string, string> temp{para, ""};
            args.push_back(temp);
            para = "";
        }
        else
        {
            para += line[i];
        }
    } while (line[i] != '\0');
    return args;
}

void Actual_Arguments(string mname,unordered_map<string, vector<pair<string, string>>> &arguments,string line)
{
    string para = "";
    int i = -1,j = 0;
    do
    {
        i++;
        if (line[i] == ',' || line[i] == '\0')
        {
            arguments[mname][j].second = para;
            j++;
            para = "";
        }
        else
        {
            para += line[i];
        }
    } while (line[i] != '\0');
}
void createTable(ifstream &fdef, unordered_map<string, macroname> &mnt, vector<macrodef> &mdt, unordered_map<string, vector<pair<string, string>>> &arguments)
{
    string line;
    while (getline(fdef, line))
    {
        if (line.empty())
            continue;

        if (line.find("macro") != string::npos)
        {
            trim(line);
            line.erase(line.begin(), line.begin() + 5);
            trim(line);
            // Extract Marco Name
            string mname = "";
            int i = 0;
            for (i = 0; line[i] != ' '; i++)
            {
                mname += line[i];
            }
            mname += '\0';
            // Store Arguments in vector
            line.erase(line.begin(), line.begin() + i + 1);

            macroname *mstruct = new macroname();
            mstruct->index = mntIndex++;
            mstruct->mdtIndex = mdtIndex;
            mnt[mname] = *mstruct;

            arguments[mname] = Extract_Arguments(line);

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
            } while (line.find("mend") == string::npos);
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
void expandCode(ifstream &fin,
                ofstream &fout,
                unordered_map<string, macroname> &mnt,
                vector<macrodef> &mdt,
                unordered_map<string,
                              vector<pair<string, string>>> &arguments)
{
    string line;
    while (getline(fin, line))
    {
        trim(line);
        // Extract Marco Name
        string mname = "";
        int i = 0;
        for (i = 0; line[i] != ' ' && line[i] != '\0'; i++)
        {
            mname += line[i];
        }
        mname += '\0';
        
        if (mnt.find(mname) != mnt.end())
        {
            // Store Arguments in vector
            line.erase(line.begin(), line.begin() + i + 1);
            Actual_Arguments(mname,arguments,line);

            int index = mnt[mname].mdtIndex;
            index = mdtFind(mdt,index);
            while (mdt[index].line != "mend")
            {
                string code = mdt[index].line;
                int pos = code.find("&");
                if (pos != string::npos)
                {
                    string var = "&";
                    for (int n = pos + 1; code[n] != '\0'; n++)
                    {
                        var += code[n];
                    }
                    code.erase(code.begin() + pos , code.end());
                    string val;
                    for (auto i : arguments[mname])
                    {
                        if (i.first == var)
                        {
                            val = i.second;
                            break;
                        }
                    }
                    code += val;  
                }
                fout << code << "\n";
                index++;
            }
            fout << flush;
        }
        else
        {
            fout << line << "\n";
        }
        
    }
}
int main()
{
    string inputfile = "../Assignment_3/input.txt";
    string macro_def = "../Assignment_3/macro_def.txt";
    string outputfile = "../Assignment_3/output.txt";

    ifstream fin, fdef;
    fin.open(inputfile);
    fdef.open(macro_def);

    ofstream fout;
    fout.open(outputfile);

    unordered_map<string, macroname> mnt;
    vector<macrodef> mdt;
    unordered_map<string, vector<pair<string, string>>> arguments;

    fdef.seekg(0);
    if (fdef.is_open())
    {
        createTable(fdef, mnt, mdt, arguments);
    }
    cout << "Index\tMacro\tMDTIndex\n";
    for (auto i : mnt)
    {
        cout << i.second.index << "\t" << i.first << "\t" << i.second.mdtIndex << "\n";
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
    expandCode(fin,fout,mnt,mdt,arguments);
    cout << "\nMacroName\tParameter\tPostion\t\tValue\n";
    for (auto i : arguments)
    {
        int count = 1;
        for (auto j : i.second)
        {
            cout << i.first << "\t\t#" << count << "\t\t" << j.first <<"\t\t"<< j.second<< "\n";
            count++;
        }
    }
    cout <<endl;
    fin.close();
    fout.close();

    return 0;
}