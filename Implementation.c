//  HW 3 - Semester 2 2020 - Grade 100
//  Created by Tareez Ghandour 
//  Copyright Tareez Ghandour © All rights reserved.
//  Implementation File

#include"Header.h"

// 1
// Function that creates a linked list of products from the file "Manot.txt"
void CreateProducts(ProductManager* list, FILE* in, FILE* out, FILE* instructions)
{
    char TempName[50];
    int TempQuantity, i, flag=0;
    float TempPrice;
    Products* temp;

    while (fscanf(in, "%s%d%f", TempName, &TempQuantity, &TempPrice) != EOF) // recieve the data from the Manot file
    {
        flag = 0;
        if (TempQuantity < 0)
            fprintf(out, "The Quintity should be a positive number\n");

        if(TempPrice < 0)
            fprintf(out, "The Price should be a positive number\n");
        
        if (list->count)
        {
            temp = list->head;
            for (i = 0; i < list->count; i++)
            {
                if (!strcmp(temp->ProductName, TempName))
                {
                    fprintf(out, "This product already added to the kitchen\n");
                    flag = 1;
                }
            }
        }
        if (!flag)
        {
            temp = (Products*)malloc(sizeof(Products)); // Allocate memory for the product
            if (temp == NULL)
            {
                FreeProductsList(list);
                fprintf(out, "Memory allocation failed!\n");
                fclose(in);
                fclose(out);
                fclose(instructions);
                exit(1);
            }
            temp->ProductName = (char*)malloc(sizeof(TempName)); // Allocate memory for product name
            if (!temp->ProductName)
            {
                FreeProductsList(list);
                fprintf(out, "Memory allocation failed!\n");
                fclose(in);
                fclose(out);
                fclose(instructions);
                exit(1);
            }
 
            // Initialize the new node
            strcpy(temp->ProductName, TempName);
            temp->Price = TempPrice;
            temp->Quantity = TempQuantity;
            temp->next = NULL;
            temp->OrdersCounter = 0;

            if (list->head == NULL) // if the products list empty
            {
                list->head = temp;
                list->tail = temp;
                list->count = 1;
            }
            else // if the products list isn't empty, connect the new node to the current list
            {
                list->tail->next = temp;
                list->tail = list->tail->next;
                list->count++;
            }
        }// if
    }// while
    
    fprintf(out, "The kitchen was created\n");
}


// 2
// Function that adds quantity of a product to an existed product
void AddItems(ProductManager* list, char* ProductName, int Quantity, FILE* out)
{
    Products* temp = list->head;

    if (Quantity < 0) // check if the quantity is valid
    {
        fprintf(out, "The Quantity should be positive number!\n");
        return;
    }
    while (temp != NULL)
    {
        if (!strcmp(temp->ProductName, ProductName)) // check if product exist in the kitchen
        {
            temp->Quantity += Quantity;
            fprintf(out, "%d %s were added to the kitchen\n", Quantity, ProductName);
            return;
        }
        temp = temp->next;
    }
    fprintf(out, "We do not have %s in the kitchen", ProductName);
}


//3
// Function that check if the product exist in our kitchen ,then transfer a product from the kitchen to the table, and subtract the quantity
void OrderItem(ProductManager* list, TablesManager* Tables, int TableNubmer, int NumOfTables, char* ProductName, int Quantity, FILE* out, FILE* instruction)
{
    Dish* tempDish;
    int i, flag=0;
    Products* P = list->head;

    for (i = 0; i < list->count; i++)
    {
        if (!strcmp(P->ProductName, ProductName)) // check if product exist in the kitchen
        {
            flag = 1;
            i = list->count;
        }
        if (!flag)
            P = P->next;
    }
    if ((P == NULL) || (Quantity > P->Quantity)) // check if we have enough quantity in the the kitchen
    {
        fprintf(out, "We don't have %s in the kitchen, sorry!\n", ProductName);
        return;
    }
    
    //if the dish doesn't exist, creates a new node
    if (!(tempDish = (Dish*)malloc(sizeof(Dish)))) // Allocate memory for the product
    {
        for (i = 0; i < NumOfTables; i++)
                FreeTableList(&Tables[i]);
        free(Tables);
        FreeProductsList(list);
        fprintf(out, "Memory allocation failed!\n");
        fclose(out);
        fclose(instruction);
        exit(1);
    }
 
    if (!(tempDish->ProductName = (char*)malloc(strlen(ProductName) + 1)))// Allocate memory for product name
    {
        for (i = 0; i < NumOfTables; i++)
                FreeTableList(&Tables[i]);
        free(Tables);
        FreeProductsList(list);
        fprintf(out, "Memory allocation failed!\n");
        fclose(out);
        fclose(instruction);
        exit(1);
    }
    
    // Initialize the new node
    strcpy(tempDish->ProductName, ProductName);
    tempDish->Quantity = Quantity; // update the quantity
    tempDish->Price = (P->Price) * Quantity; // update the price
    P->Quantity -= Quantity; // update the stock
    P->OrdersCounter += Quantity; // update the total orders of the product today
    
    if (Tables[TableNubmer].head == NULL) // if the list is empty
    {
        tempDish->prev = NULL;
        tempDish->next = NULL;
        Tables[TableNubmer].head = tempDish;
        Tables[TableNubmer].tail = tempDish;
        Tables[TableNubmer].count = 1;
        Tables[TableNubmer].TotalPrice += tempDish->Price; // Update the Total Price of the table
    }
    else // if the list is not empty
    {
        tempDish->prev = Tables[TableNubmer].tail;
        tempDish->next = NULL;
        Tables[TableNubmer].tail->next = tempDish;
        Tables[TableNubmer].tail = tempDish;
        Tables[TableNubmer].count ++;
        Tables[TableNubmer].TotalPrice += tempDish->Price; // Update the Total Price of the table
    }
    fprintf(out, "%d %s were added to the table number %d\n", Quantity, ProductName, TableNubmer);
}


//4
//Function that subtract a product from specific table
void RemoveItem(ProductManager* list, TablesManager* Tables, int TableNumber, char* ProductName, int Quantity, FILE* out)
{
    int i, flag=0;
    Products* TempProduct = list->head;
    Dish* P;

    if (Tables[TableNumber].count == 0) // Checks if table is empty
    {
        fprintf(out, "The table number %d is not ordered yet!\n", TableNumber);
        return;
    }
    
    for (i = 0; i < list->count; i++)
    {
        if (!strcmp(TempProduct->ProductName, ProductName)) // check if product exist in the kitchen
        {
            flag = 1;
            i = list->count;
        }
        if (!flag)
            TempProduct = TempProduct->next;
    }
    if (TempProduct == NULL)
    {
        fprintf(out, "We don't have %s in the kitchen, sorry!\n", ProductName);
        return;
    }
    
    flag = 0;
    P = Tables[TableNumber].head;
    for (i = 0; i < Tables[TableNumber].count; i++)
    {
        if (!strcmp(P->ProductName, ProductName)) // check if product exist in the kitchen
        {
            flag = 1;
            i = Tables[TableNumber].count;
        }
        if (!flag)
            P = P->next;
    }
    if (P == NULL)
    {
        fprintf(out, "The product %s was ordered to the table number %d\n", ProductName, TableNumber);
        return;
    }
    if (Quantity > P->Quantity)
    {
        fprintf(out, "The quantity you asked to cancel is more than you ordered\n");
        return;
    }
    P->Quantity -= Quantity;
    P->Price = TempProduct->Price * Quantity;
    Tables[TableNumber].TotalPrice -= TempProduct->Price * Quantity;

    fprintf(out, "%d %s was returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);
    return;
}


// 5
// Function that present data of specific table and delete it
void RemoveTable(ProductManager list, TablesManager* Tables, int TableNumber, int NumOfTables, FILE* out)
{
    int i, flag = 0;
    Products* P, *MaxProduct;
    Dish* temp;

    if (Tables[TableNumber].count == 0)
    {
        fprintf(out, "The table number %d is not ordered yet!\n", TableNumber);
        return;
    }
    temp = Tables[TableNumber].head;
    for (i = 0; i < Tables[TableNumber].count; i++)
    {
        fprintf(out, "%d %s. ", temp->Quantity, temp->ProductName);
        temp = temp->next;
    }
    fprintf(out, "%.2f NIS, please!\n", Tables[TableNumber].TotalPrice);
    FreeTableList(&Tables[TableNumber]);

    for (i = 0; i < NumOfTables; i++)
    {
        if (i != TableNumber && Tables[i].count)
            flag = 1;
    }

    if (!flag)
    {
        P = list.head;
        MaxProduct = list.head;
        for (i = 0; i < list.count; i++) // check which product ordered the most today
        {
            if (MaxProduct->OrdersCounter < P->OrdersCounter)
                MaxProduct = P;
            P = P->next;
        }
        fprintf(out, "The most popular dish today is %s! (it was ordered %d times)\n", MaxProduct->ProductName, MaxProduct->OrdersCounter);
    }
}


// Function that free the allocated memory of all the products in the kitchen
void FreeProductsList(ProductManager* list)
{
    Products* temp;
    if (list->head == NULL)
        return;
    while (list->head != NULL)
    {
        temp = list->head;
        list->head = list->head->next;
        free(temp->ProductName);
        free(temp);
    }
    list->tail = NULL;
    list->count = 0;
}


// Function that free the allocated memory of a specific table
void FreeTableList(TablesManager* list)
{
    Dish* temp;
    if (list->head == NULL)
        return;
    while (list->head != NULL)
    {
        temp = list->head;
        list->head = list->head->next;
        free(temp->ProductName);
        free(temp);
    }
    list->tail = NULL;
    list->count = 0;
}
