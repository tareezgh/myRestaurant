//  HW 3 - Semester 2 2020 - Grade 100
//  Created by Tareez Ghandour
//  Copyright Tareez Ghandour © All rights reserved.
//  Main File

#include "Header.h"

int main()
{
    FILE* instruction, * out, * Manot;
    int i, NumOfTabels, function_num, TableNumber, QuantityToAdd, QuantityToRemove;
    char ProductName[50];
    ProductManager PM;
    TablesManager* TM_Arr;


    if (!(instruction = fopen("Instructions.txt", "r"))) //open the instruction file
    {
        printf("Cannot open the Instructions file!");
        exit(1);
    }

    if (!(out = fopen("output.txt", "w"))) // open the output file
    {
        printf("Cannot open the Output file!");
        fclose(instruction);
        exit(1);
    }

    if (!(Manot = fopen("Manot.txt", "r"))) // open the Manot file
    {
        printf("Cannot open Manot file!");
        fclose(instruction);
        fclose(out);
        exit(1);
    }

    PM.count = 0;
    PM.head = NULL;
    PM.tail = NULL;

    fscanf(instruction, "%d", &NumOfTabels); // recieve the number of tabels

    if ((TM_Arr = (TablesManager*)malloc(sizeof(TablesManager)*NumOfTabels)) == NULL)
    {
        printf("NO ENOUGH MEMORY!");
        fclose(instruction);
        fclose(out);
        fclose(Manot);
        exit(1);
    }


    for (i = 0; i < NumOfTabels; i++) // Initialize the array
    {
        TM_Arr[i].head = NULL;
        TM_Arr[i].tail = NULL;
        TM_Arr[i].count = 0;
        TM_Arr[i].TotalPrice = 0;
    }

    while (fscanf(instruction, "%d", &function_num) != EOF) // while we didn't reach the end of file
    {
        switch (function_num)
        {
            case 1: // Function 1
                CreateProducts(&PM, Manot, out, instruction);
                fclose(Manot);
                break;

            case 2: // Function 2
                fscanf(instruction, "%s %d", ProductName, &QuantityToAdd); // recieve the data from the instructions file
                AddItems(&PM, ProductName, QuantityToAdd, out);
                break;

            case 3: // Function 3
                fscanf(instruction, "%d %s %d", &TableNumber, ProductName, &QuantityToAdd); // recieve the data from the instructions file
                if (TableNumber > NumOfTabels)
                {
                    fprintf(out, "Table number %d is not exist in our resturant!\n",TableNumber);
                    break;
                }
                if (QuantityToAdd < 0) // check if the quantity is valid
                {
                    fprintf(out, "The Quantity should be positive number!\n");
                    break;
                }
                OrderItem(&PM, TM_Arr, TableNumber, NumOfTabels, ProductName, QuantityToAdd, out, instruction);
                break;

            case 4: // Function 4
                fscanf(instruction, "%d %s %d",&TableNumber, ProductName, &QuantityToRemove); // recieve the data from the instructions file
                if (TableNumber > NumOfTabels)
                {
                    fprintf(out, "Table number %d is not exist in our resturant!\n",TableNumber);
                    break;
                }
                if (QuantityToRemove < 0) // check if the quantity is valid
                {
                    fprintf(out, "The Quantity that you want to remove should be positive number!\n");
                    break;
                }
                RemoveItem(&PM, TM_Arr, TableNumber, ProductName, QuantityToRemove, out);
                break;

            case 5: // Function 5
                fscanf(instruction, "%d", &TableNumber); // recieve the data from the instructions file
                RemoveTable(PM, TM_Arr, TableNumber, NumOfTabels, out);
                break;

            default:
                break;
        }
    }
    FreeProductsList(&PM);
    free(TM_Arr);
    fclose(out);
    fclose(instruction);
    return 0;
}
