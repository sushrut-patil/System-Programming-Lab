#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int checkDataType(char *string, char *(*Datatypes)[6])
{
    char **array = *Datatypes;
    for (int i = 0; i < 6; i++)
    {
        if (strstr(string, array[i]))
        {
            return i;
        }
    }
    return -1;
}
void trim(char **line, char *Data)
{
    *line = *line + strlen(Data);
}
int main()
{
    FILE *fileptr = fopen("Code.c", "r");
    if (fileptr == NULL)
    {
        perror("fopen");
        exit(1);
    }

    static const char *Datatypes[6] = {"char", "short", "int", "long", "float", "double"};
    int DataSize[6] = {1, 2, 4, 4, 4, 8};
    char buffer[256];
    int address = 100;
    printf("\nSymbol\tType\tLength\tAddress\n\n");
    while (fgets(buffer, sizeof(buffer), fileptr))
    {
        char *line = strdup(buffer);
        while (isspace((unsigned char)*line))
            line++;

        if (*line == '\0')
        {
            continue;
        }
        int ret = checkDataType(line, &Datatypes);
        if (ret != -1)
        {
            trim(&line, Datatypes[ret]);
            while (isspace((unsigned char)*line))
                line++;

            char symbol[100];
            memset(symbol, 0, sizeof(symbol));
            while (*line != ';')
            {
                if (*line == ',')
                {
                    printf("%s\t%s\t%d\t%d\n", symbol, Datatypes[ret], DataSize[ret], address);
                    address = address + DataSize[ret];
                    memset(symbol, 0, sizeof(symbol));
                }
                else if (*line == '[')
                {
                    strncat(symbol, &line[0], 1);
                    line++;
                    strncat(symbol, &line[0], 1);
                    int num = (int)(*line) - 48;
                    line++;
                    strncat(symbol, &line[0], 1);
                    if (line[1] != ';') line++;

                    printf("%s\t%s\t%d\t%d\n", symbol, Datatypes[ret], num * DataSize[ret], address);
                    address = address + DataSize[ret] * num;

                    memset(symbol, 0, sizeof(symbol));
                    
                }
                else
                {
                    strncat(symbol, &line[0], 1);
                }
                line++;
            }
            if (symbol[0] != '\0')
            {
                printf("%s\t%s\t%d\t%d\n", symbol, Datatypes[ret], DataSize[ret], address);
                address = address + DataSize[ret];
            }
        }
    }
    fclose(fileptr);

    return 0;
}