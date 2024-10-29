#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int print_menu(int *choice);

int main() {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.
    int error = ERR_SUCCESS;
    int index = 0;
    int choice = -1;  
    int value = 0;
    unsigned int data;
    unsigned int oldData;
    SubsystemCollection subsystemCollection;

    char name[MAX_STR];
    Subsystem subsystem;
    int status = 0;

    subsys_collection_init(&subsystemCollection);

    while (choice != MENU_EXIT){
        print_menu(&choice);
        if (choice == ERR_NULL_POINTER){
            printf("Null pointer exception\n");
        }
        switch(choice){
            case MENU_ADD:
                printf("Please input the name of the subsystem: ");
                scanf("%s", name);
                while(getchar() != '\n');
                //remember to do status stuff
                subsys_init(&subsystem, name, status);
                subsys_append(&subsystemCollection, &subsystem);
                break;
            case MENU_PRINT:
                printf("Please input the name of the subsystem: ");
                scanf("%s", name);
                while(getchar() != '\n');
                index = subsys_find(&subsystemCollection, name);
                if (index == ERR_SYS_NOT_FOUND){
                    printf("Subsystem not found\n");
                    break;
                }
                error = subsys_print(&subsystemCollection.subsystems[index]);
                if (error != ERR_SUCCESS){
                    error = ERR_SUCCESS;
                    printf("Null Pointer Exception\n");
                    break;
                }

                break;
            case MENU_PRINTALL:
                subsys_collection_print(&subsystemCollection);
                break;
            case MENU_STATUS:
                printf("Please input the name of the subsystem, the status you want to change (7,6,5,4,2,0), and how you want to change it (3,2,1,0): ");
                scanf("%s %d %d", name, &status, &value);
                while(getchar() != '\n');
                index = subsys_find(&subsystemCollection, name);
                if (index == ERR_SYS_NOT_FOUND){
                    printf("Subsystem not found\n");
                    break;
                }
                error = subsys_status_set(&subsystemCollection.subsystems[index], status, value);
                if (error != ERR_SUCCESS){
                    printf("Error");
                }
                printf("%d", error);
                break;
            case MENU_DATA:
                printf("Enter the subsystem name, then enter the data in hex uppercase without the 0x");
                scanf("%s %x", name, &data);
                while(getchar() != '\n');
                index = subsys_find(&subsystemCollection, name);
                if (index == ERR_SYS_NOT_FOUND){
                    printf("Subsystem not found\n");
                    break;
                }
                subsys_data_set(&subsystemCollection.subsystems[index], data, &oldData);
                printf("data updated successfully");
        }
    }
    return 0;
}

/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}