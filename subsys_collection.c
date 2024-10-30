#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

/*
    Initializes a SubsystemCollection.

    out: *subsystems, a SubsystemCollection that gets initialized.

    Returns:
        ERR_SUCCESS if it was successfully initialized
        ERR_NULL_POINTER if the subsystem was a null pointer
*/
int subsys_collection_init(SubsystemCollection *subsystems){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    subsystems->size = 0;
    return ERR_SUCCESS;
}

/*
    Appends a Subsystem to a SubsystemCollection, then increases the size by 1.

    out: *subsystems, a SubsystemCollection with an added subsystem.

    Returns:
        ERR_SUCCESS if it successfully was added.
        ERR_MAX_CAPACITY if the size for SubsystemCollection was full
        ERR_NULL_POINTER if either pointers were NULL
*/
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
/*
    Searches the subsystem collection for a subsystem with the corresponding name given by the name parameter, then returns the index of that subsystem.

    Returns:
        the index of the subsystem if it was in the collection.
        ERR_NULL POINTER if the pointer given by the systems parameter was empty.
        ERR_SYS_NOT_FOUND if no subsystem with the name parameter was in the system.
*/
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
/*
    Iterates through the subsystem collection and prints each subsystem and their attributes

    Returns:
        ERR_NULL_POINTER if the pointer given to the subsystem collection is null
        ERR_NO_DATA if the subsystem is empty
        ERR_SUCCESS if the subsystems were printed successfully
*/

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