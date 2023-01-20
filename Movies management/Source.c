#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"movies.h"
#include"Users.h"


// MADE BY:
//  elad reani: 207688532
// yarin reani: 208597450
// marcus eyal: 316532910

void CheckString(char* input, int size) {
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
int check_and_get_int() {
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
float check_and_get_float() {
    float input;
    while (1) {
        if (scanf("%f", &input) == 1) {
            return input;
        }
        else {
            printf("Invalid input, please try again.\n");
            scanf("%*[^\n]%*c"); // Clear input buffer
        }
    }
}



int main() {
    char cUser[MaxUserName], cPassword[MaxPassword], keyToFind[MaxNameMovie];
    int choice1, choice2, userlvl, num1, num2,count=0, mID, uID, Is_Program_On = 1, Check_Register = 1;
    float min, max;
    movie mv;
    Node* arrMovies = NULL;
    mID = playMovie(&arrMovies);
    user u;
    usersList* arrUsers = NULL;
    uID = playUsers(&arrUsers);
    uID++;

   while (Is_Program_On) {
       if (Check_Register)
       {
           printf("Hello! Please Enter your username: \n");
           CheckString(cUser, 10);
           printf("Enter Password:(%d chars) \n", MaxPassword - 1);
           CheckString(cPassword, 8);
           userlvl = lvlOfUser(cUser, cPassword);
           count++;
       }
       if (userlvl == 0)
       {
           printf("Sorry, User not found\n\n");
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

           if(userlvl>=2)
                printf("To enter employees menu enter 7\n");//lvl 2 and above

           if(userlvl>=3)
                printf("To enter manager's menu enter 8\n"); //lvl 3 only

           printf("to switch accounts enter 9\n"); //all lvls

           if(userlvl==1)
                printf("To buy a movie enter 10\n"); //all lvl

           printf("To exit enter 0\n"); //all levels
           choice1 = check_and_get_int();

           switch (choice1) {
           case 1:
               printf("What movie would you like to watch? \n");
               CheckString(keyToFind, 30);
               searchMovieByName(keyToFind);
               break;

           case 2:
               printf("What genre of movies would you like to watch? \n");
               CheckString(keyToFind, 8);
               searchByGenre(keyToFind);
               break;

           case 3:
               printf("Type in the max price you will like to see: \n");
               max = check_and_get_float();
               printf("Type in the min price you would like to see: \n");
               min = check_and_get_float();
               searchMovieByPrice(min, max);
               break;

           case 4:
               searchByBestSeller();
               break;

           case 5:
               printf("what year of release would you like to see movies from? \n");
               num1 = check_and_get_int();
               searchMovieByDate(num1);
               break;

           case 6:
               displayMovie(arrMovies);
               break;

           case 7:
               if (userlvl < 2) {
                   printf("Sorry, you don't have permission  :( \n\n");
                   choice2 = -1;
               }
               else {
                   printf("\n======>Employees menu<======\n");
                   printf("To add a new movie enter 1 \n");
                   printf("To delete a movie enter 2 \n");
                   printf("To edit a movie enter 3 \n");
                   printf("To create a new costumer account enter 4 \n");
                   printf("To return the regular menu enter 9 \n");
                   choice2 = check_and_get_int();
               }

               switch (choice2) {
               case 1:
                   mID += 1;
                   initMovie(&mv, mID);
                   if (arrMovies == NULL)
                       addMovieToStart(&arrMovies, &mv);

                   else
                       addMovieToEnd(arrMovies, &mv);
                   AddMovieToFile(mv);
                   printf("The movie has been added successfully\n");
                   break;
               case 2:
                   printf("What is the ID of the movie you would like to delete?\n");
                   num1 = check_and_get_int();
                   DeleteMovie(num1, &arrMovies);
                   printf("The movie has been deleted successfully\n");
                   break;
               case 3:
                   printf("What is the ID of the movie you would like to edit?\n");
                   num1 = check_and_get_int();
                   EditMovie(&arrMovies, num1);
                   printf("Your movie has been edited successfully\n");
                   break;
               case 4:
                   uID++;
                   initUser(&u, uID, 1);
                   if (arrUsers == NULL)
                       addUserToStart(&arrUsers, &u);
                   else
                       addUserToEnd(arrUsers, &u);
                   addNewUserToF(u);
                   printf("Costumer has been added successfully\n");
                   break;
               case -1:
                   break;
               default:
                   printf("Sorry, I don't have that option, please try again\n");
                   break;
               }
               break;

           case 8:
               if (userlvl < 3) {
                   printf("Sorry, you don't have permission :(\n\n");
                   choice2 = -1;
               }
               else {
                   printf("\n======>Managers menu<======\n");
                   printf("To add a new account enter 1\n");
                   printf("To delete an account enter 2\n");
                   printf("To change Clearance of an account enter 3\n");
                   printf("To display all Employees enter 4\n");
                   printf("To return the regular menu enter 9 \n");
                   choice2 = check_and_get_int();

               }
               switch (choice2) {
               case 1:
                   printf("\n");
                   printf("If the new account is a costumer, enter 1\n");
                   printf("If the new account is an employee, enter 2\n");
                   printf("If the new account is a manager, enter 3\n");
                   num1 = check_and_get_int();
                   uID++;
                   initUser(&u, uID, num1);
                   if (arrUsers == NULL)
                       addUserToStart(&arrUsers, &u);

                   else
                       addUserToEnd(arrUsers, &u);

                   addNewUserToF(u);
                   printf("Yay! The new user has been added successfully\n");
                   break;

               case 2:
                   printf("What is the ID of the user you'd like to delete?\n");
                   num1 = check_and_get_int();
                   deleteUser(num1, &arrUsers);
                   printf("The user has been deleted successfully\n");
                   break;

               case 3:
                   printf("What is the ID of the user you'd like to edit?\n");
                   num1 = check_and_get_int();
                   EditUser(&arrUsers, num1);
                   break;

               case 4:
                   displayUser(arrUsers);
                   break;
               default:
               case -1:
                   break;
                   printf("Sorry, I don't have that option, please try again\n");
                   break;
               }
               break;

           case 0:
               Is_Program_On = 0;
               break;
           case 9:
               Check_Register = 1;
               num2 = 0;
               break;
           case 10:
               if (userlvl == 1)
               {
                   displayMovie(arrMovies);
                   BuyMovie();
               }
               break;
           case -1:
               break;
           default:
               printf("Sorry, I don't have that option, please try again\n");
               break;
           }

       }
       if (count == 3)
       {
           Is_Program_On = 0;
           printf("too many attmpts :( \n");
       }
   }
    freeMovieList(arrMovies);
    freeUsersList(arrUsers);
    return 0;
}