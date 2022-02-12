//  HW 3 - Semester 2 2020 - Grade 100
//  Created by Tareez Ghandour 
//  Copyright Tareez Ghandour © All rights reserved.
//  Header File

#ifndef _Header
#define _Header

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Products 
{
    char* ProductName;
    float Price;
    int Quantity;
    int OrdersCounter;
    struct Products* next;
}Products;

typedef struct ProductManager
{
    Products* head;
    Products* tail;
    int count;
}ProductManager;

typedef struct Dish
{
    char* ProductName;
    float Price;
    int Quantity;
    struct Dish* next;
    struct Dish* prev;
}Dish;

typedef struct TablesManager
{
    Dish* head;
    Dish* tail;
    float TotalPrice;
    int count;
}TablesManager;

// Function Declarations

void CreateProducts(ProductManager* list, FILE* in, FILE* out, FILE* instructions);
void AddItems(ProductManager* list, char* ProductName, int Quantity, FILE* out);
void OrderItem(ProductManager* list, TablesManager* Tables, int TableNumber, int NumOfTables, char* ProductName, int Quantity, FILE* out, FILE* instructions);
void RemoveItem(ProductManager* list, TablesManager* Tables, int TableNumber, char* ProductName, int Quantity, FILE* out);
void RemoveTable(ProductManager list, TablesManager* Tables, int TableNumber, int NumOfTables, FILE* out);
void FreeProductsList(ProductManager* list);
void FreeTableList(TablesManager* list);

#endif
