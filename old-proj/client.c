#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
command line arguments:
file_name, server_ipaddress, port_number

argv[0] = file_path
argv[1] = server_ipaddress
argv[2] = port_number
*/

typedef struct
{
    char username[65];
    char password[65];
    char date_of_birth[11];
    char favourite_animal[21];
    char account_no[14];
    float balance;

} user_info;

typedef struct
{
    char username[65];
    char password[65];

} admin_info;

void get_username(char username[], const char prompt[]);
void get_password(char password[], const char prompt[]);
void get_date_of_birth(char date_of_birth[], const char prompt[]);
bool check_valid_date(int birth_date, int birth_month, int birth_year);
void get_favourite_animal(char favourite_animal[], const char prompt[]);
void get_account_no(char account_no[], const char prompt[]);
void package_command(char command[], const char seg1[], const char seg2[], const char seg3[], const char seg4[], const char seg5[], const char seg6[], const char seg7[]); 
void sha256(char input[], int mode, char salt[]);
void salter(char input[], char salt[]);

void main_menu(int sockfd);
void user_signup(int sockfd);
void user_input_signup(int sockfd, char username[], char password[], char salt[], char date_of_birth[], char favourite_animal[]);

user_info packet_userinfo_signup(char username[], char password[], char date_of_birth[], char favourite_animal[], char account_no[]);
void user_login(int sockfd);
void user_input_login(int sockfd, char username[]);
void forgot_password(int sockfd, char username[]);
void change_password(int sockfd, char username[]);



void homepage(int sockfd, char username[]);
user_info get_user_info(int sockfd, char username[]);
void withdraw(int sockfd, user_info user);
void deposit(int sockfd, user_info user);
void transfer(int sockfd, user_info user);
void check_balance(int sockfd, user_info user);
void change_account_details(int sockfd,user_info user);
void change_username(int sockfd, char username[]);
void change_password_homepage(int sockfd, char username[]);
void change_date_of_birth(int sockfd, char username[]);
void change_favourite_animal(int sockfd, char username[]);
bool check_password(int sockfd, char username[]);
void view_transactions(int sockfd, user_info user);
void decode_entry(char entry[]);
void download_trx(int sockfd, user_info user);


void admin_registration(int sockfd);
void admin_input_signup(int sockfd, char username[], char password[], char salt[]);
void admin_login(int sockfd);
void admin_input_login(int sockfd, char username[]);
void admin_homepage(int sockfd, char username[]);
void manage_users(int sockfd);
void reset_login_info(int sockfd, char account_no[]);
void view_logbook(int sockfd);
void view_database(int sockfd);


void error(const char *message);
void clear_screen(); 

int main(int argc, char *argv[])
{
    int sockfd, port_number, n;
    struct sockaddr_in server_address;
    struct hostent *server;        //stores information about the given host, ie, host name and ipv4 address

    if(argc < 3)
    {
        fprintf(stderr, "Usage %s hostname port\n", argv[0]);
        exit(1);
    }

    port_number = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening socket.");
    }

    server = gethostbyname(argv[1]);    //IP address of the server
    if(server == NULL)
    {
        fprintf(stderr, "Error! No such host.");
        exit(1);
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);

    if(connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        error("Connection failed.");
    }


    main_menu(sockfd);


    return 0;
}

void get_username(char username[], const char prompt[])
{
    bzero(username, 65);
    bool valid_username = false;
    while(valid_username == false)
    {
        
        char test_username[25];
        printf("%s", prompt);
        bzero(test_username, 25);
        fgets(test_username, 25, stdin);
        if (test_username[strlen(test_username) - 1] != '\n') 
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        test_username[strcspn(test_username, "\n")] = '\0'; 

        if(strcmp(test_username, "0") == 0)
        {
            strcpy(username, test_username);
            return;
        }
            

        int size = strlen(test_username);

        if(isdigit(test_username[0]))
        {
            clear_screen(); 
            printf("Username cannot start with a digit.\n");
            continue;
        }
        else if(size > 20)
        {
            clear_screen(); 
            printf("Username cannot be over 20 characters.\n");
            continue;
        }
        
        bool fault_found = false;
        for(int i = 0; i < size; i++)
        {
            if(!isdigit(test_username[i]) && !isalpha(test_username[i]))
            {
                clear_screen(); 
                fault_found = true;
                printf("Username cannot contain special characters or spaces.\n");
                break;
            } 
            else if(test_username[i] >= 'A' && test_username[i] <= 'Z') 
            {
                clear_screen(); 
                fault_found = true;
                printf("Username cannot contain uppercase characters.\n");
                break;
            }       
        }

        if(fault_found == true)
            continue;
        else
        {
            strcpy(username, test_username);
            valid_username = true;
        }      
    }
}
void get_password(char password[], const char prompt[])
{
    bzero(password, 65);
    bool valid_password = false;

    while(valid_password == false)
    {
        char test_password[25];
        printf("%s", prompt);
        bzero(test_password, 25);
        fgets(test_password, 25, stdin);
        if (test_password[strlen(test_password) - 1] != '\n') 
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        test_password[strcspn(test_password, "\n")] = '\0'; 

        if(strcmp(test_password, "0") == 0)
        {
            strcpy(password, test_password);
            return;
        }

        int size = strlen(test_password);

        if(size > 20)
        {
            clear_screen(); 
            printf("Password cannot be over 20 characters.\n");
            continue;
        }
        if(size < 7)
        {
            clear_screen(); 
            printf("Password needs to be atleast 8 characters long.\n");
            continue;
        }

        bool fault_found = false;
        for(int i = 0; i < size; i++)
        {
            if(test_password[0] == ' ' || test_password[size - 1] == ' ')
            {
                clear_screen(); 
                fault_found = true;
                printf("Password cannot contain spaces in the beginning or end.\n");
                break;
            }

            if(test_password[i] == '\t' || test_password[i] == '\n' || test_password[i] == '\0' || test_password[i] == '\b')
            {
                clear_screen(); 
                fault_found = true;
                printf("Password cannot contain control characters.\n");
                break;
            }    
        }

        if(fault_found == true)
            continue;
        else
        {
            strcpy(password, test_password);
            valid_password = true;
        }          
    }
}
void get_date_of_birth(char date_of_birth[],  const char prompt[])
{
    bzero(date_of_birth, 11);
    bool valid_dob = false;

    while(valid_dob == false)
    {
        char test_dob[15];
        printf("%s", prompt);
        bzero(test_dob, 15);
        fgets(test_dob, 15, stdin);
        if (test_dob[strlen(test_dob) - 1] != '\n') 
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        test_dob[strcspn(test_dob, "\n")] = '\0'; 

        if(strcmp(test_dob, "0") == 0)
        {
            strcpy(date_of_birth, test_dob);
            return;
        }

        int size = strlen(test_dob);

        if(size != 10)
        {
            clear_screen(); 
            printf("Invalid format.\n");
            continue;
        }

        bool fault_found = false;
        for(int i = 0; i < 10; i++)
        {
            if(test_dob[2] != '-' || test_dob[5] != '-')
            {
                clear_screen(); 
                fault_found = true;
                printf("Invalid format.\n");
                break;
            }

            if(test_dob[i] - '\0' == '\t' || test_dob[i] - '\0' == '\n' || test_dob[i] - '\0' == '\0' || test_dob[i] - '\0' == '\b')
            {
                clear_screen(); 
                fault_found = true;
                printf("Date of birth cannot contain control characters.\n");
                break;
            }            
        }

        int birth_date = 0, birth_month = 0, birth_year = 0;
        int age;

        int multiplier = 10;
        for(int i = 0; i <= 1; i++)
        {
            birth_date += (test_dob[i] - '0') * multiplier;
            multiplier /= 10;
        }

        multiplier = 10;
        for(int i = 3; i <= 4; i++)
        {
            birth_month += (test_dob[i] - '0') * multiplier;
            multiplier /= 10;
        }
        
        multiplier = 1000;
        for(int i = 6; i <= 9; i++)
        {
            birth_year += (test_dob[i] - '0') * multiplier;
            multiplier /= 10;
        }

        if(check_valid_date(birth_date, birth_month, birth_year) == false)
        {
            clear_screen(); 
            fault_found = true;
            printf("Invalid date/month.\n");
            continue;
        }

        time_t now;
        struct tm *current;
        time(&now); 
        current = localtime(&now);

        int current_year = current->tm_year + 1900;
        int current_month = current->tm_mon + 1;
        int current_day =current->tm_mday;


        if(current_month < birth_month)
        {
            age = current_year - birth_year - 1;
        }
        else if(current_month == birth_month && current_day <= birth_date)
        {
            age = current_year - birth_year - 1;
        }
        else
        {
            age = current_year - birth_year;
        }
        
        if(age < 0)
        {
            clear_screen(); 
            printf("Invalid date.\n");
            fault_found = true;
            continue;
        }
        else if(age < 18)
        {
            clear_screen(); 
            printf("You are not old enough to create an account.\n");
            fault_found = true;
            continue;
        }

        if(fault_found == true)
            continue;
        else
        {
            strcpy(date_of_birth, test_dob);
            valid_dob = true;
        }           
    }
}
bool check_valid_date(int birth_date, int birth_month, int birth_year)
{
    int max_date;

    bool leap_year = false;

    if((birth_year % 400) == 0)
        leap_year = true;
    if((birth_year % 4) == 0 && (birth_year % 100) != 0)
        leap_year = true;

    switch (birth_month)
    {
        case  4: 
        case  6:
        case  9:
        case 11: max_date = 30;
                 break;

        case  2: max_date = 28;
                 break;

        default: max_date = 31;
    }

    if(leap_year && birth_month == 2)
        max_date = 29;

    if(birth_date <= 0 || birth_date > max_date)
        return false;

    if(birth_month <= 0 || birth_month > 12)
        return false;

    return true;  
}
void get_favourite_animal(char favourite_animal[], const char prompt[])
{
    bzero(favourite_animal, 21);
    bool valid_animal = false;

    while(valid_animal == false)
    {
        char test_animal[25];
        printf("%s", prompt);
        bzero(test_animal, 25);
        fgets(test_animal, 25, stdin);
        if (test_animal[strlen(test_animal) - 1] != '\n') 
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        test_animal[strcspn(test_animal, "\n")] = '\0'; 

        if(strcmp(test_animal, "0") == 0)
        {
            strcpy(favourite_animal, test_animal);
            return;
        }

        int size = strlen(test_animal);
        
        bool fault_found = false;
        for(int i = 0; i < size; i++)
        {
            if(!isalpha(test_animal[i]) && test_animal[i] != '-')
            {
                clear_screen(); 
                fault_found = true;
                printf("Animal name cannot contain non-alphabetic characters or non-dashes.\n");
                break;
            }  
        }

        if(test_animal[0] == '-' || test_animal[size - 1] == '-')
        {
            clear_screen(); 
            printf("Animal name cannot contain dashes at the beginning or end.\n");
            fault_found = true;
        }

        if(fault_found == true)
            continue;
        else
        {
            strcpy(favourite_animal, test_animal);
            valid_animal = true;
        }    
    }
}
void get_account_no(char account_no[], const char prompt[])
{
    bool valid_account_no = false;

    while(valid_account_no == false)
    {
        char test_account_no[20];
        printf("%s", prompt);
        bzero(test_account_no, 20);
        fgets(test_account_no, 20, stdin);
        if (test_account_no[strlen(test_account_no) - 1] != '\n') 
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        test_account_no[strcspn(test_account_no, "\n")] = '\0';
        
        if(strcmp(test_account_no, "0") == 0)
        {
            strcpy(account_no, test_account_no);
            return;
        }

        int size = strlen(test_account_no);

        
        if(size =! 13)
        {
            clear_screen(); 
            printf("Account number must be 13 characters long.\n");
            continue;
        }
        
        bool fault_found = false;
        for(int i = 0; i < size; i++)
        {
            if(!isdigit(test_account_no[i]))
            {
                clear_screen(); 
                fault_found = true;
                printf("Account number must contain digits only.\n");
                break;
            } 
        }

        if(fault_found == true)
            continue;
        else
        {
            strcpy(account_no, test_account_no);
            valid_account_no = true;
        }      
    }
}
void package_command(char command[], const char seg1[], const char seg2[], const char seg3[], const char seg4[], const char seg5[], const char seg6[], const char seg7[])
{
    bzero(command, 256);
    strcpy(&command[0], seg1);         //max 18 - main cmd
    strcpy(&command[20], seg2);        //max 64 - username
    strcpy(&command[85], seg3);        //max 64 - pass
    strcpy(&command[150], seg4);       //max 10 - dob
    strcpy(&command[161], seg5);       //max 20 - fav ani
    strcpy(&command[182], seg6);       //max 13 - acc no
    strcpy(&command[196], seg7);       //       - salt
}

void sha256(char input[], int mode, char salt[]) 
{
    // mode 0 for usernames
    // mode 1 for server salts
    // mode 2 for given salts 

    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    // ensures that all the bytes after the 20th byte are nulls
    bzero(input + 21, 44);

    if(mode == 1)
        salter(input, salt);
    else if(mode == 2)
        strcat(input, salt);

    // hashes the entire 65-byte buffer 
    SHA256((unsigned char *)input, 65, hash);

    // overwrites input with the hex string representation 
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
    {
        sprintf(&input[i * 2], "%02x", hash[i]);
    }

    input[64] = '\0'; 
}
void salter(char input[], char salt[])
{
    char salt_letters[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    int size = strlen(salt_letters);
    srand(time(NULL));

    for(int i = 0; i < 16; i++)
    {
        int letter_index = rand() % size;
        salt[i] = salt_letters[letter_index];
    }

    strcat(input, salt);
}
void main_menu(int sockfd)
{
    clear_screen();
    printf("Welcome to TNI Bank!\n\n");

    while(1)
    {
        int choice;
        char command[21];

        printf("1. Login as existing user.\n"
               "2. Signup as new user.\n"
               "3. Register as an admin.\n"
               "4. Login as admin.\n"
               "Please select your desired option (1-3): ");
        scanf("%d", &choice);
        getchar();

        switch(choice)
        {
            case  1:    clear_screen(); 
                        user_login(sockfd);
                        break;

            case  2:    clear_screen(); 
                        user_signup(sockfd);
                        break;

            case  3:    clear_screen(); 
                        admin_registration(sockfd);
                        break;

            case  4:    clear_screen(); 
                        admin_login(sockfd);
                        break;

            default:    clear_screen(); 
                        printf("Invalid choice. Try again.\n");
        }


    }
}

void user_signup(int sockfd)
{
    char command[256];
    char username[65] = {0}, password[65] = {0}, salt[17] = {0}, date_of_birth[11], favourite_animal[21], account_no[14];
    bool response;


    user_input_signup(sockfd, username, password, salt, date_of_birth, favourite_animal);

    if(strcmp(date_of_birth, "0") == 0)
        return;

    package_command(command, "REQ-ACC-NO", "", "", "", "", "", "");

    int n = write(sockfd, command, sizeof(command));
    if(n < 0)
            error("Error on writing.");

    n = read(sockfd, &account_no, sizeof(account_no));
    if(n < 0)
            error("Error on writing.");

    //printf("Generated acc no\n");


    package_command(command, "SIGNUP", username, password, date_of_birth, favourite_animal, account_no, salt);

    write(sockfd, command, sizeof(command));

    read(sockfd, &response, sizeof(response));
    if(response == true)
    {
        clear_screen();
        printf("Account creation successful. Your account number is %s\n\n", account_no);
    }
    else
    {
        printf("Account creation failed.\n");
    }
}
void user_input_signup(int sockfd, char username[], char password[], char salt[], char date_of_birth[], char favourite_animal[])
{

    LABEL01:
    get_date_of_birth(date_of_birth, "Enter your date of birth [DD-MM-YYYY] (0 to go back): ");
    if(strcmp(date_of_birth, "0") == 0)
    {
        clear_screen();
        return;
    }

    LABEL02:
    bzero(username, 65);
    char command[256] = {0};
    bool response = false;

    while(response == false)
    {
        get_username(username, "Set username (0 to go back): ");
        if(strcmp(username, "0") == 0)
        {
            goto LABEL01;
        }
        sha256(username, 0, username);
        package_command(command, "USERNAME-CHECK", username, "", "", "", "", "");
        //printf("%s\n", command);
        //printf("%s\n", &command[20]);
        write(sockfd, &command, sizeof(command));
        read(sockfd, &response, sizeof(bool));
        if(response == false)
        {
            printf("Username taken already, try a different username.\n");
        }
    }

    //printf("Username validated.\n");

    

    LABEL03:
    get_password(password, "Set password (0 to go back): ");
    if(strcmp(password, "0") == 0)
    {
        clear_screen();
        goto LABEL02;
    }
    sha256(password, 1, salt);

    

    get_favourite_animal(favourite_animal, "What is your favourite animal? (0 to go back): ");
    if(strcmp(favourite_animal, "0") == 0)
    {
        clear_screen();
        goto LABEL03;
    }

    //printf("Salt: %s\n", salt);
}


void user_login(int sockfd)
{
    char username[65] = {0};
    
    user_input_login(sockfd, username);

    if(strcmp(username, "0") == 0)
        return;

    //("Login initiated.\n");
    clear_screen();
    homepage(sockfd, username);

}
void user_input_login(int sockfd, char username[])
{
    LABEL01:
    
    char command[256];
    bool response;
    get_username(username, "Enter username (0 to go back): ");
    if(strcmp(username, "0") == 0)
    {
        clear_screen();
        return;
    }
    
    sha256(username, 0, username);

    package_command(command, "USERNAME-CHECK", username, "", "", "", "", "");
    //printf("%s\n", command);
    //printf("%s\n", &command[20]);
    write(sockfd, &command, 256);
    read(sockfd, &response, sizeof(bool));
    
    
    if(response == true)
    {
        printf("Username does not exist.\n");
        goto LABEL01;
    }

    //printf("Username validated.\n");

    LABEL02:
    
    int choice;
    printf("1. Enter password.\n"
           "2. Forgot/Change password\n"
           "Please select an option (0 to go back): ");
    scanf("%d", &choice);
    getchar();


    LABEL03:

    char password[65] = {0};
    switch (choice)
    {
        case 1: {
                    get_password(password, "Enter password (0 to go back): ");
                    if(strcmp(password, "0") == 0)
                    {
                        clear_screen();
                        goto LABEL02;
                    }

                    package_command(command, "PASS-CHECK", username, "", "", "", "", "");
                    write(sockfd, &command, sizeof(command));
                    char salt[17];
                    read(sockfd, &salt, sizeof(salt));
                    sha256(password, 2, salt);
                    write(sockfd, &password, sizeof(password));
                    read(sockfd, &response, sizeof(bool));
                    if(response == false)
                    {
                        clear_screen();
                        printf("Password incorrect. Try again\n");
                        goto LABEL03;
                    }
                    else
                        break;
                }

        case 2: forgot_password(sockfd, username);
                goto LABEL02;
                break;

        case 0: goto LABEL01;

        default:clear_screen();
                printf("Invalid option\n");
                goto LABEL02;
    }
}
void forgot_password(int sockfd, char username[])
{
    LABEL01:
    char command[256];
    bool response;
    char date_of_birth[21] = {0}, favourite_animal[21] ={0};
    get_date_of_birth(date_of_birth, "Enter your date of birth [DD-MM-YYYY] (0 to go back): ");
    if(strcmp(date_of_birth, "0") == 0)
    {
        clear_screen();
        return;
    }

    
    get_favourite_animal(favourite_animal, "Enter your favourite animal (0 to go back): ");
    if(strcmp(favourite_animal, "0") == 0)
    {
        clear_screen();
        goto LABEL01;
    }
    
    package_command(command, "FORGOT-PASS", username, date_of_birth, favourite_animal, "", "", "");

    write(sockfd, &command, sizeof(command));
    read(sockfd, &response, sizeof(bool));

    if(response == false)
    {
        clear_screen(); 
        printf("Date of birth and/or favourite animal does not match. Try again.\n");
        goto LABEL01;  
    }
        
    change_password(sockfd, username);     
    
}
void change_password(int sockfd, char username[])
{
    bool response;
    char command[256];
    char new_password[65] = {0};
    char salt[17] = {0};
    get_password(new_password, "Enter new password (0 to go back): ");
    if(strcmp(new_password, "0") == 0)
    {
        return;
    }

    sha256(new_password, 1, salt);

    package_command(command, "CHANGE-PASS", username, new_password, "", "", "", salt);
    write(sockfd, &command, sizeof(command));

    read(sockfd, &response, sizeof(response));
    clear_screen();

    if(response == true)
    {
        printf("Password updated.\n\n");
    }
    else
    {   
        printf("Password update failed. Client not found.\n\n");
    }
}



void homepage(int sockfd, char username[])
{
    printf("Welcome!\n");
    int choice;

    while(1)
    {
        user_info user = get_user_info(sockfd, username);
        printf("Account number: %s\n"
                "Balance: %.2f\n\n"
                "1. Withdraw money\n"
               "2. Deposit money\n"
               "3. Transfer to another account\n"
               "4. Update Balance\n"
               "5. Change Account Details\n"
               "6. View Transaction History\n"
               "7. Logout\n"
               "Please select an operation (1-7): ", user.account_no, user.balance);
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case  1:    withdraw(sockfd, user);
                        break;

            case  2:    deposit(sockfd, user);
                        break;

            case  3:    transfer(sockfd, user);
                        break;

            case  4:    check_balance(sockfd, user);
                        break;

            case  5:    change_account_details(sockfd, user);
                        break;

            case  6:    view_transactions(sockfd, user);
                        break;

            case  7:    clear_screen();
                        printf("Thank you for banking with us!\n\n");
                        char command[256];
                        package_command(command, "LOGOUT", username, "", "", "", "", "");
                        write(sockfd, command, 256);
                        //flush_socket(sockfd);
                        return;
        
            default:    clear_screen();
                        printf("Invalid choice.\n\n");
                        
        }

        
            
    }
}
user_info get_user_info(int sockfd, char username[])
{
    //flush_socket(sockfd);
    char command[256];
    package_command(command, "GET-USER-INFO", username, "", "", "", "", "");
    write(sockfd, command, 256);
    user_info user;
    read(sockfd, &user, sizeof(user_info));
    return user;
}
void withdraw(int sockfd, user_info user)
{
    //flush_socket(sockfd);
    clear_screen(); 
    char command[256];
    float withdraw_amount;
    while(1)
    {
        user = get_user_info(sockfd, user.username);
        printf("Current balance: Tk%.2f\n", user.balance);
        printf("Enter amount to withdraw (0 to go back): ");
        if(scanf("%f", &withdraw_amount) == 0)
        {
            getchar();
            printf("Invalid amount. Try again.\n");
            continue;
        }
        getchar();
        if(withdraw_amount == 0)
        {
            clear_screen();
            return;
        }

        user = get_user_info(sockfd, user.username);

        if(user.balance >= withdraw_amount)
        {
            
            package_command(command, "WITHDRAW", user.username, "", "", "", "", "");
            write(sockfd, &command, sizeof(command));
            write(sockfd, &withdraw_amount, sizeof(float));
            bool response;
            read(sockfd, &response, sizeof(response));

            if(response == true)
            {
                clear_screen();
                printf("Withdrawal successful, returning to homepage\n\n");
                return;
            }
            else
            {
                clear_screen();
                printf("Invalid amount. Try again.\n");
            }
            
        }
        else
        {
            clear_screen();
            printf("Insufficient balance.\n");
        }         
    }
}
void deposit(int sockfd, user_info user)
{
    //flush_socket(sockfd);
    clear_screen(); 
    char command[256];

    float deposit_amount;
    while(1)
    {
        user = get_user_info(sockfd, user.username);
        printf("Current balance: Tk%.2f\n", user.balance);
        printf("Enter amount to deposit (0 to go back): ");

        if(scanf("%f", &deposit_amount) == 0)
        {
            getchar();
            printf("Invalid amount. Try again.\n");
            continue;
        }
        
        getchar();

        if(deposit_amount == 0)
        {
            clear_screen();
            return;
        }
        else
        {
            
            package_command(command, "DEPOSIT", user.username, "", "", "", "", "");
            write(sockfd, command, 256);
            write(sockfd, &deposit_amount, sizeof(float));
            bool response;
            read(sockfd, &response, sizeof(response));

            if(response == true)
            {
                clear_screen();
                printf("Deposition successful, returning to homepage\n\n");
                return;
            }
            else
            {
                clear_screen();
                printf("Deposition failed. Returning to homepage.\n\n");
                return;
            }
            
        }       
    }
}
void transfer(int sockfd, user_info user)
{
    clear_screen();
    char account_number[14];
    char command[256];
    float transfer_amount;

        
    bool response = true;
    while(response == true)
    {
        get_account_no(account_number, "Please enter the account number of the transferee (0 to go back): ");
        if(strcmp(account_number, "0") == 0)
        {
            return;
        }

        package_command(command, "ACC-NO-CHECK", account_number, "", "", "", "", "");
        write(sockfd, command, 256);
        read(sockfd, &response, sizeof(bool));

        if(response == true)
        {
            clear_screen;
            printf("Account number does not exist. Try again.\n");
        }
    }


    while(1)
    {
        clear_screen();
        user = get_user_info(sockfd, user.username);
        printf("Current balance: Tk%.2f\n", user.balance);
        printf("Enter the amount to transfer (0 to go back): ");
        if(scanf("%f", &transfer_amount) == 0)
        {
            getchar();
            printf("Invalid amount. Try again.\n");
            continue;
        }
        getchar();
        if(transfer_amount == 0)
        {
            clear_screen();
            return;
        }

        user = get_user_info(sockfd, user.username);

        if(user.balance >= transfer_amount)
        {
            
            package_command(command, "TRANSFER", user.username, account_number, "", "", "", "");
            write(sockfd, &command, sizeof(command));
            write(sockfd, &transfer_amount, sizeof(float));
            bool response;
            read(sockfd, &response, sizeof(response));

            if(response == true)
            {
                clear_screen();
                printf("Transfer successful, returning to homepage\n\n");
                return;
            }
            else
            {
                clear_screen();
                printf("Transaction failed. Try again.\n");
            }
            
        }
        else
        {
            clear_screen();
            printf("Insufficient balance.\n");
        }         
    }

            

    
}
void check_balance(int sockfd, user_info user)
{
    //flush_socket(sockfd);
    user = get_user_info(sockfd, user.username);
    clear_screen();
    printf("Balance: Tk%.2f", user.balance);
}
void change_account_details(int sockfd, user_info user)
{
    clear_screen();
    
    int choice;

    while(1)
    {
        printf("1. Change Username\n"
               "2. Change Password\n"
               "3. Change Date of Birth\n"
               "4. Change Favourite Animal\n"
               "0. Go Back\n"
               "Please select an operation (0-4): ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case  0:    clear_screen();
                        return;

            case  1:    change_username(sockfd, user.username);
                        break;

            case  2:    change_password_homepage(sockfd, user.username);
                        break;

            case  3:    change_date_of_birth(sockfd, user.username);
                        break;

            case  4:    change_favourite_animal(sockfd, user.username);
                        break;

        
            default:    clear_screen();
                        printf("Invalid choice.\n\n");
                        
        }

        
            
    }

}
void change_username(int sockfd, char username[])
{
    bool response = check_password(sockfd, username);
    if(response == false)
        return;
    
    char command[256];
    char new_username[65];
    response = false;
    while(response == false)
    {
        get_username(new_username, "Enter new username (0 to go back): ");
        if(strcmp(new_username, "0") == 0)
        {
            clear_screen();
            return;
        }
        sha256(new_username, 0, new_username);
        package_command(command, "USERNAME-CHECK", new_username, "", "", "", "", "");
        write(sockfd, command, 256);
        read(sockfd, &response, sizeof(bool));
        if(response == false)
        {
            printf("Username taken already, try a different username.\n");
        }
    }

    package_command(command, "CHANGE-USERNAME", username, new_username, "", "", "", "");
    write(sockfd, command, 256);
    read(sockfd, &response, sizeof(bool));
    if(response == true)
        printf("Username validated.\n\n");
    else
        printf("Failed to update username.\n\n");
    


}
void change_password_homepage(int sockfd, char username[])
{
    bool response = check_password(sockfd, username);
    if(response == false)
        return;
    
    change_password(sockfd, username);
}
void change_date_of_birth(int sockfd, char username[])
{
    bool response = check_password(sockfd, username);
    if(response == false)
        return;
    
    char command[256];
    char new_date_of_birth[11];
    response = false;
    
    
    get_date_of_birth(new_date_of_birth, "Enter new date of birth [DD-MM-YYYY] (0 to go back): ");
    if(strcmp(new_date_of_birth, "0") == 0)
    {
        clear_screen();
        return;
    }
        
    

    package_command(command, "CHANGE-DOB", username, new_date_of_birth, "", "", "", "");
    write(sockfd, command, 256);
    read(sockfd, &response, sizeof(bool));
    if(response == true)
        printf("Date of birth successfully updated.\n\n");
    else
        printf("Failed to update date of birth.\n\n");
}
void change_favourite_animal(int sockfd, char username[])
{
    bool response = check_password(sockfd, username);
    if(response == false)
        return;
    
    char command[256];
    char new_favourite_animal[21];
    response = false;
    
    
    get_favourite_animal(new_favourite_animal, "Enter new favourite animal (0 to go back): ");
    if(strcmp(new_favourite_animal, "0") == 0)
    {
        clear_screen();
        return;
    }
        
    

    package_command(command, "CHANGE-FAVANI", username, new_favourite_animal, "", "", "", "");
    write(sockfd, command, 256);
    read(sockfd, &response, sizeof(bool));
    if(response == true)
        printf("Favourite animal successfully updated.\n\n");
    else
        printf("Failed to update favourite animal.\n\n");
}
bool check_password(int sockfd, char username[])
{   
    LABEL01:
    char command[256];
    char password[65];
    bool response = false;
    get_password(password, "Enter your password (0 to go back): ");
    if(strcmp(password, "0") == 0)
    {
        clear_screen();
        return response;
    }
    package_command(command, "PASS-CHECK", username, "", "", "", "", "");
    write(sockfd, &command, sizeof(command));
    char salt[17];
    read(sockfd, &salt, sizeof(salt));
    sha256(password, 2, salt);
    write(sockfd, &password, sizeof(password));
    read(sockfd, &response, sizeof(bool));
    if(response == false)
    {
        clear_screen();
        printf("Password incorrect. Try again\n");
        goto LABEL01;
    }
    else
        return response;


}
void view_transactions(int sockfd, user_info user)
{
    char command[256];
    package_command(command, "VIEW-TRX", user.username, user.account_no, "", "", "", "");
    write(sockfd, command, sizeof(command));
    
    char buffer[256] = {0};

    printf("\n\n----------------------------------------------------BANK STATEMENT--------------------------------------------------\n\n");
    printf("%22s | %12s | %18s | %12s | %s | %9s | Status\n", "Date", "Timestamp", "Transaction ID", "Type", "Receiver/Sender", "Amount");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    while(1)
    {
        read(sockfd, buffer, sizeof(buffer));

        if(strcmp(buffer, "EOF") != 0)
        {
            printf("%s\n", buffer);
        }
        else 
            break;

    }
    printf("\n-------------------------------------------------END OF TRANSACTIONS------------------------------------------------\n");

    int choice;

    while(1)
    {
        printf("1. Download history\n"
            "2. Return to home screen\n"
            "Please select an operation (1-2): ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
            case  1:    download_trx(sockfd, user);
                        return;

            case  2:    return;

            default:    clear_screen();
                        printf("Invalid choice.\n\n");
        }
    }
    


}
void download_trx(int sockfd, user_info user)
{
    char output_file[64];
    sprintf(output_file, "%s/transaction_history.txt", user.account_no);
    FILE* f = fopen(output_file, "w");
    
    char command[256];
    package_command(command, "VIEW-TRX", user.username, user.account_no, "", "", "", "");
    write(sockfd, command, sizeof(command));
    
    char buffer[256] = {0};

    fprintf(f, "----------------------------------------------------BANK STATEMENT--------------------------------------------------\n\n");
    fprintf(f, "%22s | %12s | %18s | %12s | %s | %9s | Status\n", "Date", "Timestamp", "Transaction ID", "Type", "Receiver/Sender", "Amount");
    fprintf(f, "-----------------------+--------------+-----------------------------------+-----------------+-----------+------------\n");
    while(1)
    {
        read(sockfd, buffer, sizeof(buffer));

        if(strcmp(buffer, "EOF") != 0)
        {
            fprintf(f, "%s\n", buffer);
        }
        else 
            break;

    }
    fprintf(f, "\n-------------------------------------------------END OF TRANSACTIONS------------------------------------------------\n");

    fclose(f);
}


void admin_registration(int sockfd)
{
    char command[256];
    char username[65] = {0}, password[65] = {0}, salt[17] = {0};
    bool response;

    
    admin_input_signup(sockfd, username, password, salt);

    if(strcmp(username, "0") == 0)
        return;

    package_command(command, "AD-SIGNUP", username, password, "", "", "", salt);

    write(sockfd, command, sizeof(command));

    read(sockfd, &response, sizeof(response));
    if(response == true)
    {
        clear_screen();
        printf("Account creation successful\n\n");
        //printf("salt: %s\n", salt);
    }
    else
    {
        printf("Account creation failed.\n");
    }


}
void admin_input_signup(int sockfd, char username[], char password[], char salt[])
{
    
    LABEL02:
    bzero(username, 65);
    char command[256] = {0};
    bool response = false;

    while(response == false)
    {
        get_username(username, "Set username (0 to go back): ");
        if(strcmp(username, "0") == 0)
        {
            return;
        }
        sha256(username, 0, username);
        package_command(command, "AD-USERNAME-CHECK", username, "", "", "", "", "");
        //printf("%s\n", command);
        //printf("%s\n", &command[20]);
        write(sockfd, &command, sizeof(command));
        read(sockfd, &response, sizeof(bool));
        if(response == false)
        {
            printf("Username taken already, try a different username.\n");
        }
    }

    //printf("Username validated.\n");

    

    LABEL03:
    get_password(password, "Set password (0 to go back): ");
    if(strcmp(password, "0") == 0)
    {
        clear_screen();
        goto LABEL02;
    }
    sha256(password, 1, salt);
    
    //printf("password: %s\n", password);
    //printf("salt: %s\n", salt);

}
void admin_login(int sockfd)
{
    char username[65] = {0};
    
    admin_input_login(sockfd, username);

    if(strcmp(username, "0") == 0)
        return;

    //("Login initiated.\n");
    clear_screen();
    admin_homepage(sockfd, username);
}
void admin_input_login(int sockfd, char username[])
{
    LABEL01:
    
    char command[256];
    bool response;
    get_username(username, "Enter username (0 to go back): ");
    if(strcmp(username, "0") == 0)
    {
        clear_screen();
        return;
    }
    
    sha256(username, 0, username);

    package_command(command, "AD-USERNAME-CHECK", username, "", "", "", "", "");
    write(sockfd, &command, 256);
    read(sockfd, &response, sizeof(bool));
    
    
    if(response == true)
    {
        printf("Username does not exist.\n");
        goto LABEL01;
    }

    LABEL02:
    char password[65] = {0};
    
    get_password(password, "Enter password (0 to go back): ");
    if(strcmp(password, "0") == 0)
    {
        clear_screen();
        goto LABEL01;
    }

    package_command(command, "AD-PASS-CHECK", username, "", "", "", "", "");
    write(sockfd, &command, sizeof(command));
    char salt[17];
    read(sockfd, &salt, sizeof(salt));
    sha256(password, 2, salt);
    printf("%s\n", password);
    write(sockfd, &password, sizeof(password));
    read(sockfd, &response, sizeof(bool));
    if(response == false)
    {
        clear_screen();
        printf("Password incorrect. Try again\n");
        goto LABEL02;
    }
    

    
    
}
void admin_homepage(int sockfd, char username[])
{
    printf("Admin Homepage\n\n");
    int choice;

    while(1)
    {
        printf("1. Manage user profiles\n"
               "2. Download logbooks\n"
               "3. Download databases\n"
               "4. Logout\n"
               "Please select an operation (1-4): ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case  1:    manage_users(sockfd);
                        break;

            case  2:    view_logbook(sockfd);   
                        break;       

            case  3:    view_database(sockfd);               
                        break;
            
            case  4:    return;

            default:    printf("Invalid choice.\n");
        }
    }
}
void manage_users(int sockfd)
{
    clear_screen();
    LABEL01:
    bool response = true;
    char account_no[14];
    char command[256];
    
    while(response == true)
    {
        
        get_account_no(account_no, "Enter the account number to search for (0 to go back): ");
        if(strcmp(account_no, "0") == 0)
            return;
        
        package_command(command, "AD-ACC-SEARCH", account_no, "", "", "", "", "");
        write(sockfd, command, sizeof(command));
        read(sockfd, &response, sizeof(bool));

        if(response == true)
        {
            clear_screen;
            printf("Account does not exist\n");
        }
    }
    
    char choice;
    printf("Reset login info (y/n) [0 to go back]?: ");
    scanf("%c", &choice);
    getchar();

    if(choice == 'y')
        reset_login_info(sockfd, account_no);
    else if(choice == '0')
        goto LABEL01;
    else 
        return;
            
}
void reset_login_info(int sockfd, char account_no[])
{
    char command[256];
    char new_username[65];
    char new_password[65];
    char salt[17] = {0};
    bool response;
    LABEL01:
    get_username(new_username, "Enter new username (0 to go back): ");
    if(strcmp(new_username, "0") == 0)
        return;

    sha256(new_username, 0, new_username);

    package_command(command, "USERNAME-CHECK", new_username, "", "", "", "", "");
    write(sockfd, &command, 256);
    read(sockfd, &response, sizeof(bool));
    if(response == false)
    {
        printf("Username already taken\n");
        goto LABEL01;
    }
    

    
    get_password(new_password, "Enter new password(0 to go back): ");
    if(strcmp(new_password, "0") == 0)
        goto LABEL01;

    sha256(new_password, 1, salt);

    printf("username: %s\n", new_username);
    printf("new_password: %s\n", new_password);
    printf("salt: %s\n", salt);

    package_command(command, "AD-RESET-LOGIN", new_username, new_password, "", "", account_no, salt);
    write(sockfd, command, sizeof(command));
    read(sockfd, &response, sizeof(bool));
    if(response == true)
    {
        printf("Account reset successful\n");
    }
    else
        printf("Account reset failed\n");

}
void view_logbook(int sockfd)
{
    int choice;
    
    while(1)
    {
        printf("Download: ");
        printf("1. User Activity Logbook\n"
               "2. User Transaction Logbook\n"
               "3. Admin Activity Logbook\n"
               "4. Return to previous menu\n"
               "Please select an operation (1-4): ");
        scanf("%d", &choice);
        getchar();

        char output_file[64];
        char command[256];
        FILE* f;

        switch (choice)
        {
            case  1:    sprintf(output_file, "admin/logbook_activity.log");
                        package_command(command, "DOWN-ACTIVITY", "", "", "", "", "", "");
                        break;

            case  2:    sprintf(output_file, "admin/logbook_transactions.log");
                        package_command(command, "DOWN-TRX", "", "", "", "", "", "");
                        break;       

            case  3:    sprintf(output_file, "admin/logbook_activity_admin.log");
                        package_command(command, "DOWN-ACTIVITY-AD", "", "", "", "", "", "");         
                        break;
            
            case  4:    return;

            default:    printf("Invalid choice.\n");
        }

        
        f = fopen(output_file, "w");
        write(sockfd, command, sizeof(command));
        
        char buffer[256] = {0};

        while(1)
        {
            read(sockfd, buffer, sizeof(buffer));

            if(strcmp(buffer, "EOF") != 0)
            {
                fprintf(f, "%s\n", buffer);
            }
            else 
                break;

        }
        

        fclose(f);

    }
}

void view_database(int sockfd)
{
    int choice;
    
    while(1)
    {
        printf("Download: ");
        printf("1. User Database\n"
               "2. User Database\n"
               "3. Return to previous menu\n"
               "Please select an operation (1-4): ");
        scanf("%d", &choice);
        getchar();

        char output_file[64];
        char command[256];
        FILE* f;

        switch (choice)
        {
            case  1:    sprintf(output_file, "admin/user_database.bin");
                        package_command(command, "BDOWN-USER-DB", "", "", "", "", "", "");
                        break;

            case  2:    sprintf(output_file, "admin/admin_datababase.bin");
                        package_command(command, "BDOWN-AD-DB", "", "", "", "", "", "");
                        break;       

            case  3:    return;

            default:    printf("Invalid choice.\n");
        }

        
        f = fopen(output_file, "wb");
        write(sockfd, command, sizeof(command));

        if(choice == 1)
        {
            user_info user;

            while(1)
            {
                read(sockfd, &user, sizeof(user_info));

                if(strcmp(user.username, "\0") != 0)
                {
                    fwrite(&user, sizeof(user), 1, f);
                }
                else 
                    break;

            }
        }
        else if(choice == 2)
        {
            admin_info admin;

            while(1)
            {
                read(sockfd, &admin, sizeof(admin_info));

                if(strcmp(admin.username, "\0") != 0)
                {
                    fwrite(&admin, sizeof(admin), 1, f);
                }
                else 
                    break;

            }
        }
        
        
        
        

        fclose(f);

    }
}

void error(const char *message)
{
    perror(message);
    exit(1);
}
void clear_screen()
{
    system("clear");
}















