#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MaxNameMovie 20
#define MaxGenre 15
#define NameMovieFileTXT "movies.txt"
#define NameMovieFileBIN "movies.bin"
typedef struct Date {
    int day;
    int month;
    int year;
}Date;
typedef struct movie {
    char NameMovie[MaxNameMovie];
    char Genre[MaxGenre];
    int LengthMovie;
    int ID;
    float price;
    Date ReleaseDate;
    bool BeastSeller;
}movie;
typedef struct node
{
    movie data;
    struct node* next;
    struct node* previous;
}Node;
typedef struct PaymentDetails {
    char cardNumber[16];
    char CVV[3];
    int expiration_month;
    int expiration_year;
}PaymentDetails;


void CheckStringMovie(char* input, int size) {
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
int checkIntMovie() {
    int input;
    while (1) {
        if (scanf("%d%*c", &input) == 1 && input == (int)input) {
            return input;
        }
        else {
            printf("Invalid input, please try again.\n");
            scanf("%*[^\n]%*c"); // Clear input buffer
        }
    }
}
float checkfloatPrice() {
    float input;
    while (1) {
        if (scanf("%f%*c", &input) == 1) {
            return input;
        }
        else {
            printf("Invalid input, please try again.\n");
            scanf("%*[^\n]%*c"); // Clear input buffer
        }
    }
}
void get_date(int* day, int* month, int* year) {
    char date[20];
    while (1) {
        printf("Enter the date of the movie (example: 2/1/2020): ");
        scanf("%[^\n]%*c", date);
        if (sscanf(date, "%d/%d/%d", day, month, year) == 3) {
            return;
        }
        else {
            printf("Invalid date format, please try again.\n");
        }
    }
}
void get_month_year(int* month, int* year) {
    char date[20];
    while (1) {
        scanf("%[^\n]%*c", date);
        if (sscanf(date, "%d/%d", month, year) == 2) {
            return;
        }
        else {
            printf("Invalid date format, please try again.\n");
        }
    }
}
int CheckBool() {
    int input;
    while (1) {
        if (scanf("%d%*c", &input) == 1 && (input == 0 || input == 1)) {
            return input;
        }
        else {
            printf("Invalid input, please enter a number that is either 0 or 1.\n");
            scanf("%*[^\n]%*c"); // Clear input buffer
        }
    }
}



//creates a new node in the movies list
Node* createMovieNode(movie* value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Node creation did not succeed..! Try again..\n");
        return NULL;
    }
    newNode->data.price = value->price;
    newNode->data.LengthMovie = value->LengthMovie;
    newNode->data.ID = value->ID;
    // copy the Date
    newNode->data.ReleaseDate.day = value->ReleaseDate.day;
    newNode->data.ReleaseDate.month = value->ReleaseDate.month;
    newNode->data.ReleaseDate.year = value->ReleaseDate.year;
    //
    newNode->data.BeastSeller = value->BeastSeller;
    strcpy(newNode->data.Genre, value->Genre);
    strcpy(newNode->data.NameMovie, value->NameMovie);
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}
//free the liked list
void freeMovieList(Node* listHead)
{
    Node* nodeToFree = listHead;
    while (nodeToFree != NULL) // while not reached the end of the List
    {
        listHead = listHead->next;
        free(nodeToFree);
        nodeToFree = listHead;
    }
}
// added the movie to the text and binary file
void AddMovieToFile(movie mv)
{
    FILE* fp = NULL;
    fp = fopen(NameMovieFileTXT, "a");
    fprintf(fp, "%-3d %-30s %-15s\t %.2f$\t %-3d \t   %-2d/%-2d/%-4d    \t %s\n\n", mv.ID, mv.NameMovie, mv.Genre, mv.price, mv.LengthMovie, mv.ReleaseDate.day, mv.ReleaseDate.month, mv.ReleaseDate.year, mv.BeastSeller ? "True" : "False");
    fclose(fp);
    ///
    fp = fopen(NameMovieFileBIN, "ab");
    fwrite(&mv, sizeof(movie), 1, fp);
    fclose(fp);
}
//added movie to the end of the linked list
void addMovieToEnd(Node* listHead, movie* value)
{
    Node* newNode = createMovieNode(value); // it's going to be the last element in the list

    while (listHead->next != NULL) // Finding the last element of the list (not the NULL).
        listHead = listHead->next;

    listHead->next = newNode;
    newNode->previous = listHead;

}
//added movie to the start of the linked list
void addMovieToStart(Node** listHeadPtr, movie* value)
{
    Node* previousNodeHead = *listHeadPtr;
    Node* newNodeHead = createMovieNode(value);
    newNodeHead->next = previousNodeHead;
    *listHeadPtr = newNodeHead;
}
//load movies from file
int playMovie(Node** head)
{
    int count;
    // Open the file for reading
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL)
    {
        printf("Error: unable to open file for reading\n");
        return -1;
    }

    // Read in the first movie from the file
    movie mv;
    if (fread(&mv, sizeof(movie), 1, fp) == 1)
    {
        *head = createMovieNode(&mv);
    }
    else
    {
        printf("Error: unable to read first movie from file\n");
        return -1;
    }

    // Read in the remaining movies from the file and add them to the end of the linked list
    while (fread(&mv, sizeof(movie), 1, fp) == 1)
    {
        addMovieToEnd(*head, &mv);
    }
    Node* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    count = temp->data.ID;
    fclose(fp);
    return count;
}
//show the movies
void displayMovie(Node* listHead)
{
    if (listHead == NULL)
    {
        printf("Cant find the file\n");
        return;
    }
    Node* currentNode = listHead;
    int count = 0;
    while (currentNode != NULL)
    {
        printf("%-2d: ID: %-2d, Movie name: %-20s, Genre: %-8s, Price: %.2lf$, Length: %d, Date: %d/%d/%d, Beast seller: %s\n", count, currentNode->data.ID, currentNode->data.NameMovie, currentNode->data.Genre, currentNode->data.price, currentNode->data.LengthMovie, currentNode->data.ReleaseDate.day, currentNode->data.ReleaseDate.month, currentNode->data.ReleaseDate.year, currentNode->data.BeastSeller ? "True" : "False");
        currentNode = currentNode->next;
        count++;
    }
}
//display a single movie
void printMovie(movie mv) {
    printf("ID: %d, Movie name: %s, Genre: %s, Price: %.2lf$, Length: %d, Date: %d/%d/%d, Best seller: %s \n", mv.ID, mv.NameMovie, mv.Genre, mv.price, mv.LengthMovie, mv.ReleaseDate.day, mv.ReleaseDate.month, mv.ReleaseDate.year, mv.BeastSeller ? "True" : "False");
}
//initialize movie
void initMovie(movie* mv, int ID)
{
    int temp;
    mv->ID = ID;
    printf("Enter the name of the movie: ");
    CheckStringMovie(mv->NameMovie, MaxNameMovie);
    printf("\n");
    printf("Enter the genre of the movie: ");
    CheckStringMovie(mv->Genre, MaxGenre);
    printf("\n");
    printf("Enter the Length of the movie in minutes: ");
    mv->LengthMovie = checkIntMovie();
    printf("\n");
    printf("Enter the price of the movie: $");
    mv->price = checkfloatPrice();
    printf("\n");
    get_date(&mv->ReleaseDate.day, &mv->ReleaseDate.month, &mv->ReleaseDate.year);
    printf("\n");
    printf("Enter if the movie is beast seller (1=True/0=False): ");
    mv->BeastSeller = CheckBool();
    printf("\n");
}
// delete movie by ID
void DeleteMovie(int ID, Node** head) {
    if (*head == NULL) {
        printf("cant find the file\n");
        return;
    }

    Node* arrmovie = *head;
    while (arrmovie != NULL && arrmovie->data.ID != ID)
        arrmovie = arrmovie->next;

    // movie not found
    if (arrmovie == NULL) {
        printf("movie not found\n");
        return;
    }

    // is the last movie
    if (arrmovie->next == NULL && arrmovie->previous == NULL) {
        free(arrmovie);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        printf("movie deleted completely and the files\n");
        *head = NULL;
    }
    // is the last movie in the list
    else if (arrmovie->next == NULL) {
        Node* prev = arrmovie->previous;
        prev->next = NULL;
        free(arrmovie);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
    }
    // is the first movie one in the list
    else if (arrmovie->previous == NULL) {
        Node* next = arrmovie->next;
        next->previous = NULL;
        free(arrmovie);
        *head = next;
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
    }
    // movie is in between
    else {
        Node* prev = arrmovie->previous;
        Node* next = arrmovie->next;
        prev->next = next;
        next->previous = prev;
        free(arrmovie);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
    }
}
// search by price
void searchMovieByPrice(float minPrice, float maxPrice) {

    int i = 1;
    movie mv;
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&mv, sizeof(movie), 1, fp) == 1) {
        if (mv.price >= minPrice && mv.price <= maxPrice)
        {
            if (i == 1)
                printf("Here are all the movies that match your pricing range: \n");
            printf("%d) ", i++);
            printMovie(mv);
        }
    }
    if (i == 1)
        printf("sorry, movie not found\n");
    fclose(fp);

}
//search movie by name
int searchMovieByName(char* name[])
{
    movie mv;
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    while (fread(&mv, sizeof(movie), 1, fp) == 1) {
        if (strcmp(mv.NameMovie, name) == 0)
        {
            printMovie(mv);
            return 0;
        }
    }
    printf("sorry, movie not found\n");
    fclose(fp);
    return 1;
}
// search movie by beast seller
void searchByBestSeller()
{
    int i = 1;
    movie mv;
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&mv, sizeof(movie), 1, fp) == 1) {
        if (mv.BeastSeller)
        {
            if (i == 1)
                printf("Here are all the best seller movies: \n");
            printf("%d) ", i++);
            printMovie(mv);
        }
    }
    if (i == 1)
        printf("sorry, movie not found\n");
    fclose(fp);
}
// search movie by date
void searchMovieByDate(int Year) {
    int i = 1;
    movie mv;
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&mv, sizeof(movie), 1, fp) == 1) {
        if (mv.ReleaseDate.year == Year)
        {
            if (i == 1)
                printf("Here are all the movies that match to your release year choice: \n");
            printf("%d) ", i++);
            printMovie(mv);
        }
    }
    if (i == 1)
        printf("sorry, movie not found\n");
    fclose(fp);
}
//search movie by genre
void searchByGenre(char* genre[]) {
    int i = 1;
    movie mv;
    FILE* fp = fopen(NameMovieFileBIN, "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&mv, sizeof(movie), 1, fp) == 1) {
        if (strcmp(mv.Genre, genre) == 0)
        {
            if (i == 1)
                printf("Here are all the movies that match to your genre: \n");
            printf("%d) ", i++);
            printMovie(mv);
        }
    }
    if (i == 1)
        printf("sorry, movie not found\n");
    fclose(fp);
}
//edit movie
void EditMovie(Node** head, int ID)
{
    movie mv;
    if (*head == NULL) {
        printf("cant find the file\n");
        return;
    }

    Node* arrmovie = *head;
    while (arrmovie != NULL && arrmovie->data.ID != ID)
        arrmovie = arrmovie->next;

    // movie not found
    if (arrmovie == NULL) {
        printf("movie not found\n");
        return;
    }

    // is the last movie
    if (arrmovie->next == NULL && arrmovie->previous == NULL) {
        printf("movie found!\n");
        initMovie(&mv, arrmovie->data.ID);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        free(arrmovie);
        *head = createMovieNode(&mv);
        AddMovieToFile(mv);
        printf("movie Edit completely and the files\n");
    }
    // is the last movie in the list
    else if (arrmovie->next == NULL) {
        printf("movie found!\n");
        initMovie(&mv, arrmovie->data.ID);
        Node* prev = arrmovie->previous;
        Node* curr = createMovieNode(&mv);
        prev->next = curr;
        curr->next = NULL;
        curr->previous = prev;

        free(arrmovie);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
        printf("movie Edit completely and the files\n");

    }
    // is the first movie one in the list
    else if (arrmovie->previous == NULL) {
        printf("movie found!\n");
        initMovie(&mv, arrmovie->data.ID);
        Node* next = arrmovie->next;
        Node* curr = createMovieNode(&mv);
        next->previous = curr;
        curr->next = next;
        curr->previous = NULL;
        free(arrmovie);
        *head = curr;
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
        printf("movie Edit completely and the files\n");

    }

    else {
        printf("movie found!\n");
        initMovie(&mv, arrmovie->data.ID);
        Node* prev = arrmovie->previous;
        Node* next = arrmovie->next;
        Node* curr = createMovieNode(&mv);;
        prev->next = curr;
        next->previous = curr;
        curr->next = next;
        curr->previous = prev;

        free(arrmovie);
        if (remove(NameMovieFileTXT) != 0) printf("%s not exist\n", NameMovieFileTXT);
        if (remove(NameMovieFileBIN) != 0) printf("%s not exist\n", NameMovieFileBIN);
        arrmovie = *head;
        while (arrmovie != NULL) {
            AddMovieToFile(arrmovie->data);
            arrmovie = arrmovie->next;
        }
        printf("movie Edit completely and the files\n");
    }
}
// check if the card is right
bool ValidationCardNumber(const char* cardNumber) {
    // check if card number is 16 digits long
    if (strlen(cardNumber) != 16) {
        return false;
    }

    // check if card number only contains digits
    for (int i = 0; i < 16; i++) {
        if (!isdigit(cardNumber[i])) {
            return false;
        }
    }
}
// check if the cvv is right
bool ValidationCVV(char* CVV)
{
    int i;
    int CVVlen = strlen(CVV);
    if (CVVlen != 3) {
        // CVV must be 3 digits long
        return false;
    }
    for (i = 0; i < CVVlen; i++) {
        if (!isdigit(CVV[i])) {
            // CVV must be composed of digits only
            return false;
        }
    }
    return true;
}
// check if the month and the year is right
bool is_valid_month_year(int month, int year) {
    if (month < 1 || month > 12) {
        return false;
    }
    if (year < 2023) {
        return false;
    }
    return true;
}
// payment
void GetPaymentDetails(PaymentDetails* details)
{
    int i = 0;
    while (1)
    {

        printf("Enter card number: \n");
        do
        {
            CheckStringMovie(details->cardNumber, 18);
            if(i)
                printf("card number not currect please try again\n");
            i = 1;
        }
        while (!ValidationCardNumber(details->cardNumber));
        i = 0;
        printf("Enter CVV: \n");
        do
        {
            CheckStringMovie(details->CVV, 10);
            if (i)
                printf("CVV not currect please try again\n");
            i = 1;
        } while (!ValidationCVV(details->CVV));

        printf("Enter expiration date (MM/YYYY):\n");

        i = 0;
        get_month_year(&details->expiration_month, &details->expiration_year);
        if (!is_valid_month_year(details->expiration_month, details->expiration_year))
        {
            printf("expiration date is expire :(\n");
            return;
        }

        printf("the payment is succeed, enjoy!!!\n");
        break;
    }
}

//To buy a movie
void BuyMovie()
{
    PaymentDetails pay;
    int flag = 1;
    char name[MaxNameMovie];
    while (flag)
    {
        printf("please enter the name of the movie that you want to buy\n");
        CheckStringMovie(name, MaxNameMovie);
        if (searchMovieByName(name))
            return;
        printf("%s movie detected, are you want to continue to payment?(type 1 for yes, 0 for no)\n", name);
        flag = CheckBool();
        if (flag)
        {
            GetPaymentDetails(&pay);
            break;
        }
        flag = 1;
    }
}