#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

int subsys_init(Subsystem *subsystem, const char *name, char status){
    if (subsystem == NULL){
        return ERR_NULL_POINTER;
    }
    strcpy(subsystem->name, name);
    subsystem->status = status;
    return ERR_SUCCESS;
}

int subsys_print(Subsystem *subsystem){
    //COME BACK HERE TO FIX THIS SIGMA NUGGETS STYLE
    //THIS WILL PROBABLY BREAK
    if (subsystem == NULL){
        return ERR_NULL_POINTER;
    }

    printf("name: %-16s status: %d\n", subsystem->name, subsystem->status);
    return ERR_SUCCESS;
}
