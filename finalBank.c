#include <stdio.h>
#include <math.h>

struct account // this creates the structure of an account :)
{
    int id;
    int pin;
    double balance;
};

//prototypes for functions
void menu(double *pBalance, FILE *pF, struct account bankUsers[], int totalUsers, int userIdx);
void depo(double *pBalance, FILE *pF);
void with(double *pBalance, FILE *pF);
void printBalance(double *pBalance);
void transfer(struct account bankUsers[], int totalUsers, int userIdx, FILE*pf);
int auth(struct account bankUsers[], int totalUsers);
void printStatement(FILE *pF);
void saveAccounts(struct account bankUsers[], int totalUsers);
int loadAccounts(struct account bankUsers[]);


int main()
{
    //this creates 3 accounts that each have an ID, PIN, Balance.. all in that order    
    struct account bankUsers[3]; 

    //this will load the accounts found in file accounts.dat and if there is no file, it will revert to default values
    if(loadAccounts(bankUsers) == 0){
        printf("No save file found. Initializing default accounts...\n");
        bankUsers[0] = (struct account){101, 1234, 100.00};
        bankUsers[1] = (struct account){102, 4321, 200.00};
        bankUsers[2] = (struct account){103, 1111, 300.00};
    }
    else
    {
        printf("Account data loaded succesfully!\n");
    }

    int totalUsers = 3;
   
    //this will tell you what user logged in through the auth() function
    int userIdx = auth(bankUsers, totalUsers);

    // this is a safeguard to end program if user fails to log in
    if(userIdx == -1)
    {
        printf("too many failed attempts goodbye!\n");
        return 0;
    }

    double *pBalance = &bankUsers[userIdx].balance;

    /*    fopen(filename, mode);
                MODES 
        w - Writes to a file 
        a - Appends new data to a file | adds to a new line at the end of the file
        r - Reads from a file
    */
    FILE *pF;//this creates a file and assigns it to pointer *pF 
    pF = fopen("Statement.txt","w+");// this opens the Statement.txt file and writes the next line into it 
    fprintf(pF, "-----Welcome to your statement-----\n\n");// fprintf will print to the file instead of the terminal.  

    printf("\n\nHello welcome to Gbank\n");
    menu(pBalance, pF, bankUsers, totalUsers, userIdx);//passing the POINTER to menu function 
    // when passing a pointer, you wont include the * because you are passing the actual address. When you receive a pointer, you must specify with a 
    //* because you are saying that you are passing a POINTER
    // you can use/change the actual value of the pointer by DEREFERENCING by putting a * before the name
   

    saveAccounts(bankUsers, totalUsers);
    fclose(pF);
    return 0;
}

//this function will determine what the user wants to do and then call the next appropriate function 
void menu(double *pBalance, FILE *pF, struct account bankUsers[], int totalUsers, int userIdx)
{
    int choice=0;
    while(choice !=6){
    //print options
    printf("1. Deposit money\n");
    printf("2. Withdraw money\n");
    printf("3. Check Balance\n");
    printf("4. Transfer Money\n");
    printf("5. Print Statement\n");
    printf("6. Exit\n\n");
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
        printStatement(pF);
        break;

        case 6:
        printf("thank you!\n");
        break;

        default:
        printf("invalid choice, try again\n\n");
        continue;
        }
    }
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
    printf("Please enter withdrawal amount: $");
    scanf("%lf", &amount);

    //make sure withdrawal is less than balance and positive
    while(amount < 0){
        printf("\nInvalid amount, please try again\nEnter withdrawal amount: $");
        scanf("%lf", &amount);
    }
    while(amount > *pBalance){ 
        printf("\ninsufficient funds.\nEnter a new withdrawal amount: $");
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
void printBalance(double *pBalance)
{
    printf("your balance is: $%.2lf \n",*pBalance); //dereference
}

//this function will try to match the account number the user enters to one in the array
//after finding an account, we ask the user for a pin and see if it matches that specific accounts pin :)
int auth(struct account bankUsers[], int totalUsers)
{
    int tempID, tempPIN, attempts =0;

    printf("hello what is your account number\n");
    scanf("%d", &tempID);
    
    int found = 0;
    for(int i=0;  i < totalUsers; i++)
    {
        if(tempID == bankUsers[i].id)
        {
            found = 1;
            printf("Account found!\nPlease enter PIN:");
            scanf("%d", &tempPIN);

            while(attempts < 3)
            {
                attempts +=1;
                if(tempPIN == bankUsers[i].pin)
                {
                    return i;
                }
                printf("Incorrect PIN, please try again \n");
                scanf("%d", &tempPIN);
            }
            return -1;
        }    
    }
    return found ? -1 : -2;
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


    for(int i=0;  i < totalUsers; i++)
    {
        if(tempAccount == bankUsers[i].id)
        {
            //find account and collect transfer amount
            printf("Account %d found!\n", bankUsers[i].id);
            printf("Please enter the amount you wish to transfer: $");
            scanf("%lf", &tAmount);

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

void printStatement(FILE *pF)
{
    // 1. Flush the buffer to ensure the file on disk is up to date
    fflush(pF);

    // 2. REWIND: This moves the "cursor" back to the very first character of the file
    rewind(pF);

    // 3. Read and print until the end of the file
    char line[100];
    printf("\n\n-----Your Statement-----\n");
    while(fgets(line, sizeof(line), pF)){
        printf("%s", line);
    }
    printf("------------------------\n\n");

    // 4. IMPORTANT: Move the cursor back to the end so future 
    // transactions don't overwrite the beginning of your file!
    fseek(pF, 0, SEEK_END);
}

int loadAccounts(struct account bankUsers[]){

    //opens accounts.dat or returns null if accounts.dat doesnt exist
    FILE *pF = fopen("accounts.dat","rb");

    //if the accounts.dat doesnt exist, we need to revert to original values, so return 0
    if(pF == NULL)
    {
        return 0;
    }
    // checks how many bank users are in the accounts.dat file to make sure its not corrupted
    int count = fread(bankUsers, sizeof(struct account), 3, pF);
    //closes the accounts.dat file
    fclose(pF);
    return count;
}

void saveAccounts(struct account bankUsers[], int totalUsers)
{
    // opens the file or returns NULL if something goes wrong in the memory
    FILE *pF = fopen("accounts.dat","wb");

    //if the file DOES open, write 
    if(pF != NULL)
    {
        // fwrite will update accounts.dat which updates the balances of the different accounts
        fwrite(bankUsers, sizeof(struct account), totalUsers, pF);
        fclose(pF);    
    }else{
        printf("Error saving account data!\n");
    }
}
