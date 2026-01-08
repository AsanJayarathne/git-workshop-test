 #include <stdio.h>
#include <string.h>
#include <stdlib.h> // For dynamic memory allocation

#define MAX_ITEMS 100 // Maximum number of items

void displaybill();
void readBillData();
void addNewItem();

int main(void) {
    char user[50];
    int password;

    printf("-------WELCOME-----------\n");
    printf("PLEASE LOG IN TO SYSTEM\n");

    printf("ENTER USERNAME: ");
    scanf("%s", user);
    printf("ENTER PASSWORD: ");
    scanf("%d", &password);

    int func;
    if (strcmp(user, "admin") == 0 && password == 1234) {
        int loggedin=1;
        while(loggedin){
        printf("-------MENU-----------\n");
        printf("----1-Print-Bill\n");
        printf("----2-Print-Bill-Summary\n");
        printf("----3-Add-New-Items\n");
        printf("----0-Exit--\n");
        scanf("%d",&func);
        switch(func) {
            case 1:
                displaybill();
                break;
            case 2:
                readBillData();
                break;
            case 3:
                addNewItem();
                break;
            case 0:
                printf("Exiting...\n");
                    loggedin=0;
                break;
            default:
                printf("no function selected");
                break;
        }
        }
    } else {
        printf("WRONG USERNAME OR PASSWORD\n");
    }

    return 0;
}

void displaybill() {
    FILE *file = fopen("billing_data.txt", "a+");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    FILE *itemsFile = fopen("items_prices.txt", "r");
    if (itemsFile == NULL) {
        printf("Error opening items and prices file\n");
        return;
    }

    char line[256];
    char *items[MAX_ITEMS];
    float prices[MAX_ITEMS];
    int itemCount = 0;

    
    while (fgets(line, sizeof(line), itemsFile) && itemCount < MAX_ITEMS) {
        items[itemCount] = strdup(strtok(line, ","));
        prices[itemCount] = atof(strtok(NULL, ","));
        itemCount++;
    }
    fclose(itemsFile);

    int index, qty;
    float total = 0, subtotal = 0;

    printf("\nGrocery Items and Prices:\n");
    printf("----------------------------\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%02d-%-10s - Rs %.2f\n", i+1, items[i], prices[i]);
    }

    printf("\nTo stop adding items, enter '0' as the item code.\n");
    while (1) {
        printf("\nEnter item code (1 to %d, 0 to exit): ", itemCount);
        scanf("%d", &index);
        if (index == 0) break;
        if (index < 1 || index > itemCount) {
            printf("Invalid item code. Please enter a code between 1 and %d.\n", itemCount);
            continue;
        }
        printf("Enter quantity: ");
        scanf("%d", &qty);
        if (qty <= 0) {
            printf("Quantity must be greater than 0. Please try again.\n");
            continue;
        }
        total = qty * prices[index - 1];
        fprintf(file, "%s: %d * %.2f = %.2f\n", items[index - 1], qty, prices[index - 1], total);
        subtotal += total;
    }
    fprintf(file, "Total before discount: %.2f\n\n", subtotal);

    char ans;
    int dis;
    float ftot;

    printf("\nDo you want to add a discount? (y/n): ");
    scanf(" %c", &ans);

    if (ans == 'y' || ans == 'Y') {
        printf("Enter discount percentage (0--100): ");
        scanf("%d", &dis);

        if (dis < 0 || dis > 100) {
            printf("Invalid discount percentage. No discount applied.\n");
            printf("Your total is: Rs %.2f\n", subtotal);
        } else {
            ftot = subtotal - (subtotal * dis / 100.0);
            printf("\nTotal: Rs %.2f\n", subtotal);
            printf("Discount: %d%%\n", dis);
            printf("Total after discount: Rs %.2f\n", ftot);
            fprintf(file, "Total after discount: %.2f\n\n\n", ftot);
        }
    } else {
        printf("\nYour total is: Rs %.2f\n", subtotal);
    }
    fclose(file);
}

void readBillData() {
    FILE *file = fopen("billing_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[256];
    printf("\nBill summary:\n");
    printf("----------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}
void addNewItem() {
    char itemName[100];
    float price;
    printf("Enter the name of the new item: ");
    scanf("%s", itemName); 
    printf("Enter the price of the new item: ");
    scanf("%f", &price);

    FILE *fp = fopen("items_prices.txt", "a"); 
    if (fp == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    fprintf(fp, "%s,%.2f\n", itemName, price);
    fclose(fp);
    printf("Item added successfully.\n");
}
