#include <stratcom.h>

#include <stdio.h>
#include <stdlib.h>

int is_any_button_pressed(stratcom_device* device)
{
    stratcom_button b;
    for(b = stratcom_iterate_buttons_range_begin(); b != stratcom_iterate_buttons_range_end();
        b = stratcom_iterate_buttons_range_increment(b))
    {
        if(stratcom_is_button_pressed(device, b)) {
            return 1;
        }
    }
    return 0;
}

void wait_for_button_press(stratcom_device* device)
{
    printf("Press any button on the Strategic Commander to continue.\n");
    do {
        if(stratcom_read_input(device) == STRATCOM_RET_ERROR) {
            printf("Error: Lost connection to the Strategic Commander.\n");
            exit(1);
        }
    } while(!is_any_button_pressed(device));
}

