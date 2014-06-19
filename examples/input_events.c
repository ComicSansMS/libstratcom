
#include "common.h"

int main(int argc, char* argv[])
{
    stratcom_device* device;
    stratcom_input_state old_input_state;
    stratcom_init();

    device = stratcom_open_device();
    if(!device) {
        printf("Strategic Commander was not found.\n");
        exit(1);
    }

    printf("Press the REC button on the Strategic Commander to quit.\n");
    old_input_state = stratcom_get_input_state(device);
    while(stratcom_read_input(device) == STRATCOM_RET_SUCCESS) {
        stratcom_input_state current_input_state = stratcom_get_input_state(device);
        stratcom_input_event* input_events = stratcom_create_input_events_from_states(&old_input_state,
                                                                                      &current_input_state);
        // input_events is a linked list of events;
        // traversing it gives a detailed description of what caused
        // the latest call to stratcom_read_input() to return.
        stratcom_input_event* input_it;
        for(input_it = input_events; input_it != NULL; input_it = input_it->next) {
            switch(input_it->type) {
            case STRATCOM_INPUT_EVENT_AXIS:
                switch(input_it->desc.axis.axis) {
                case STRATCOM_AXIS_X:
                    printf("X");
                    break;
                case STRATCOM_AXIS_Y:
                    printf("Y");
                    break;
                case STRATCOM_AXIS_Z:
                    printf("Z");
                    break;
                }
                printf("-Axis moved to %d.\n", input_it->desc.axis.status);
                break;
            case STRATCOM_INPUT_EVENT_BUTTON:
                printf("Button ");
                switch(input_it->desc.button.button) {
                case STRATCOM_BUTTON_1: printf("1"); break;
                case STRATCOM_BUTTON_2: printf("2"); break;
                case STRATCOM_BUTTON_3: printf("3"); break;
                case STRATCOM_BUTTON_4: printf("4"); break;
                case STRATCOM_BUTTON_5: printf("5"); break;
                case STRATCOM_BUTTON_6: printf("6"); break;
                case STRATCOM_BUTTON_PLUS: printf("+"); break;
                case STRATCOM_BUTTON_MINUS: printf("-"); break;
                case STRATCOM_BUTTON_SHIFT1: printf("Shift1"); break;
                case STRATCOM_BUTTON_SHIFT2: printf("Shift2"); break;
                case STRATCOM_BUTTON_SHIFT3: printf("Shift3"); break;
                case STRATCOM_BUTTON_REC:  printf("REC"); break;
                }
                printf(" was ");
                if(input_it->desc.button.status) {
                    printf("pressed.\n");
                } else {
                    printf("released.\n");
                }
                break;
            case STRATCOM_INPUT_EVENT_SLIDER:
                printf("Slider changed position to %d.\n", input_it->desc.slider.status);
                break;
            }
        }

        // do not forget to free the input list after use
        stratcom_free_input_events(input_events);

        // update the old_input_state for the next loop iteration
        old_input_state = current_input_state;

        if(stratcom_is_button_pressed(device, STRATCOM_BUTTON_REC)) break;
    }

    stratcom_close_device(device);

    stratcom_shutdown();
    return 0;
}

