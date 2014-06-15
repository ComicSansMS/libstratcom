
#include <stdio.h>

#include <stratcom.h>

int main(void)
{
    stratcom_device* dev;

    stratcom_init();

    dev = stratcom_open_device();

    if(!dev) {
        printf("No suitable device was found :(\n");
    } else {
        stratcom_set_button_led_state(dev, STRATCOM_LEDBUTTON_1, STRATCOM_LED_ON);

        stratcom_read_input(dev);

        stratcom_set_button_led_state(dev, STRATCOM_LEDBUTTON_ALL, STRATCOM_LED_OFF);

        stratcom_close_device(dev);
    }

    stratcom_shutdown();

    return 0;
}

