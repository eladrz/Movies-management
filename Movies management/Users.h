#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MaxFullName 25
#define MaxUserName 10
#define MaxPassword 8
#define NameUsersFileTXT "Users.txt"
#define NameUsersFileBIN "Users.bin"
typedef struct user {
    int UID;
    char FullName[MaxFullName];
    char userName[MaxUserName];
    char password[MaxPassword];
    int level;

}user;
typedef struct usersList {
    user data;
    struct userList* next;
    struct userList* previous;
}usersList;

void CheckStringUser(char* input, int size) {
    int result;
    char* pos;
    while (true) {
        fgets(input, size, stdin);
        if ((pos = strchr(input, '\n')) != NULL)
            *pos = '\0';
        else {
            printf("Input too long with more than %d characters. try again please\n", size - 1);
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        result = sscanf(input, "%[^\n]", input);
        if (result == 1)
            break;
        printf("Invalid input, please try again.\n");
    }
}

usersList* createUserNode(user* value)
{
    usersList* newNode = (usersList*)malloc(sizeof(usersList));
    if (newNode == NULL)
    {
        printf("user creation did not succeed..! Try again..\n");
        return NULL;
    }
    newNode->next = NULL;
    newNode->previous = NULL;
    newNode->data.level = value->level;
    newNode->data.UID = value->UID;
    strcpy(newNode->data.userName, value->userName);
    strcpy(newNode->data.password, value->password);
    strcpy(newNode->data.FullName, value->FullName);
    return newNode;
}
//Free the users list
void freeUsersList(usersList* listHead)
{
    usersList* nodeToFree = listHead;
    while (nodeToFree != NULL) // while not reached the end of the List
    {
        listHead = listHead->next;
        free(nodeToFree);
        nodeToFree = listHead;
    }
}
//adds user to end of the users list
void addUserToEnd(usersList* listHead, user* value) {
    usersList* newNode = createUserNode(value); // it's going to be the last element in the list

    while (listHead->next != NULL) // Finding the last element of the list (not the NULL).
        listHead = listHead->next;

    listHead->next = newNode;
    newNode->previous = listHead;
}
//adds user to start of users list
void addUserToStart(usersList** listHeadPtr, user* value)
{
    usersList* previousNodeHead = *listHeadPtr;
    usersList* newNodeHead = createUserNode(value);
    newNodeHead->next = previousNodeHead;
    *listHeadPtr = newNodeHead;
}
//loads users from file
int playUsers(usersList** head) {
    int count;
    // Open the file for reading
    FILE* fp = fopen(NameUsersFileBIN, "rb");
    if (fp == NULL)
    {
        printf("Error: unable to open %s file for reading\n", NameUsersFileBIN);
        return -1;
    }

    // Read in the first movie from the file
    user u;
    if (fread(&u, sizeof(user), 1, fp) == 1)
    {
        *head = createUserNode(&u);
    }
    else
    {
        printf("Error: unable to read first user from the file\n");
        return -1;
    }

    // Read in the remaining movies from the file and add them to the end of the linked list
    while (fread(&u, sizeof(user), 1, fp) == 1)
    {
        addUserToEnd(*head, &u);
    }
    usersList* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    count = temp->data.UID;
    fclose(fp);
    return count;
}
//search movie by price range
void addNewUserToF(user u) {
    FILE* fp = NULL;
    fp = fopen(NameUsersFileTXT, "a");
    fprintf(fp, "ID: %-4d  Full Name: %-30s Username: %-15s Password: %-12s Level: %-2d\n\n", u.UID, u.FullName, u.userName, u.password, u.level);
    fclose(fp);

    fp = fopen(NameUsersFileBIN, "ab");
    fwrite(&u, sizeof(user), 1, fp);
    fclose(fp);
}
//initialize user
void initUser(user* u, int userID, int level) {
    user Utemp;
    bool flag;
    do {
        flag = 0;
        u->UID = userID;
        u->level = level;
        printf("Enter full name:\n");
        CheckStringUser(u->FullName, 25);
        printf("Enter username(no spaces): \n");
        CheckStringUser(u->userName, 10);
        printf("Enter password(7 chars): \n");
        CheckStringUser(u->password, 8);
        FILE* fp = fopen(NameUsersFileBIN, "rb");
        if (fp == NULL)
            return;
        while (fread(&Utemp, sizeof(user), 1, fp) == 1)
        {
            if (strcmp(Utemp.userName, u->userName) == 0)
            {
                printf("User name is aready exist try again\n");
                flag = 1;
            }
        }
    } while (flag);
}
//returns the lvl of a user from the users file
int lvlOfUser(char* uName, char* uPass) {
    user u;
    FILE* fp = fopen(NameUsersFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening %s file.\n", NameUsersFileBIN);
        return 0;
    }
    while (fread(&u, sizeof(user), 1, fp) == 1)
    {
        if (strcmp(u.userName, uName) == 0 && strcmp(u.password, uPass) == 0) {
            printf("User found\n");
            fclose(fp);
            return u.level;
        }
    }
    fclose(fp);
    return 0;
}
//deletes a user
void deleteUser(int ID, usersList** head) {
    if (*head == NULL) {
        printf("cant find the file\n");
        return;
    }

    usersList* arrUsers = *head;
    while (arrUsers != NULL && arrUsers->data.UID != ID)
        arrUsers = arrUsers->next;

    // movie not found
    if (arrUsers == NULL) {
        printf("User not found\n");
        return;
    }

    // is the last user
    if (arrUsers->next == NULL && arrUsers->previous == NULL) {
        free(arrUsers);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        printf("User deleted completely from the files\n");
        *head = NULL;
    }
    // is the last user in the list
    else if (arrUsers->next == NULL) {
        usersList* prev = arrUsers->previous;
        prev->next = NULL;
        free(arrUsers);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrUsers = *head;
        while (arrUsers != NULL) {
            addNewUserToF(arrUsers->data);
            arrUsers = arrUsers->next;
        }
    }
    // is the first user is in the list
    else if (arrUsers->previous == NULL) {
        usersList* next = arrUsers->next;
        next->previous = NULL;
        free(arrUsers);
        *head = next;
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrUsers = *head;
        while (arrUsers != NULL) {
            addNewUserToF(arrUsers->data);
            arrUsers = arrUsers->next;
        }
    }
    // user is in between
    else {
        usersList* prev = arrUsers->previous;
        usersList* next = arrUsers->next;
        prev->next = next;
        next->previous = prev;
        free(arrUsers);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrUsers = *head;
        while (arrUsers != NULL) {
            addNewUserToF(arrUsers->data);
            arrUsers = arrUsers->next;
        }
    }
}
//displays the users ditails
void displayUser(usersList* listHead)
{
    if (listHead == NULL)
    {
        printf("Cant find the file\n");
        return;
    }
    usersList* currentNode = listHead;
    int count = 0;
    while (currentNode != NULL)
    {
        printf("Id: %d Full Name: %s Username: %s Password: %s Level: %d\n", currentNode->data.UID, currentNode->data.FullName, currentNode->data.userName, currentNode->data.password, currentNode->data.level);
        currentNode = currentNode->next;
        count++;
    }
}
//edit user
void EditUser(usersList** head, int ID)
{
    user u;
    if (*head == NULL) {
        printf("cant find the file\n");
        return;
    }

    usersList* arrusers = *head;
    while (arrusers != NULL && arrusers->data.UID != ID)
        arrusers = arrusers->next;

    // user not found
    if (arrusers == NULL) {
        printf("user not found\n");
        return;
    }

    // is the last user
    if (arrusers->next == NULL && arrusers->previous == NULL) {
        printf("User found!\n");
        initUser(&u, arrusers->data.UID, arrusers->data.level);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        free(arrusers);
        *head = createUserNode(&u);
        addNewUserToF(u);
        printf("User edit completely and the files\n");
    }
    // is the last user in the list
    else if (arrusers->next == NULL) {
        printf("User found!\n");
        initUser(&u, arrusers->data.UID, arrusers->data.level);
        usersList* prev = arrusers->previous;
        usersList* curr = createUserNode(&u);
        prev->next = curr;
        curr->next = NULL;
        curr->previous = prev;
        free(arrusers);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrusers = *head;
        while (arrusers != NULL) {
            addNewUserToF(arrusers->data);
            arrusers = arrusers->next;
        }
        printf("User edit completely and the files\n");

    }
    // is the first user in the list
    else if (arrusers->previous == NULL) {
        printf("User found!\n");
        initUser(&u, arrusers->data.UID, arrusers->data.level);
        usersList* next = arrusers->next;
        usersList* curr = createUserNode(&u);
        next->previous = curr;
        curr->next = next;
        curr->previous = NULL;
        free(arrusers);
        *head = curr;
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrusers = *head;
        while (arrusers != NULL) {
            addNewUserToF(arrusers->data);
            arrusers = arrusers->next;
        }
        printf("User edit completely and the files\n");

    }

    else {
        printf("User found!\n");
        initUser(&u, arrusers->data.UID, arrusers->data.level);
        usersList* prev = arrusers->previous;
        usersList* next = arrusers->next;
        usersList* curr = createUserNode(&u);;
        prev->next = curr;
        next->previous = curr;
        curr->next = next;
        curr->previous = prev;

        free(arrusers);
        if (remove(NameUsersFileTXT) != 0) printf("%s not exist\n", NameUsersFileTXT);
        if (remove(NameUsersFileBIN) != 0) printf("%s not exist\n", NameUsersFileBIN);
        arrusers = *head;
        while (arrusers != NULL) {
            addNewUserToF(arrusers->data);
            arrusers = arrusers->next;
        }
        printf("User edit completely and the files\n");
    }
}
