#ifndef LIBSTRATCOM_INCLUDE_GUARD_STRATCOM_H_
#define LIBSTRATCOM_INCLUDE_GUARD_STRATCOM_H_

#ifdef _WIN32
#   ifdef LIBSTRATCOM_EXPORT
#       define LIBSTRATCOM_API __declspec(dllexport)
#   else
#       define LIBSTRATCOM_API __declspec(dllimport)
#   endif
#else
#   define LIBSTRATCOM_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    struct stratcom_device_;
    typedef struct stratcom_device_ stratcom_device;

    /** Button LED Identifiers
     */
    typedef enum stratcom_button_led_ {
        STRATCOM_LEDBUTTON_1    = 0x0001,           /**< The LED of the button labeled '1' */
        STRATCOM_LEDBUTTON_2    = 0x0004,           /**< The LED of the button labeled '2' */
        STRATCOM_LEDBUTTON_3    = 0x0010,           /**< The LED of the button labeled '3' */
        STRATCOM_LEDBUTTON_4    = 0x0040,           /**< The LED of the button labeled '4' */
        STRATCOM_LEDBUTTON_5    = 0x0100,           /**< The LED of the button labeled '5' */
        STRATCOM_LEDBUTTON_6    = 0x0400,           /**< The LED of the button labeled '6' */
        STRATCOM_LEDBUTTON_REC  = 0x1000,           /**< The LED of the button labeled 'REC' */
        STRATCOM_LEDBUTTON_ALL  = 0x1555,           /**< Addresses all LEDs at once */
        STRATCOM_LEDBUTTON_NONE = 0x0               /**< Addresses nothing */
    } stratcom_button_led;

    /** LED State
     */
    typedef enum stratcom_led_state_ {
        STRATCOM_LED_ON,                            /**< LED is constantly lit */
        STRATCOM_LED_OFF,                           /**< LED is constantly dark */
        STRATCOM_LED_BLINK                          /**< LED is blinking */
    } stratcom_led_state;

    /** Button Identifiers
     */
    typedef enum stratcom_buttons_ {
        STRATCOM_BUTTON_1      = 0x0001,            /**< The button labeled '1' */
        STRATCOM_BUTTON_2      = 0x0002,            /**< The button labeled '2' */
        STRATCOM_BUTTON_3      = 0x0004,            /**< The button labeled '3' */
        STRATCOM_BUTTON_4      = 0x0008,            /**< The button labeled '4' */
        STRATCOM_BUTTON_5      = 0x0010,            /**< The button labeled '5' */
        STRATCOM_BUTTON_6      = 0x0020,            /**< The button labeled '6' */
        STRATCOM_BUTTON_PLUS   = 0x0040,            /**< The button labeled '+' */
        STRATCOM_BUTTON_MINUS  = 0x0080,            /**< The button labeled '-' */
        STRATCOM_BUTTON_SHIFT1 = 0x0100,            /**< The top shift button */
        STRATCOM_BUTTON_SHIFT2 = 0x0200,            /**< The middle shift button */
        STRATCOM_BUTTON_SHIFT3 = 0x0400,            /**< The lower shift button */
        STRATCOM_BUTTON_REC    = 0x0800,            /**< The button labeled 'REC' */
        STRATCOM_BUTTON_NONE   = 0x0                /**< Refers to no button */
    } stratcom_buttons;
    /** Slider State
     */
    typedef enum stratcom_slider_state_ {
        STRATCOM_SLIDER_UNKNOWN = 0,                /**< Slider position is unknown */
        STRATCOM_SLIDER_1,                          /**< Slider set to the position marked with one circle */
        STRATCOM_SLIDER_2,                          /**< Slider set to the position marked with two circles */
        STRATCOM_SLIDER_3                           /**< Slider set to the position marked with three circles */
    } stratcom_slider_state;
    /** Axis Identifier
     */
    typedef enum stratcom_axis_ {
        STRATCOM_AXIS_X,                            /**< Horizontal X-axis */
        STRATCOM_AXIS_Y,                            /**< Vertical Y-axis */
        STRATCOM_AXIS_Z                             /**< Rotational Z-axis */
    } stratcom_axis;

    LIBSTRATCOM_API void stratcom_init();

    LIBSTRATCOM_API void stratcom_shutdown();

    LIBSTRATCOM_API stratcom_device* stratcom_open_device();

    LIBSTRATCOM_API stratcom_device* stratcom_open_device_on_path(char const* device_path);

    LIBSTRATCOM_API void stratcom_close_device(stratcom_device* device);

    LIBSTRATCOM_API void stratcom_set_button_led_state(stratcom_device* device,
                                                       stratcom_button_led led, stratcom_led_state state);

    LIBSTRATCOM_API void stratcom_set_button_led_state_without_flushing(stratcom_device* device,
                                                                        stratcom_button_led led,
                                                                        stratcom_led_state state);

    LIBSTRATCOM_API void stratcom_flush_button_led_state(stratcom_device* device);

    LIBSTRATCOM_API int stratcom_led_state_has_unflushed_changes(stratcom_device* device);

    LIBSTRATCOM_API void stratcom_set_led_blink_interval(stratcom_device* device,
                                                         unsigned char on_time, unsigned char off_time);

    LIBSTRATCOM_API void stratcom_read_input(stratcom_device* device);

    LIBSTRATCOM_API int stratcom_read_input_non_blocking(stratcom_device* device);

#ifdef __cplusplus
}
#endif

#endif
