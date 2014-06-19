
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

    // leds can be set either separately
    stratcom_set_button_led_state(device, STRATCOM_LEDBUTTON_1, STRATCOM_LED_ON);
    stratcom_set_button_led_state(device, STRATCOM_LEDBUTTON_3, STRATCOM_LED_ON);
    stratcom_set_button_led_state(device, STRATCOM_LEDBUTTON_5, STRATCOM_LED_ON);
    stratcom_set_button_led_state(device, STRATCOM_LEDBUTTON_REC, STRATCOM_LED_BLINK);

    wait_for_button_press(device);

    // ... or all at once
    stratcom_set_button_led_state(device, STRATCOM_LEDBUTTON_ALL, STRATCOM_LED_OFF);

    stratcom_close_device(device);

    stratcom_shutdown();
    return 0;
}

