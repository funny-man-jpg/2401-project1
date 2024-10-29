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

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
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
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data){
    if (old_data == NULL){
        return ERR_NULL_POINTER;
    }else{
        *old_data = subsystem->data;
        subsystem->data = new_data;
        subsys_status_set(subsystem, STATUS_DATA, 1);
        return ERR_SUCCESS;
    }
}
int subsys_data_get(Subsystem *subsystem, unsigned int *data){
    if ((subsystem->status & (1<<STATUS_DATA)) >> STATUS_DATA == 0){
        data = 0;
        return ERR_NO_DATA;
    }
    *data = subsystem->data;
    subsystem->data = 0;
    subsys_status_set(subsystem, STATUS_DATA, 0);
    return ERR_SUCCESS;
}