#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Date {
	int day;
	int month;
	int year;
}Date;
typedef struct movie {
	char NameMovie[20];
	char Genre[15];
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

//TODO serch by name 

#define MaxNameMovie 30
#define MaxGenre 15

Node* createNode(movie* value)
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
	fp = fopen("movies.txt", "a");
	fprintf(fp, "%-3d %-30s %-15s\t %.2f$\t %-3d \t   %-2d/%-2d/%-4d    \t %d\n\n", mv.ID, mv.NameMovie, mv.Genre, mv.price,mv.LengthMovie, mv.ReleaseDate.day,mv.ReleaseDate.month,mv.ReleaseDate.year, mv.BeastSeller);
	fclose(fp);
	///
	fp = fopen("movies.bin", "ab");
	fwrite(&mv, sizeof(movie), 1, fp);
	fclose(fp);
}
//added movie to the end of the linked list
void addMovieToEnd(Node* listHead, movie* value)
{
	Node* newNode = createNode(value); // it's going to be the last element in the list

	while (listHead->next != NULL) // Finding the last element of the list (not the NULL).
		listHead = listHead->next;

	listHead->next = newNode;
	newNode->previous = listHead;

}
 //added movie to the start of the linked list
void addMovieToStart(Node** listHeadPtr, movie* value)
{
	Node* previousNodeHead = *listHeadPtr;
	Node* newNodeHead = createNode(value);
	newNodeHead->next = previousNodeHead;
	*listHeadPtr = newNodeHead;
}

//load movies from file
int play(Node** head)
{
	int count;
	// Open the file for reading
	FILE* fp = fopen("movies.bin", "rb");
	if (fp == NULL)
	{
		printf("Error: unable to open file for reading\n");
		return -1;
	}

	// Read in the first movie from the file
	movie mv;
	if (fread(&mv, sizeof(movie), 1, fp) == 1)
	{
		*head = createNode(&mv);
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
		printf("%d:  ID: %d, Movie name: %s, Genre: %s, Price: %.2lf$, Length: %d, Date: %d/%d/%d, Beast seller: %d \n-> ", count, currentNode->data.ID, currentNode->data.NameMovie, currentNode->data.Genre, currentNode->data.price, currentNode->data.LengthMovie, currentNode->data.ReleaseDate.day, currentNode->data.ReleaseDate.month, currentNode->data.ReleaseDate.year, currentNode->data.BeastSeller);
		currentNode = currentNode->next;
		count++;
	}
	printf("  NULL\n\n");
}
//initialize movie
void InitMovie(movie* mv,int ID)
{
	int temp;
	mv->ID = ID;
	printf("Enter the name of the movie: ");
	scanf("%[^\n]%*c", mv->NameMovie);
	////TODO fix run time failure but it still added to the file correctly
	/*if (strlen(mv->NameMovie) > MaxNameMovie)
	{
		printf("too long try again\n");
		return InitMovie(mv, ID);
		return;
	}*/
	printf("\n");
	printf("Enter the genre of the movie: ");
	scanf("%[^\n]%*c", mv->Genre);
	////TODO fix run time failure but it still added to the file correctly
	/*if (strlen(mv->Genre) > MaxGenre)
	{
		printf("too long try again\n");
		return InitMovie(mv, ID);
		return;
	}*/
	printf("\n");
	printf("Enter the Length of the movie in minutes: ");
	scanf("%d", &mv->LengthMovie);
	printf("\n");
	printf("Enter the price of the movie: $");
	scanf("%f", &mv->price);
	printf("\n");
	printf("Enter the Date of the movie (example: 2/1/2020): ");
	scanf("%d%*c%d%*c%d", &mv->ReleaseDate.day, &mv->ReleaseDate.month, &mv->ReleaseDate.year);
	printf("\n");
	printf("Enter if the movie is beast seller (1=True/0=False): ");
	scanf("%d", &mv->BeastSeller);
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
		if (remove("movies.txt") != 0) printf("movies.txt not exist\n");
		if (remove("movies.bin") != 0) printf("movies.bin not exist\n");
		printf("movie deleted completely and the files\n");
	}
	// is the last movie in the list
	else if (arrmovie->next == NULL) {
		Node* prev = arrmovie->previous;
		prev->next = NULL;
		free(arrmovie);
		if (remove("movies.txt") != 0) printf("movies.txt not exist\n");
		if (remove("movies.bin") != 0) printf("movies.bin not exist\n");
		arrmovie = *head;
		while (arrmovie != NULL) {
			AddMovieToFile(arrmovie->data);
			arrmovie = arrmovie->next;
		}
	}//TODO בעיה במצביע להתחלה
	// is the first movie one in the list
	else if (arrmovie->previous == NULL) {
		Node* next = arrmovie->next;
		next->previous = NULL;
		free(arrmovie);
		*head = next;
		if (remove("movies.txt") != 0) printf("movies.txt not exist\n");
		if (remove("movies.bin") != 0) printf("movies.bin not exist\n");
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
		if (remove("movies.txt") != 0) printf("movies.txt not exist\n");
		if (remove("movies.bin") != 0) printf("movies.bin not exist\n");
		arrmovie = *head;
		while (arrmovie != NULL) {
			AddMovieToFile(arrmovie->data);
			arrmovie = arrmovie->next;
		}
	}
}
// serch by price
void searchByPrice(Node* head, float minPrice, float maxPrice) {
	Node* current = head;

	int i = 1;
	printf("Here are all the movies that match your pricing range: \n");
	while (current != NULL) {
		if (current->data.price >= minPrice && current->data.price <= maxPrice) {
			printf("%d) ", i);
			printMovie(current->data);
			i += 1;
		}
		current = current->next;
	}
}

int main()
{
	Node* ActionMovie = NULL;
	
	movie mv;
	int x;
	int count = play(&ActionMovie);
	displayMovie(ActionMovie);
	scanf("%d", &x);
	DeleteMovie(x, &ActionMovie);
	displayMovie(ActionMovie);
	count++;
	//InitMovie(&mv, count);
	//if (ActionMovie == NULL)
	//	addMovieToStart(&ActionMovie, &mv);
	//else
	//	addMovieToEnd(ActionMovie, &mv);
	//AddMovieToFile(mv);
	//printf("Your movie are successfully added\n");
	//displayMovie(ActionMovie);
	//freeMovieList(ActionMovie);
	//gchjgcfjhfgjfg
	return 0;
}