
#include "common.h"

int main(int argc, char* argv[])
{
    stratcom_device* device;
    stratcom_init();

    device = stratcom_open_device();
    if(!device) {
        printf("Strategic Commander was not found.\n");
        exit(1);
    }

    printf("Press any button on the Strategic Commander to quit.\n");
    while(!is_any_button_pressed(device)) {
        if(stratcom_read_input(device) == STRATCOM_RET_ERROR) {
            printf("Error: Lost connection to the Strategic Commander.\n");
            exit(1);
        }
        printf("Axis X: %d  --  Y: %d  --  Z: %d\n", 
            stratcom_get_axis_value(device, STRATCOM_AXIS_X),
            stratcom_get_axis_value(device, STRATCOM_AXIS_Y),
            stratcom_get_axis_value(device, STRATCOM_AXIS_Z));
    }

    stratcom_close_device(device);

    stratcom_shutdown();
    return 0;
}

