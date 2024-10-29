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

    printf("name: %16s, ", subsystem->name);
    subsys_status_print(subsystem);
    return ERR_SUCCESS;
}

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value){
    if(value > 3 || value < 0){
        return ERR_INVALID_STATUS;
    }
    if(status > STATUS_POWER || status < STATUS_RESOURCE){
        return ERR_INVALID_STATUS;
    }
    for (int i = 0; i < 8; i++)
    {
        printf("%d", (subsystem->status & (1<<i)) >> i);
    }
    printf(" prefunction\n");
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
    for (int i = 0; i < 8; i++)
    {
        printf("%d", (subsystem->status & (1<<i)) >> i);
    }
    printf("\n");
    printf("Status: (PwR: %d", ((subsystem->status & (1<<STATUS_POWER)) >> STATUS_POWER));
    printf("|DATA: %d",((subsystem->status & (1<<STATUS_DATA)) >> STATUS_DATA));
    printf("|ACT: %d",((subsystem->status & (1<<STATUS_ACTIVITY)) >> STATUS_ACTIVITY));
    printf("|ERROR: %d",((subsystem->status & (1<<STATUS_ERROR)) >> STATUS_ERROR));
    printf("|PERF: %d",(2 * (int) (subsystem->status & (1<<STATUS_PERFORMANCE+1)) >> STATUS_PERFORMANCE+1)+(int) (subsystem->status & (1<<STATUS_PERFORMANCE)) >> STATUS_PERFORMANCE);
    printf("|RES: %d)\n",(2 * (int) (subsystem->status & (1<<STATUS_RESOURCE+1)) >> STATUS_RESOURCE+1)+(int) (subsystem->status & (1<<STATUS_RESOURCE)) >> STATUS_RESOURCE);
    for (int i = 0; i < 8; i++)
    {
        printf("%d", (subsystem->status & (1<<i)) >> i);
    }
    printf("\n");
    return ERR_SUCCESS;

}