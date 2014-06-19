
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stratcom.h>

int main(void)
{
    stratcom_device* dev;

    stratcom_init();

    dev = stratcom_open_device();

    if(!dev) {
        printf("No suitable device was found :(\n");
    } else {
        stratcom_input_state new_state, old_state;
        memset(&old_state, 0, sizeof(old_state));
        printf("Press the + button on the Strategic Commander to quit.\n");
        while(!stratcom_is_button_pressed(dev, STRATCOM_BUTTON_PLUS))
        {
            int number_of_events;
            stratcom_input_event* events, * events_it;
            if(stratcom_read_input(dev) != STRATCOM_RET_SUCCESS) {
                printf("Error: Device disconnected.\n");
                exit(1);
            }
            new_state = stratcom_get_input_state(dev);
            events = stratcom_create_input_events_from_states(&old_state, &new_state);
            number_of_events = 0;
            events_it = events;
            while(events_it) {
                if((events_it->type == STRATCOM_INPUT_EVENT_BUTTON) && (events_it->desc.button.status)) {
                    stratcom_button_led led;
                    stratcom_led_state led_state, new_led_state;
                    led = stratcom_get_led_for_button(events_it->desc.button.button);
                    led_state = stratcom_get_button_led_state(dev, led);
                    if(led_state == STRATCOM_LED_OFF) {
                        new_led_state = STRATCOM_LED_ON;
                    } else if(led_state == STRATCOM_LED_ON) {
                        new_led_state = STRATCOM_LED_BLINK;
                    } else {
                        new_led_state = STRATCOM_LED_OFF;
                    }
                    stratcom_set_button_led_state(dev, led, new_led_state);
                }
                ++number_of_events;
                events_it = events_it->next;
            }
            printf("%d input event(s).\n", number_of_events);
            stratcom_free_input_events(events);
            old_state = new_state;
        }

        stratcom_set_button_led_state(dev, STRATCOM_LEDBUTTON_ALL, STRATCOM_LED_OFF);

        stratcom_close_device(dev);
    }

    stratcom_shutdown();

    return 0;
}

