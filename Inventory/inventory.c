#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "readline.h"


#define NAME_LEN 30
#define MAX_PARTS 100

struct part {
    int number;
    char name[NAME_LEN+1];
    int on_hand;
} inventory[MAX_PARTS];

int num_parts = 0; // currently stored

int find_part(int number);
void insert(void);
void search(void);
void update(void);
void print(void);

int main(void)
{
    char code;

    for(;;) {

        printf("Enter operation code: ");
        scanf(" %1c", &code);
        while (getchar() != '\n')
            ;
        switch (code)
        {
            case 'i': insert(); break;
            case 's': search(); break;
            case 'u': update(); break;
            case 'p': print(); break;
            case 'q': return 0; break;
            default: printf("Illegal code\n");
        }
        printf("\n");
    }
}

int find_part(int number) {
    int i;

    for (i = 0; i < num_parts; i++) {
        if(inventory[i].number == number) {
            return i;
        }
    }
    return -1;
}

void insert(void) {
    int part_number;
    char input_buffer[256];

    if (num_parts == MAX_PARTS) {
        printf("Database is full. Can't add more parts\n");
        return;
    }
    
    while(1) {
        printf("Enter part number: ");
        if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return;
        }

        int valid_number = 1;
        for (int i = 0; input_buffer[i] != '\n'; i++) {
            if(!isdigit(input_buffer[i])) {
                valid_number = 0;
                break;
            }
        }

        if (valid_number && sscanf(input_buffer, "%d", &part_number) == 1) {
            break;
        } else {
            fprintf(stderr, "Incorrect part number. Use only digits.\n");
        }
    }

    if(find_part(part_number) >= 0) {
        printf("Part already exists.\n");
        return;
    }

    inventory[num_parts].number = part_number;
    printf("Enter part name: ");
    read_line(inventory[num_parts].name, NAME_LEN);
    printf("Enter quantity on hand: ");
    scanf("%3d", &inventory[num_parts].on_hand);

    num_parts++;
}

void search(void) {
    int i, number;
    char input_buffer[256];

    while(1) {
        printf("Enter part number: ");
        if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Error value. Enter only digits\n");
            return;
        }

        int valid_number = 1;
        for (int j = 0; input_buffer[j] != '\n'; j++) {
            if(!isdigit(input_buffer[j])) {
                valid_number = 0;
                break;
            }
        }

        if (valid_number && sscanf(input_buffer, "%d", &number) == 1) {
            break;
        } else {
            fprintf(stderr, "Incorrect part number. Use only digits.\n");
        } 
    }

    i = find_part(number);
    if (i >= 0) {
        printf("Part name: %s\n", inventory[i].name);
        printf("Quantity on hand: %d\n", inventory[i].on_hand);
    } else {
        printf("Part not found.\n");
    }
    
}

void update(void) {
    int i, number, change;
    char input_buffer[256];

    while(1) {
        printf("Enter part number: ");
        if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            fprintf(stderr, "Incorrect value. Enter part number\n");
            return;
        }

        int valid_number = 1;
        for (int j = 0; input_buffer[j] != '\n'; j++) {
            if(!isdigit(input_buffer[j])) {
                valid_number = 0;
                break;
            }
        }

        if(valid_number && sscanf(input_buffer, "%d", &number)) {
            break;
        } else {
            fprintf(stderr, "Incorrect value. Enter part number\n");
        }
    }

    i = find_part(number);
    if (i >= 0) {
        printf("Enter change in quantity on hand: ");
        scanf("%3d", &change);
        inventory[i].on_hand += change;
    } else {
        printf("Part not found.\n");
    }
}

void print(void) {
    int i;

    printf("Part Number    Part Name        Quantity on Hand\n");
    for (i = 0; i < num_parts; i++) {
        printf("%7d        %-25s%2d\n", inventory[i].number, inventory[i].name, inventory[i].on_hand);
    }
}