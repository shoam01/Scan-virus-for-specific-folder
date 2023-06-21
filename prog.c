/*********************************
* Class: MAGSHIMIM C2            *
* Week: 8                        *
* Name: shoam avraham            *
**********************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dirent.h"
// ba CECAF laasot <dirent.h>

#define PARAM_NUM 2
#define NEW_LINE '\n'
#define COMMA ','
#define MAX_LEN 256

int findVirus(char* srcText, char* virusText, int start, int end);
void fgetStr(char str[], int size);


int main()
{
    FILE* srcFile = NULL;
    char srcFileDir[MAX_LEN] = { 0 };

    FILE* virusFile = NULL;

    DIR* d = NULL;
    struct dirent* dir = NULL;
    char folderName[MAX_LEN] = { 0 };

    int size = 0;
    int i = 0;
    int j = 0;
    int sizeVirus = 0;
    int start = 0;
    int end = 0;

    char choice = ' ';
    char* virusText = NULL;
    char* srcText = NULL;
    char* src = NULL;
    char* virus = NULL;


    printf("\nFolder to scan: ");
    fgets(folderName, MAX_LEN, stdin);
    folderName[strcspn(folderName, "\n")] = NULL;

    d = opendir(folderName);

    if (d == NULL)
    {
        printf("Error in opening directory.\n");
        return 1;
    }

    printf("\nvirus signature:  ");
    fgets(virus, MAX_LEN, stdin);
    virus[strcspn(virus, "\n")] = NULL;

    virusFile = fopen(virus, "br");

    if (virusFile == NULL)
    {
        printf("Error in opening file.\n");
        return 1;
    }

    fseek(virusFile, 0, SEEK_END);
    sizeVirus = ftell(virusFile);
    fseek(virusFile, 0, SEEK_SET);

    virusText = (char*)malloc(size);

    fread(virusText, sizeof(char), sizeVirus, virusFile);

    printf("Press 0 for a normal scan or any other key for a quick scan: ");
    scanf("%d", &choice);
    getchar();

    printf("Scanning began...\nThis process may take several minutes...\n");

    while ((dir = readdir(d)) != NULL)
    {
        // Current File Name - dir->d_name (kitten.jpg)
        if (dir->d_type != DT_DIR)
        {

            strncpy(srcFileDir, folderName, strlen(folderName) + 1);
            strncat(srcFileDir, "/", 2);
            strncat(srcFileDir, dir->d_name, strlen(dir->d_name) + 1);

            srcFile = fopen(srcFileDir, "br");

            if (srcFile == NULL)
            {
                printf("error opening file.\n");
            }

            else
            {
                printf("%s - ", srcFileDir);

                fseek(srcFile, 0, SEEK_END);
                size = ftell(srcFile);
                fseek(srcFile, 0, SEEK_SET);

                srcText = (char*)malloc(size);

                fread(srcText, sizeof(char), size, srcFile);



                switch (choice) // Test conditions with fast scan or standard scan 
                {
                case 0:
                    start = 0;
                    end = size + 1;
                    printf(findVirus(srcText, virusText, start, end) ? "Infected!" : "clean");
                    break;

                default:
                    start = 0;
                    end = size / 5;
                    if (!findVirus(srcText, virusText, start, end))
                    {
                        start = size - (size / 5);
                        end = size;
                        if (!findVirus(srcText, virusText, start, end))
                        {
                            start = size / 5;
                            end = size - (size / 5);
                            if (!findVirus(srcText, virusText, start, end))
                            {
                                printf("Clean\n");
                            }
                            else
                            {
                                printf("Infected!\n");
                            }
                        }
                        else
                        {
                            printf("Infected! (last 20%)\n");
                        }
                    }
                    else
                    {
                        printf("Infected! (first 20%)\n");
                    }
                    break;
                }
            }
        }
    }
    printf("scan completed");
    printf("see log path for results: ");

    free(srcText);
    free(virusText);
    closedir(d);
    getchar();
    return 0;
}


/*
  The function checks where the virus is signed
  Input: virusText, srcText, start, end
  Output: Whether there is or not
*/
int findVirus(char* srcText, char* virusText, int start, int end)
{
    int virus = 0;
    int i = 0;
    int j = 0;
    int counter = 0;

    for (i = start; i < end && !virus; i++)
    {
        for (j = 0; j < strlen(virusText) && !virus; j++)
        {
            if (counter == strlen(virusText))
            {
                virus = 1;
            }

            if (virusText[j] == srcText[i + j])
            {
                counter++;
            }
        }
    }

    return virus;
}