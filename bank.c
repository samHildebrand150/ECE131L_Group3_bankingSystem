#include <stdio.h>
#include <math.h>

struct account // this creates the structure of an account :)
{
    int id;
    int pin;
    double balance;
};
//prototypes for functions
int auth(struct account bankUsers[], int totalUsers);
int main()
{
    //this creates 3 accounts that each have an ID, PIN, Balance.. all in that order
    struct account bankUsers[3] ={
        {101, 1234, 100.00},
        {102, 4321, 200.00},
        {103, 1111, 300.00}
    };

    int totalUsers = 3;
   
    //this will tell you what user logged in through the auth() function
    int userIdx = auth(bankUsers, totalUsers);

    // this is a safegaurd to end program if user fails to log in
    if(userIdx == -1)
    {
        printf("too many failed attempts goodbye!\n");
        return 0;
    }

    double *pBalance = &bankUsers[userIdx].balance;
