#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Consumer {
    int id;
    char name[50];
    float prev_units;
    float curr_units;
    float used_units;
    float bill;
};

float calculateBill(float units) {
    float amount = 0;

    if (units <= 100)
        amount = units * 1.5;
    else if (units <= 300)
        amount = (100 * 1.5) + ((units - 100) * 2.5);
    else if (units <= 500)
        amount = (100 * 1.5) + (200 * 2.5) + ((units - 300) * 4);
    else
        amount = (100 * 1.5) + (200 * 2.5) + (200 * 4) + ((units - 500) * 6);

    amount += 50;                 // Fixed charge
    amount += amount * 0.05;      // 5% tax

    return amount;
}

void addConsumer() {
    struct Consumer c;
    FILE *fp = fopen("bill.dat", "ab");

    if (fp == NULL) {
        printf("File Error!\n");
        return;
    }

    printf("\nEnter Consumer ID: ");
    scanf("%d", &c.id);
    printf("Enter Consumer Name: ");
    scanf(" %[^\n]", c.name);
    printf("Enter Previous Meter Reading: ");
    scanf("%f", &c.prev_units);
    printf("Enter Current Meter Reading: ");
    scanf("%f", &c.curr_units);

    c.used_units = c.curr_units - c.prev_units;
    c.bill = calculateBill(c.used_units);

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    printf("\n Consumer Added Successfully!\n");
}

void displayBill() {
    struct Consumer c;
    int id, found = 0;
    FILE *fp = fopen("bill.dat", "rb");

    if (fp == NULL) {
        printf("No Records Found!\n");
        return;
    }

    printf("\nEnter Consumer ID to Search: ");
    scanf("%d", &id);

    while (fread(&c, sizeof(c), 1, fp)) {
        if (c.id == id) {
            printf("\n-------------------------------------");
            printf("\n        ELECTRICITY BILL              ");
            printf("\n-------------------------------------");
            printf("\nConsumer ID     : %d", c.id);
            printf("\nConsumer Name   : %s", c.name);
            printf("\nPrevious Units  : %.2f", c.prev_units);
            printf("\nCurrent Units   : %.2f", c.curr_units);
            printf("\nUnits Used      : %.2f", c.used_units);
            printf("\n-------------------------------------");
            printf("\nTotal Bill      : ₹ %.2f", c.bill);
            printf("\n-------------------------------------\n");

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\n Consumer Not Found!\n");

    fclose(fp);
}

void monthlyReport() {
    struct Consumer c;
    FILE *fp = fopen("bill.dat", "rb");
    float total = 0;
    int count = 0;

    if (fp == NULL) {
        printf("No Data Available!\n");
        return;
    }

    while (fread(&c, sizeof(c), 1, fp)) {
        total += c.bill;
        count++;
    }

    fclose(fp);

    printf("\n-----------------------------------");
    printf("\n        MONTHLY REPORT              ");
    printf("\n-----------------------------------");
    printf("\nTotal Consumers : %d", count);
    printf("\nTotal Collection: ₹ %.2f", total);
    printf("\n-----------------------------------\n");
}

int main() {
    int choice;

    do {
        printf("\n=================================");
        printf("\n     ELECTRICITY BILL SYSTEM     ");
        printf("\n=================================");
        printf("\n1. Add New Consumer");
        printf("\n2. Generate Bill");
        printf("\n3. Monthly Report");
        printf("\n4. Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addConsumer();
                break;
            case 2:
                displayBill();
                break;
            case 3:
                monthlyReport();
                break;
            case 4:
                printf("\nThank You! Exiting...\n");
                break;
            default:
                printf("\n Invalid Choice!\n");
        }

    } while (choice != 4);

    return 0;
}

