#include <bits/stdc++.h>
using namespace std;

std::string To_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

string convert(int decimal)
{
	string binary = "";
	for(int i = decimal;i > 0;i/=2)
		binary = To_string(i%2) + binary;
	if(binary.length() < 8)
		binary = string(8-binary.length(),'0').append(binary);
	return binary;
}

struct mnemonics{
	string name;
	string binary;
	int size;
}mot[13];

struct symbol{
	string name;
	string type;
	int location;
	int size;
	int section_id;
};

struct section{
	int id;
	string name;
	int size;
};

vector<symbol> symTab;
vector<section> sec;
int lc = 0;
int sec_id = 0;
int var_lc;
ifstream infile;
ofstream outfile;
string word;
string temp;
int control,Size = 0;

void init()
{
	mot[0] = {"ADD","00000001",1};
	mot[1] = {"ADDI","00000010",5};
	mot[2] = {"CMP","00000011",5};
	mot[3] = {"INC","00000100",1};
	mot[4] = {"JE","00000101",5};
	mot[5] = {"JMP","00000110",5};
	mot[6] = {"LOAD","00000111",5};
	mot[7] = {"LOADI","00001000",1};
	mot[8] = {"MVI","00001001",5};
	mot[9] = {"MOV","00001010",1};
	mot[10] = {"STOP","00001011",1};
	mot[11] = {"STORE","00001100",5};
	mot[12] = {"STORI","00001101",1};
}

int search_mot(string opcode) 
{
	int index = -1;
	for(int i = 0;i < 13;i++)
	{
		if(mot[i].name == opcode)
		{
			index = i;
			break;
		}
	}
	return index;
}

int search_symbol(string variable)
{
	int location = -1;
	for(vector<symbol>::const_iterator i = symTab.begin();i != symTab.end();++i)
	{
		if(i->name == variable)
		{
			location = i->location;
			break;
		}
	}
  	return location;
}

int size_evaluation(string data)
{
	int size = 0;
	for(int i = 0;i < data.length();i++)
	{
		if(data[i] == ',')
			size += 4;
	}
	size += 4;
	return size;
}

string data_break(string data)
{
	string final;
	string temporary = "";
	for(int i = 0;i < data.length();i++)
	{
		if(data[i] == ',')
		{
			final += convert(atoi(temporary.c_str()))+",";
			temporary = "";
		}
		else 
			temporary += data[i];
	}
	final.erase(final.length()-1,1);
	return final;
}

void print_symbols() 
{
	cout << "Name Type Location Size SectionID\n";
	for(vector<symbol>::const_iterator i = symTab.begin();i != symTab.end();++i)
	{
		cout << i->name<<"\t";
		cout << i->type<<"\t";
		cout << i->location<<"\t";
		cout << i->size<<"\t";
		cout << i->section_id<<"\n";
	}	
}

void print_sec()
{
	cout << "\nID\tName\tSize\n";
	for(vector<section>::const_iterator i = sec.begin();i != sec.end();++i)
	{
		cout << i->id<<"\t";
		cout << i->name<<"\t";
		cout << i->size<<"\n";
	}
}

void pass1()
{
	infile.open("input.txt");
	while(infile >> word)
	{
		control = search_mot(word);
		if(control == -1)
		{
			temp = word;
			if(word.find(":") != -1)
			{
				symTab.push_back({temp.erase(word.length()-1,1),"label",lc,-1,sec_id});
			}
			else if(word == "section")
			{
				infile >> word;
				sec_id++;
				sec.push_back({sec_id,word,0});
				if(sec_id != 1)
				{
					sec[sec_id-2].size = lc;
					lc = 0;
				}
			}
			else
			{
				infile >> word;
				infile >> word;
				Size = size_evaluation(word);
				symTab.push_back({temp,"var",lc,Size,sec_id});
				lc += Size;
			}
		}
		else
		{
			if(!(control == 7 || control == 12))
				infile >> word;
			if(control==2 || control==8 || control == 9)
				infile >> word;
			lc += mot[control].size;
		}
	}
	
	sec[sec_id-1].size = lc; //Updating size of current Section
	
	print_symbols();
    print_sec();	
	infile.close();
}

void pass2()
{
	infile.open("input.txt");
	outfile.open("output.txt");
	while(infile >> word)
	{
		control = search_mot(word);
		if(control == -1)
		{
			temp = word;
			if(word.find(":") != -1)
 			{
 				outfile << "";
			}
			else if(word == "section") //No change in Section content
			{
				infile >> word;
				outfile <<"section ."<<word<<endl;
				lc = 0;
			}
			else 
			{
				infile >> word;
				infile >> word;
				outfile <<convert(lc)<<" "<<data_break(word)<<endl;
				Size = size_evaluation(word);
				lc += Size;
			}
		}
		else
		{
			outfile <<convert(lc)<<" "<<mot[control].binary;
			if(control==0||control==3) //ADD and INC have defined register following it
			{
				infile >> word;
				outfile <<" "<<word;
			}
			else if(control==1 || control==4 || control==5 || control==6 || control==11) //ADDI, JE, JMP, LOAD and STORE have one constant following it
			{
				infile >> word;
				var_lc = search_symbol(word);
				if(var_lc == -1)
					outfile <<" "<<convert(atoi(word.c_str()));
				else
					outfile <<" "<<convert(var_lc);
			}
			else if(control==2 || control==8) //CMP and MVI have one register and one constant following it
			{
				infile >> word;
				outfile <<" "<<word;
				infile >> word;
				var_lc = search_symbol(word);
				if(var_lc == -1)
					outfile <<" "<<convert(atoi(word.c_str()));
				else
					outfile <<" "<<convert(var_lc);
			}
			else if(control == 9) //MOV have both registers following it
			{
				infile >> word;
				outfile <<" "<<word;
				infile >> word;
				outfile <<" "<<word;
			}
			lc += mot[control].size;
			outfile << "\n";
		}	
	}
	outfile.close();
	infile.close();
}

int main()
{
	init();
	pass1();
	lc = 0;
	pass2();
	return 0;
}