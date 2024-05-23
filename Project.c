#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // Include this header for getch() on Windows

#define MAX_EXPENSES 100
#define MAX_DESCRIPTION_LENGTH 100
#define FILENAME "expenses.csv"

// Structure to represent an expense
typedef struct
{
    char date[20];
    char category[30];
    char description[MAX_DESCRIPTION_LENGTH];
    float amount;
} Expense;

// Global array to store expense records
Expense expenses[MAX_EXPENSES];
int numExpenses = 0;

// Function to load expenses from the CSV file
void loadExpenses()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("No expenses recorded yet.\n");
        return;
    }

    numExpenses = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%f\n", expenses[numExpenses].date, expenses[numExpenses].category, expenses[numExpenses].description, &expenses[numExpenses].amount) == 4)
    {
        numExpenses++;
    }

    fclose(file);
}

// Function to save expenses to the CSV file
void saveExpenses()
{
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numExpenses; i++)
    {
        fprintf(file, "%s,%s,%s,%.2f\n", expenses[i].date, expenses[i].category, expenses[i].description, expenses[i].amount);
    }

    fclose(file);
}

// Function to clear the screen based on the operating system
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to print the green header
void printHeader(const char *header)
{
    printf("\033[0;32m%s\033[0m\n", header); // Set text to green
    printf("\n"); // Print a newline for gap
}

// Function to add a new expense
void addExpense()
{
    clearScreen();
    printHeader("Add Expense");
    if (numExpenses >= MAX_EXPENSES)
    {
        printf("Maximum number of expenses reached.\n");
        return;
    }

    printf("Enter date (DD/MM/YYYY): ");
    scanf("%s", expenses[numExpenses].date);
    getchar(); // Consume newline character left by scanf

    printf("Enter category: ");
    fgets(expenses[numExpenses].category, sizeof(expenses[numExpenses].category), stdin);
    strtok(expenses[numExpenses].category, "\n"); // Remove newline character

    printf("Enter description: ");
    fgets(expenses[numExpenses].description, sizeof(expenses[numExpenses].description), stdin);
    strtok(expenses[numExpenses].description, "\n"); // Remove newline character

    printf("Enter amount: ");
    scanf("%f", &expenses[numExpenses].amount);

    numExpenses++;
    saveExpenses();

    printf("Expense added successfully.\n");
}

// Function to view all expenses
void viewExpenses()
{
    clearScreen();
    printHeader("View Expenses");
    if (numExpenses == 0)
    {
        printf("No expenses recorded.\n");
        return;
    }

    printf("All Expenses:\n");
    printf("-----------------------------------------------------------------\n");
    printf("| %-4s | %-12s | %-15s | %-30s | %-10s |\n", "Index", "Date", "Category", "Description", "Amount");
    printf("-----------------------------------------------------------------\n");
    float totalAmount = 0.0;
    for (int i = 0; i < numExpenses; i++)
    {
        printf("| %-4d | %-12s | %-15s | %-30s | $%-9.2f |\n", i + 1, expenses[i].date, expenses[i].category, expenses[i].description, expenses[i].amount);
        totalAmount += expenses[i].amount;
    }
    printf("-----------------------------------------------------------------\n");
    printf("Total Amount: $%.2f\n", totalAmount);
}

// Function to delete an expense
void deleteExpense()
{
    while (1)
    {
        clearScreen();
        printHeader("Delete Expense");
        if (numExpenses == 0)
        {
            printf("No expenses recorded.\n");
            return;
        }

        viewExpenses();

        int index;
        char input[10];
        printf("Enter the index of the expense to delete (1 to %d) or 'x' to go back to main menu: ", numExpenses);
        fgets(input, sizeof(input), stdin);
        strtok(input, "\n"); // Remove newline character

        if (input[0] == 'x' || input[0] == 'X')
        {
            printf("Returning to main menu.\n");
            return;
        }

        index = atoi(input);
        if (index >= 1 && index <= numExpenses)
        {
            for (int i = index - 1; i < numExpenses - 1; i++)
            {
                expenses[i] = expenses[i + 1];
            }
            numExpenses--;
            saveExpenses();
            printf("Expense deleted successfully.\n");
            return;
        }
        else
        {
            printf("Invalid index. Please try again.\n");
        }
    }
}

// Function to edit an expense
void editExpense()
{
    while (1)
    {
        clearScreen();
        printHeader("Edit Expense");
        if (numExpenses == 0)
        {
            printf("No expenses recorded.\n");
            return;
        }

        viewExpenses();

        int index;
        char input[10];
        printf("Enter the index of the expense to edit (1 to %d) or 'x' to go back to main menu: ", numExpenses);
        fgets(input, sizeof(input), stdin);
        strtok(input, "\n"); // Remove newline character

        if (input[0] == 'x' || input[0] == 'X')
        {
            printf("Returning to main menu.\n");
            return;
        }

        index = atoi(input);
        if (index >= 1 && index <= numExpenses)
        {
            index--; // Convert to zero-based index
            int editChoice;
            do
            {
                clearScreen();
                printHeader("Edit Expense");
                printf("Editing Expense #%d\n", index + 1);
                printf("1. Edit Date (current: %s)\n", expenses[index].date);
                printf("2. Edit Category (current: %s)\n", expenses[index].category);
                printf("3. Edit Description (current: %s)\n", expenses[index].description);
                printf("4. Edit Amount (current: %.2f)\n", expenses[index].amount);
                printf("5. Return to Main Menu\n");
                printf("Enter your choice: ");
                editChoice = getch() - '0'; // Capture single key press and convert to integer

                clearScreen();
                printHeader("Edit Expense");

                switch (editChoice)
                {
                case 1:
                    printf("Enter new date (DD/MM/YYYY): ");
                    scanf("%s", expenses[index].date);
                    getchar(); // Consume newline character left by scanf
                    break;
                case 2:
                    printf("Enter new category: ");
                    fgets(expenses[index].category, sizeof(expenses[index].category), stdin);
                    strtok(expenses[index].category, "\n"); // Remove newline character
                    break;
                case 3:
                    printf("Enter new description: ");
                    fgets(expenses[index].description, sizeof(expenses[index].description), stdin);
                    strtok(expenses[index].description, "\n"); // Remove newline character
                    break;
                case 4:
                    printf("Enter new amount: ");
                    scanf("%f", &expenses[index].amount);
                    break;
                case 5:
                    printf("Returning to main menu.\n");
                    break;
                default:
                    printf("Invalid choice. Please enter a valid option.\n");
                    break;
                }
            }
            while (editChoice != 5);

            saveExpenses();
            printf("Expense edited successfully.\n");
            return;
        }
        else
        {
            printf("Invalid index. Please try again.\n");
        }
    }
}

int main()
{
    loadExpenses();

    int choice;
    do
    {
        clearScreen();
        printHeader("Expense Tracker");
        printf("1. Add Expense\n");
        printf("2. View Expenses\n");
        printf("3. Edit Expense\n");
        printf("4. Delete Expense\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        choice = getch() - '0'; // Capture single key press and convert to integer

        clearScreen(); // Clear the screen before executing the selected option

        switch (choice)
        {
        case 1:
            addExpense();
            break;
        case 2:
            viewExpenses();
            break;
        case 3:
            editExpense();
            break;
        case 4:
            deleteExpense();
            break;
        case 5:
            printf("\033[0;32mExiting program. Goodbye!\033[0m\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
        printf("\nPress any key to continue...");
        getch(); // Wait for user input before returning to the menu
    }
    while (choice != 5);

    return 0;
}
