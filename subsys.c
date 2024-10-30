#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

/*
    initializes a subsystem of name given by parameters, with status given by status parameter

    returns:
        ERR_NULL_POINTER if the pointer to the subsystem to be initialized is null
        ERR_SUCCESS if the subsystem was initialized successfully
*/

int subsys_init(Subsystem *subsystem, const char *name, char status){
    if (subsystem == NULL){
        return ERR_NULL_POINTER;
    }
    strcpy(subsystem->name, name);
    subsystem->status = status;
    return ERR_SUCCESS;
}

/*
    Prints the name, data and status of the given subsystem pointed to by the parameter

    returns:
        ERR_NULL_POINTER if the given subsystem pointer is null\
        ERR_SUCCESS if the subsystem was successfully printed
*/

int subsys_print(Subsystem *subsystem){
    if (subsystem == NULL){
        return ERR_NULL_POINTER;
    }

    printf("name: %16s, ", subsystem->name);
    subsys_status_print(subsystem);
    int data;
    if ((subsystem->status & (1<<STATUS_DATA)) >> STATUS_DATA == 1){
        subsys_data_get(subsystem, &data);
        printf(", data: %08x", data);
    }
    return ERR_SUCCESS;
}

/*
    takes in a pointer to a subsystem, a number for which status is being changed, and a value to change the status to, 
    performs bitwise operations to change the status to the given value.

    returns:
        ERR_INVALID STATUS if the given value or the given status is outside of the valid range, or if the status being changed does not support the value given
        ERR_NULL_POINTER if the given subsystem pointer is null
        ERR_SUCCESS if the status was set successfully

*/

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
    if(subsystem == NULL){
        return ERR_NULL_POINTER;
    }
    if(value > 3 || value < 0){
        return ERR_INVALID_STATUS;
    }
    if(status > STATUS_POWER || status < STATUS_RESOURCE){
        return ERR_INVALID_STATUS;
    }
    if(status == STATUS_RESOURCE || status == STATUS_PERFORMANCE){
        switch(value){
            case 0:
                subsystem->status = (subsystem->status & (~(1<<status)));
                subsystem->status = (subsystem->status & (~(1<<status+1)));
                break;
            case 1:
                subsystem->status = (subsystem->status | (1<<status));
                subsystem->status = (subsystem->status & (~(1<<status+1)));
                break;
            case 2:
                subsystem->status = (subsystem->status | (1 << status+1));
                subsystem->status = (subsystem->status & (~(1<<status)));
                break;
            case 3:
                subsystem->status = (subsystem->status | (1 << status));
                subsystem->status = (subsystem->status | (1 << status+1));
                break;
        }
    }
    else{
        if(value > 1){
            return ERR_INVALID_STATUS;
        }
        else{
            if(value == 0){
                subsystem->status = (subsystem->status & (~(1<<status)));
            }
            else{
                subsystem->status = (subsystem->status | (1<<status));
            }
        }
    }
    return ERR_SUCCESS;
}
/*
    Prints the status of a subsystem, based on the status byte.

    Returns:
        ERR_SUCCESS if it prints
        ERR_NULL_POINTER if *subsystem is null
*/
int subsys_status_print(const Subsystem *subsystem){
    if(subsystem == NULL){
        return ERR_NULL_POINTER;
    }
    printf("Status: (PwR: %d", ((subsystem->status & (1<<STATUS_POWER)) >> STATUS_POWER));
    printf("|DATA: %d",((subsystem->status & (1<<STATUS_DATA)) >> STATUS_DATA));
    printf("|ACT: %d",((subsystem->status & (1<<STATUS_ACTIVITY)) >> STATUS_ACTIVITY));
    printf("|ERROR: %d",((subsystem->status & (1<<STATUS_ERROR)) >> STATUS_ERROR));
    printf("|PERF: %d",(2 * (int) (subsystem->status & (1<<STATUS_PERFORMANCE+1)) >> STATUS_PERFORMANCE+1)+(int) (subsystem->status & (1<<STATUS_PERFORMANCE)) >> STATUS_PERFORMANCE);
    printf("|RES: %d)\n",(2 * (int) (subsystem->status & (1<<STATUS_RESOURCE+1)) >> STATUS_RESOURCE+1)+(int) (subsystem->status & (1<<STATUS_RESOURCE)) >> STATUS_RESOURCE);
    return ERR_SUCCESS;
}
/*
    Queues new data, and returns the old data of a subsystem

    out: *old_data, the old data in the subsystem before setting the new data. *subsystem, which is a subsystem with its data rewritten to new_data.

    Returns:
        ERR_SUCCESS if it was successfully set
        ERR_NULL_POINTER if the *subsystem is null
*/
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
    if (old_data == NULL || subsystem == NULL){
        return ERR_NULL_POINTER;
    }else{
        *old_data = subsystem->data;
        subsystem->data = new_data;
        subsys_status_set(subsystem, STATUS_DATA, 1);
        return ERR_SUCCESS;
    }
}
/*
    Gets the data queued in a subsystem and clears it

    out: *subsystem, the subsystem with a cleared data value, *data, the data in the subsystem before clearing

    Returns:
        ERR_SUCCESS if it successfully set the data
        ERR_NULL_POINTER if one of the pointers are NULL
        ERR_NO_DATA if there isn't any data to get
*/
int subsys_data_get(Subsystem *subsystem, unsigned int *data){
    if (subsystem == NULL || data == NULL){
        return ERR_NULL_POINTER;
    }
    if ((subsystem->status & (1<<STATUS_DATA)) >> STATUS_DATA == 0){
        data = 0;
        return ERR_NO_DATA;
    }
    *data = subsystem->data;
    subsystem->data = 0;
    subsys_status_set(subsystem, STATUS_DATA, 0);
    return ERR_SUCCESS;
}
/*
    Removes a subsystem in a subsystem collection by index

    out: *subsystems, a subsystem collection without the subsystem slated for removal

    Returns:
        ERR_SUCCESS if it was successfully removed
        ERR_NULL_POINTER if the pointer is NULL
        ERR_INVALID_INDEX if the index is out of bounds
        ERR_NO_DATA if there isn't any data
*/
int subsys_remove(SubsystemCollection *subsystems, int index){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    if(index > subsystems->size || index < 0){
        return ERR_INVALID_INDEX;
    }
    if (subsystems->size == 0){
        return ERR_NO_DATA;
    }
    for(int i = index; i< subsystems->size;i++){
        subsystems->subsystems[i] = subsystems->subsystems[i+1];
    }
    subsystems->size--;
    return ERR_SUCCESS;
}
/*
    returns a filtered subsystem collection based on a given filter

    out: *dest, a subsystem collection with only the filtered subsystems in it

    Returns:
        ERR_SUCCESS if the filter was successfully applied
        ERR_NULL_POINTER if a pointer was NULL

*/
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    if (src == NULL || dest == NULL){
        return ERR_NULL_POINTER;
    }
    unsigned char filt_mask = 0;
    unsigned char wild_mask = 0;
    for(int i = 0; i<8; i++){
        if(filter[i] == '1'){
            filt_mask = (filt_mask | (1<<7-i));
        }
        if(filter[i] == '*'){
            wild_mask = (wild_mask | (1<<7-i));
        }

    }
    filt_mask = ~filt_mask;
    

    for(int j = 0; j<src->size; j++){
        if((filt_mask ^ src->subsystems[j].status|wild_mask) == 0b11111111){
            printf("here\n");
            subsys_append(dest, &src->subsystems[j]);
        } 
    }
    
    return ERR_SUCCESS;
}