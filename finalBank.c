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
void menu(double *pBalance, FILE *pF, struct account bankUsers[], int totalUsers, int userIdx);
void printBalance(double *pBalance);
void depo(double *pBalance, FILE *pF);
void with(double *pBalance, FILE *pF);
void transfer(struct account bankUsers[], int totalUsers, int userIdx, FILE*pf);

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
        printf("Too many failed attempts goodbye!\n");
        return 0;
    }
    else if(userIdx == -2)
    {
        printf("Account not found\n");
        return 0;
    }

    double *pBalance = &bankUsers[userIdx].balance

     /*    fopen(filename, mode);
               MODES 
        w - Writes to a file 
        a - Appends new data to a file | adds to a new line at the end of the file
        r - Reads from a file
    */
    FILE *pF;//this creates a file and assigns it to pointer *pF 
    pF = fopen("Statement.txt","w");// this opens the Statement.txt file and writes the next line into it 
    fprintf(pF, "-----Welcome to your statement-----\n\n");// fprintf will print to the file instead of the terminal.  
    
    printf("\n\nHello, welcome to Gbank\n");
    menu(pBalance, pF, bankUsers, totalUsers, userIdx);//passing the POINTER to menu function 
    // when passing a pointer, you wont include the * because you are passing the actual address. When you receive a pointer, you must specify with a 
    //* because you are saying that you are passing a POINTER
    // you can use/change the actual value of the pointer by DEREFERENCING by putting a * before the name

    //this function will determine what the user wants to do and then call the next appropriate function 
void menu(double *pBalance, FILE *pF, struct account bankUsers[], int totalUsers, int userIdx)
{
    int choice;
    while(choice !=5){
    //print options
    printf("1. Deposit money\n");
    printf("2. Withdraw money\n");
    printf("3. Check Balance\n");
    printf("4. Transfer Money\n");
    printf("5. Exit\n\n");
    printf("Enter your choice:");
    
    //take and store choice
    scanf("%d", &choice);

    //call functions 
    switch(choice){
        case 1:
        depo(pBalance,pF);
        break;

        case 2:
        with(pBalance,pF);
        break;

        case 3:
        printBalance(pBalance);
        break;

        case 4:
        transfer(bankUsers, totalUsers, userIdx,pF);
        break;

        case 5:
        printf("Thank you!\n");
        break;

        default:
        printf("Invalid choice, try again\n\n");
        continue;
        }
    }
}
void printBalance(double *pBalance)
{
    printf("your balance is: $%.2lf \n",*pBalance); //dereference
}
void depo(double *pBalance, FILE *pF)
{
    //declare variable and store deposit amount
    double amount;
    printf("Enter deposit amount: $");
    scanf("%lf", &amount);

    //make sure deposit amount is positive
    if(amount <0){ 
        printf("\nInvalid amount, please try again\nEnter deposit amount: $");
        scanf("%lf", &amount);
    }

    //add deposit amount to balance
    *pBalance += amount;//dereference

    //send the deposit and new balance to statement.txt file
    fprintf(pF, "Deposited $%.2lf\n", amount);
    fprintf(pF, "   Balance: $%.2lf\n", *pBalance);
    //print success message and new balance to terminal
    printf("\nDeposit succesful\nNew balance: $%.2lf\n\n",*pBalance);
}
void with(double *pBalance, FILE *pF)
{
    //declare variable and store withdrawal amount
    double amount;
    printf("please enter withdrawal amount: $");
    scanf("%lf", &amount);

    //make sure withdrawal is less than balance
    while(amount > *pBalance){ 
        printf("\ninsufficient funds.\nEnter a new withdraw amount: $");
        scanf("%lf", &amount);
    }

    //this will check if the withdraw amount is divisible by 20. fmod(numerator,denominator)
    while(fmod(amount, 20.0) !=0){
        printf("\nYou can only withdraw in multiples of $20. \nEnter a new withdraw amount: $");
        scanf("%lf", &amount);
    }
    //withdraw money from balance
    *pBalance -= amount;

    //send the deposit and new balance to Statement.txt file
    fprintf(pF, "Withdrew $%.2lf\n", amount);
    fprintf(pF, "   Balance: $%.2lf\n", *pBalance);
    //print success message and new balance to terminal
    printf("\nWithdrawal successful\nNew balance $%.2lf \n\n", *pBalance);

}
void transfer(struct account bankUsers[], int totalUsers, int userIdx,FILE *pF){
    // ask where the money is going
    //take money from current users account 
    // delete from current account and give to new account

    int tempAccount;
    double tAmount=0.0;
    printf("\n\n----------TRANSFERING----------\n");
    printf("Please enter the account number you want to transfer to: ");
    scanf("%d",&tempAccount);
      // Prevent transferring to the same account
        if (tempAccount == bankUsers[userIdx].id) {
                printf("\nIneligible Account: Cannot transfer to the same account.\n");
                return; // Exit the function early
        }

    for(int i=0;  i < totalUsers; i++)
    {
        if(tempAccount == bankUsers[i].id)
        {
            //find account and collect transfer amount
            printf("Account %d found!\n", bankUsers[i].id);
            printf("Please enter the amount you wish to transfer: $");
            scanf("%lf", &tAmount);

            //make sure transfer amount is less than balance
            while(tAmount > bankUsers[userIdx].balance){
                printf("\ninsufficient funds.\nEnter a new transfer amount: $");
                scanf("%lf", &tAmount);
            }
            
            //transfer logic
            bankUsers[userIdx].balance -= tAmount;
            bankUsers[i].balance += tAmount;

            //success message
            printf("\n-----Transfer complete!-----\n");
            printf("Your new balance: $%.2lf\n", bankUsers[userIdx].balance);
            printf("Account %d balance: $%.2lf\n\n", bankUsers[i].id, bankUsers[i].balance);

            //send details to Statement.txt
            fprintf(pF, "Transfered $%.2lf to account %d\n", tAmount, bankUsers[i].id);
            fprintf(pF, "   Balance $%.2lf\n",bankUsers[userIdx].balance);
        }   
    }
}
