#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "readline.h"

#define NAME_LEN 25

struct part {
    int number;
    char name[NAME_LEN+1];
    int on_hand;
    struct part *next;
};

struct part *inventory = NULL; // first part

struct part *find_part(int number);
void insert(void);
void search(void);
void update(void);
void print(void);
void erace(void);


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
            case 'e': erace(); break;
            case 'i': insert(); break;
            case 's': search(); break;
            case 'u': update(); break;
            case 'p': print(); break;
            case 'q': return 0; break;
            default: printf("Illegal code\n");
        }
        printf("\n");
    }

    free(inventory);
}

struct part *find_part(int number) {
    struct part *p;

    for (p = inventory; p != NULL && number > p->number; p = p->next)
        ;
    
    if (p != NULL && number == p->number) {
        return p;
    }

    return NULL;
}

void erace(void) {
    struct part **p = &inventory;
    struct part *temp;
    int part_num;
    char num_buffer[256];

    while(1) {
        printf("Enter part number: ");
        if (fgets(num_buffer, sizeof(num_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading user input for delete part\n");
            return;
        }

        int valid_number = 1;
        for (int i = 0; num_buffer[i] != '\n'; i++) {
            if(!isdigit(num_buffer[i])) {
                valid_number = 0;
                break;
            }
        }

        if (valid_number && sscanf(num_buffer, "%d", &part_num) == 1) {
            break;
        } else {
            fprintf(stderr, "Incorrect value\n");
        }
    }

    while(*p) {
        if((*p)->number == part_num) {
            temp = *p;
            *p = (*p)->next;
            free(temp);
            return;
        }
        p = &(*p)->next;
    }
    printf("Part %d was not found in database\n", part_num);
    return;
}

void insert(void) {
    struct part *cur, *prev, *new_node;
    char input_buffer[256];

    new_node = malloc(sizeof(struct part));
    if (new_node == NULL) {
        printf("Database is full; can't add more parts.\n");
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

        if (valid_number && sscanf(input_buffer, "%d", &new_node->number) == 1) {
            break;
        } else {
            fprintf(stderr, "Incorrect part number. Use only digits.\n");
        }
    }

    for (cur = inventory, prev = NULL; cur != NULL && new_node->number > cur->number;
         prev = cur, cur = cur->next)
        ;     
    if (cur != NULL && new_node->number == cur->number) {
        printf("Part already exists.\n");
        free(new_node);
        return;
    }

    printf("Enter part name: ");
    read_line(new_node->name, NAME_LEN);
    printf("Enter quantity on hand: ");
    scanf("%3d", &new_node->on_hand);

    new_node->next = cur;
    if (prev == NULL) {
        inventory = new_node;
    } else {
        prev->next = new_node;
    }

}

void search(void) {
    int number;
    struct part *p;
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

    p = find_part(number);
    if (p != NULL) {
        printf("Part name: %s\n", p->name);
        printf("Quantity on hand: %d\n", p->on_hand);
    } else {
        printf("Part not found.\n");
    }
}

void update(void) {
    int number, change;
    struct part *p;
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

    p = find_part(number);
    if (p != NULL) {
        printf("Enter change in quantity on hand: ");
        scanf("%3d", &change);
        p->on_hand += change;
    } else {
        printf("Part not found.\n");
    }
}

void print(void) {
    struct part *p;

    printf("Part Number    Part Name        Quantity on Hand\n");
    for (p = inventory; p != NULL; p = p->next) {
        printf("%7d        %-25s%2d\n", p->number, p->name, p->on_hand);
    }
}
