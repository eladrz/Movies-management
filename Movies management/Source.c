#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>


//TODO serch by name 



   while (Is_Program_On) {
       if (Check_Register)
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
	FILE* fp = fopen(NameFileBIN, "rb");
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
        printf("%d:  ID: %d, Movie name: %s, Genre: %s, Price: %.2lf$, Length: %d, Date: %d/%d/%d, Beast seller: %s \n-> ", count, currentNode->data.ID, currentNode->data.NameMovie, currentNode->data.Genre, currentNode->data.price, currentNode->data.LengthMovie, currentNode->data.ReleaseDate.day, currentNode->data.ReleaseDate.month, currentNode->data.ReleaseDate.year, currentNode->data.BeastSeller ? "True" : "False");
		currentNode = currentNode->next;
		count++;
	}
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
       else
       {
           Check_Register = 0;
           printf("\n============> STACK MENU <============\n");
           printf("Choose your operation:\n\n");
           printf("To search a movie by name enter 1\n"); // all lvls
           printf("To search a movie by genre enter 2\n"); // all lvls
           printf("To search a movie by price range enter 3\n"); // all lvls
           printf("To see all the best sellers movies enter 4\n"); // all lvls
           printf("To search for a movie by release date enter 5\n"); // all lvls
           printf("To display all movies available enter 6\n"); //all lvls
           printf("to switch accounts enter 9\n"); //all lvls
           if(userlvl>=2)
                printf("To enter employees menu enter 7\n");//lvl 2 and above
           if(userlvl>=3)
                printf("To enter manager's menu enter 0\n"); //lvl 3 only
           printf("To exit enter 8\n"); //all levels
           scanf("%d%*c", &choice1);

           switch (choice1) {
           case 1:
               printf("What movie would you like to watch? \n");
               scanf("%[^\n]%*c", keyToFind);
               searchMovieByName(keyToFind);
               break;

           case 2:
               printf("What genre of movies would you like to watch? \n");
               scanf("%[^\n]%*c", keyToFind);
               searchByGenre(keyToFind);
               break;

           case 3:
               printf("Type in the max price you will like to see: \n");
               scanf("%d%*c", &num1);
               printf("Type in the min price you would like to see: \n");
               scanf("%d%*c", &num2);
               searchMovieByPrice(num2, num1);
               break;

           case 4:
               searchByBestSeller();
               break;

           case 5:
               printf("what year of release would you like to see movies from? \n");
               scanf("%d%*c", &num1);
               searchMovieByDate(num1);
               break;
           case 6:
               displayMovie(arrMovies);
               break;

           case 7:
               if (userlvl < 2) {
                   printf("Sorry, you don't have permission  (ג€²ג’`) \n\n");
                   choice2 = -1;
               }
	else {
                   printf("\n======>Employees menu<======\n");
                   printf("To add a new movie enter 1 \n");
                   printf("To delete a movie enter 2 \n");
                   printf("To edit a movie enter 3 \n");
                   printf("To create a new costumer account enter 4 \n");
                   printf("To return the regular menu enter 9 \n");
                   scanf("%d%*c", &choice2);
		}
	}
}
// serch by price
void searchByPrice(Node* head, float minPrice, float maxPrice) {
	Node* current = head;

		}
		current = current->next;
	}
}

int main()
{
           Is_Program_On = 0;
           printf("too many attmpts :( \n");
       }
   }
    freeMovieList(arrMovies);
    freeUsersList(arrUsers);
	return 0;
}