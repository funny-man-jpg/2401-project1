#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

int subsys_collection_init(SubsystemCollection *subsystems){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    subsystems->size = 0;
    return ERR_SUCCESS;
}

int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem){
    if (subsystems == NULL || subsystem == NULL){
        return ERR_NULL_POINTER;
    }
    if (subsystems->size != MAX_ARR){
        subsystems->subsystems[subsystems->size] = *subsystem;
        subsystems->size += 1;
        return ERR_SUCCESS;
    }else{
        return ERR_MAX_CAPACITY;
    }
    
}

int subsys_find(const SubsystemCollection *systems, const char *name){
    if (systems == NULL){
        return ERR_NULL_POINTER;
    }
    for (int i = 0; i < systems->size; i++)
    {
        if (strcmp(systems->subsystems[i].name, name) == ERR_SUCCESS){
            return i;
        }
    }
    return ERR_SYS_NOT_FOUND; 
}

int subsys_collection_print(SubsystemCollection *subsystems){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    if (subsystems->size <= 0){
        return ERR_NO_DATA;
    }
    for (int i = 0; i < subsystems->size; i++)
    {
        subsys_print(&subsystems->subsystems[i]);
    }
    return ERR_SUCCESS;
}