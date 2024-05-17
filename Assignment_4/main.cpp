#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct symbol,literal{
    int value;
    int length;
    char Relocation;
};

struct machineop {

};
unordered_map<string, struct symbol> symbolTable;
unordered_map<string, struct literal> literalTable;

// Pseudo-operation table
map<string, int> pseudoopTable = {
    {"START", 1}, {"DO", 2}, {"EQU", 3}, {"USING", 4}, {"DROP", 5}, {"END", 6}};

// Machine operation table
map<string, int> machineOpTable = {
    {"ADD", 1}, {"SUB", 2}, {"MUL", 3}, {"DIV", 4}, {"MOV", 5}, {"LD", 6}, {"ST", 7}};

void pass1(const string &inputFile)
{
    ifstream file(inputFile);
    string line;
    int LC = 0;
    while (getline(file, line))
    {
        string opcode = line.substr(0, line.find(' '));
        // Search machine op table (POTGET1)
        if (pseudoOpTable.count(opcode))
        {
            // Handle pseudo-operations
            int opValue = pseudoOpTable[opcode];
            switch (opValue)
            {
            case 1: // DS
                // Adjust LC for proper alignment
                break;
            case 2: // DO
                // Adjust LC for proper alignment
                break;
            case 3: // EQU
                // Evaluate operand field (EVAL)
                // Assign value to symbol in label field (STSTO)
                break;
            case 4: // USING
                // Handle USING directive
                break;
            case 5: // DROP
                // Handle DROP directive
                break;
            case 6: // END
                // Handle END directive
                break;
            }
            else if (machineOpTable.count(opcode))
            {
                int opValue = machineOpTable[opcode];

                // Handle different opcodes
                switch (opValue)
                {
                case 1: // ADD
                case 2: // SUB
                case 3: // MUL
                case 4: // DIV
                case 5: // MOV
                case 6: // LD
                case 7: // ST
                    // Evaluate operand field (EVAL)
                    // Assign value to symbol in label field (STSTO)
                    // Assign current value of LC to symbol (STSTO)
                    break;
                }
            }
        }
        // Check for symbols in symbol field
        // If symbol exists, increment LC by length of data field

        // Write copy of card for pass 2 (WRITE1)
    }

    file.close();
}

int main()
{
    string inputFile = "input.asm";
    pass1(inputFile);
    return 0;
}