/******************************************************************************
 * Copyright (c) 2010 Andreas Weis <der_ghulbus@ghulbus-inc.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/

/** @file
 * Main Header File.
 * libstratcom - 
 *  A Library for interacting with Microsoft SideWinder Strategic Commander
 *
 * @author Andreas Weis
 *
 */
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct stratcom_device_;
    /** Opaque device structure.
     * Maintains the internal state for interacting with the device.
     */
    typedef struct stratcom_device_ stratcom_device;


    /** @name Identifiers.
     * Identifiers are types used for identifying certain parts of the device, such as buttons or LEDs.
     * @{
     */

    /** Button Identifiers.
     * This type is used for querying the state of a button.
     * Iterating over all button identifiers can be achieved with the stratcom_iterate_buttons_range* functions.
     * @see stratcom_is_button_pressed(), stratcom_iterate_buttons_range_begin(),
     *      stratcom_iterate_buttons_range_end(), stratcom_iterate_buttons_range_increment()
     */
    typedef enum stratcom_button_ {
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
    } stratcom_button;

    /** Button LED Identifiers.
     * This type is used for setting or querying the state of the button LEDs.
     * You can obtain the matching LED identifier from a given stratcom_button by calling stratcom_get_led_for_button().
     * @see stratcom_get_button_led_state(), stratcom_set_button_led_state(), stratcom_get_led_for_button()
     */
    typedef enum stratcom_button_led_ {
        STRATCOM_LEDBUTTON_1    = 0x0001,           /**< The LED of the button labeled '1'. */
        STRATCOM_LEDBUTTON_2    = 0x0004,           /**< The LED of the button labeled '2'. */
        STRATCOM_LEDBUTTON_3    = 0x0010,           /**< The LED of the button labeled '3'. */
        STRATCOM_LEDBUTTON_4    = 0x0040,           /**< The LED of the button labeled '4'. */
        STRATCOM_LEDBUTTON_5    = 0x0100,           /**< The LED of the button labeled '5'. */
        STRATCOM_LEDBUTTON_6    = 0x0400,           /**< The LED of the button labeled '6'. */
        STRATCOM_LEDBUTTON_REC  = 0x1000,           /**< The LED of the button labeled 'REC'. */
        STRATCOM_LEDBUTTON_ALL  = 0x1555,           /**< Addresses all LEDs at once. */
        STRATCOM_LEDBUTTON_NONE = 0x0               /**< Addresses nothing. */
    } stratcom_button_led;

    /** Axis Identifier.
     * This type is used for querying the state of one of the three device axes.
     * @see stratcom_get_axis_value()
     */
    typedef enum stratcom_axis_ {
        STRATCOM_AXIS_X,                            /**< Horizontal X-axis. */
        STRATCOM_AXIS_Y,                            /**< Vertical Y-axis. */
        STRATCOM_AXIS_Z                             /**< Rotational Z-axis. */
    } stratcom_axis;

    /** @} */


    /** @name State.
     * State types are used for modeling the current state of a certain part of the device for input,
     * such as whether button is pressed or where an axis is positioned.
     * @{
     */

    /** LED State.
     */
    typedef enum stratcom_led_state_ {
        STRATCOM_LED_ON,                            /**< LED is constantly lit. */
        STRATCOM_LED_OFF,                           /**< LED is constantly dark. */
        STRATCOM_LED_BLINK                          /**< LED is blinking. */
    } stratcom_led_state;

    /** Slider State.
     * The slider may rest in one of three positions.
     * For cases where the state of the slider cannot be determined we use STRATCOM_SLIDER_UNKNOWN.
     */
    typedef enum stratcom_slider_state_ {
        STRATCOM_SLIDER_UNKNOWN = 0,                /**< Slider position is unknown. */
        STRATCOM_SLIDER_1,                          /**< Slider set to the position marked with one circle. */
        STRATCOM_SLIDER_2,                          /**< Slider set to the position marked with two circles. */
        STRATCOM_SLIDER_3                           /**< Slider set to the position marked with three circles. */
    } stratcom_slider_state;

    /** Button Word.
     * A bitwise combination of @ref stratcom_button flags indicating the
     * state of all buttons on the device. If a bit is set, it means
     * that the corresponding button is pressed.
     */
    typedef uint16_t stratcom_button_word;
    /** Axis Word.
     * Represents the state of a single axis.
     *  Values range from -512 to +511 where 0 is the axis center.
     */
    typedef int16_t stratcom_axis_word;
    /** Combined state of all buttons and axes on the device.
     * This type models the complete state of the device at a fixed point in time.
     * The representation of the values in this type is very compact and you might
     * want to prefer to use one of the more specialized library functions for
     * querying the state of a certain part.
     * Input states are most useful for creating input events from a pair of
     * states, using stratcom_create_input_events_from_states().
     * @see stratcom_get_input_state(), stratcom_create_input_events_from_states()
     */
    typedef struct stratcom_input_state_ {
        stratcom_button_word  buttons;           /**< Combined state of all buttons. */
        stratcom_slider_state slider;            /**< State of the slider. */
        stratcom_axis_word    axisX;             /**< X-axis state (-512: full left; 0: center; +511: full right). */
        stratcom_axis_word    axisY;             /**< Y-axis state (-512: full up;   0: center; +511: full down). */
        stratcom_axis_word    axisZ;             /**< Z-axis state (-512: full left; 0: center; +511: full right). */
    } stratcom_input_state;

    /** @} */


    /** @name Input Events.
     * While input state models the state of the device at a single point in time,
     * input events model the transition from one state to another.
     * Typical input events include a button being pressed or released or an axis
     * changing its position.
     * @{
     */

    /** Button event.
     * A button being pressed or released.
     */
    typedef struct stratcom_input_event_button_ {
        stratcom_button button;                  /**< The button this event refers to. */
        int status;                              /**< 1 if the button is pressed. 0 if it is released. */
    } stratcom_input_event_button;

    /** Slider event.
     * The slider changed to a different position.
     */
    typedef struct stratcom_input_event_slider_ {
        stratcom_slider_state status;            /**< New position of the slider. */
    } stratcom_input_event_slider;

    /** Axis event.
     * One of the axis changed to a different position.
     */
    typedef struct stratcom_input_event_axis_ {
        stratcom_axis axis;                      /**< Axis this event refers to. */
        stratcom_axis_word status;               /**< New position of the axis. */
    } stratcom_input_event_axis;

    /** Different input types that can form a stratcom_input_event.
     */
    typedef enum stratcom_input_event_type_ {
        STRATCOM_INPUT_EVENT_BUTTON,             /**< Button event. @see stratcom_input_event_button */
        STRATCOM_INPUT_EVENT_SLIDER,             /**< Slider event. @see stratcom_input_event_slider */
        STRATCOM_INPUT_EVENT_AXIS                /**< Axis event. @see stratcom_input_event_axis */
    } stratcom_input_event_type;

    /** Input event structure.
     * Input events form a linked list where each element contains one specific input event.
     * @see stratcom_create_input_events_from_states(), stratcom_free_input_events()
     */
    typedef struct stratcom_input_event_ {
        stratcom_input_event_type type;          /**< Type of the contained input event. */
        /** The input event union.
         * Which type of the union is valid is determined by the @ref stratcom_input_event::type field.
         */
        union event_desc_T {
            stratcom_input_event_button button;  /**< Button event. */
            stratcom_input_event_slider slider;  /**< Slider event. */
            stratcom_input_event_axis axis;      /**< Axis event. */
        } desc;                                  /**< Input event descriptor. */
        struct stratcom_input_event_* next;      /**< Pointer to the next element in the linked list structure.
                                                      For the last element in the list this is NULL. */
    } stratcom_input_event;

    /** @} */

    /** Return type.
     * This return type is used on functions that can fail to indicate runtime errors.
     */
    typedef enum stratcom_return_ {
        STRATCOM_RET_SUCCESS = 0,                /**< The function completed successfully. */
        STRATCOM_RET_ERROR   = (-1),             /**< An error occured while executing. */
        STRATCOM_RET_TIMEOUT = (-2)              /**< Timed out while waiting for input from the device. */
    } stratcom_return;

    /** @name Initialization and Shutdown.
     * @{
     */

    /** Initialize the libstratcom library.
     * Call this function once per process before invoking any other function of the library.
     * Call stratcom_shutdown() before exiting your program to free the static data allocated by this function.
     * @returns STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_shutdown()
     */
    LIBSTRATCOM_API stratcom_return stratcom_init();

    /** Finalize the libstratcom library.
     * This function frees all static data allocated by stratcom_init().
     * Call it once at the end of the program to prevent memory leaks.
     * @see stratcom_init()
     */
    LIBSTRATCOM_API void stratcom_shutdown();

    /** Open a Strategic Commander device for use with the library.
     * This function will automatically attempt to open the first device with matching HID vendor and product ids.
     * Usually this just works. In case you need more control over which device is opened (for instance when
     * having more than one device attached to the same machine), use @ref stratcom_open_device_on_path() instead.
     * @return Pointer to a device struct on success, which can be freed by calling stratcom_close_device().
     *         NULL in case of error.
     * @note The Strategic Commander is identified as the first device with an HID Vendor Id of \c 0x045e and
     *       a Product Id of \c 0x0033.
     * @note The internal state of the LEDs and LED blink intervals are set to match the state of the physical device.
     *       The input state however is left uninitialized and must be queried manually by calling one of the
     *       \c stratcom_read_input* functions.
     * @see stratcom_open_device_on_path(), stratcom_close_device()
     */
    LIBSTRATCOM_API stratcom_device* stratcom_open_device();

    /** Open a Strategic Commander device on a certain HID device path.
     * @param[in] device_path The HID path of the device to open.
     * @return Pointer to a device struct on success, which can be freed by calling stratcom_close_device().
     *         NULL in case of error.
     * @note The internal state of the LEDs and LED blink intervals are set to match the state of the physical device.
     *       The input state however is left uninitialized and must be queried manually by calling one of the
     *       \c stratcom_read_input* functions.
     * @see stratcom_close_device()
     */
    LIBSTRATCOM_API stratcom_device* stratcom_open_device_on_path(char const* device_path);

    /** Close a device.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @see stratcom_open_device(), stratcom_open_device_on_path()
     */
    LIBSTRATCOM_API void stratcom_close_device(stratcom_device* device);

    /** @} */

    /** @name Button LEDs.
     * @{
     */

    /** Retrieve the current state of a specific LED.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @param[in] led The button LED which is to be queried.
     * @return State of the queried LED.
     * @note The state returned by this function represents the internal state of the stratcom_device object,
     * which might diverge from the actual state of the physical device (for example after setting the led state
     * without flushing the changes to the device). To force the internal state to synchronize with the physical
     * device, call either stratcom_flush_button_led_state() or stratcom_read_button_led_state().
     * @see stratcom_set_button_led_state()
     */
    LIBSTRATCOM_API stratcom_led_state stratcom_get_button_led_state(stratcom_device* device,
                                                                     stratcom_button_led led);

    /** Set the state of a specific LED.
     * This function changes the state of an LED in both the internal state and flushes the internal state to
     * the physical device. This will result in sending a feature report to the device.
     * When setting multiple LEDs at once, consider using stratcom_set_button_led_state_without_flushing()
     * and stratcom_flush_button_led_state() instead, to avoid having to send multiple feature reports in a row.
     * Note though, that setting STRATCOM_LEDBUTTON_ALL with this function will also only send a single feature report.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @param[in] led The button LED which is to be changed.
     * @param[in] state Requested new state of the LED.
     * @return STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_set_button_led_state_without_flushing(), stratcom_flush_button_led_state(),
     *      stratcom_get_button_led_state()
     */
    LIBSTRATCOM_API stratcom_return stratcom_set_button_led_state(stratcom_device* device,
                                                                  stratcom_button_led led, stratcom_led_state state);

    /** Set the state of a specific LED in the internal state.
     * This function changes the state of an LED in the internal state only. To apply the change made by this
     * function to the physical device you have to flush the changes by calling either stratcom_flush_button_led_state()
     * or stratcom_set_button_led_state().
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @param[in] led The button LED which is to be changed.
     * @param[in] state Requested new state of the LED.
     * @see stratcom_set_button_led_state(), stratcom_flush_button_led_state(),
     *      stratcom_led_state_has_unflushed_changes(), stratcom_get_button_led_state()
     */
    LIBSTRATCOM_API void stratcom_set_button_led_state_without_flushing(stratcom_device* device,
                                                                        stratcom_button_led led,
                                                                        stratcom_led_state state);

    /** Flush the current internal LED state to the physical device.
     * This will send a feature report to the device to update the state of the button LEDs.
     * In case of successful execution, all button LEDs will light up according to the internal state.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @return STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_set_button_led_state_without_flushing(), stratcom_led_state_has_unflushed_changes()
     */
    LIBSTRATCOM_API stratcom_return stratcom_flush_button_led_state(stratcom_device* device);

    /** Check whether the internal LED state contains unflushed changes.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @return 1 if the internal state contains unflushed changes, 0 otherwise.
     * @note This function does not query the physical device for its state. It simply accounts for changes made
     *       through stratcom_set_button_led_state_without_flushing() that have not been flushed.
     * @see stratcom_set_button_led_state_without_flushing(), stratcom_flush_button_led_state()
     */
    LIBSTRATCOM_API int stratcom_led_state_has_unflushed_changes(stratcom_device* device);

    /** Retrieve the current blink intervals from the internal state.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @param[out] out_on_time Time that the LED is lit when blinking.
     * @param[out] out_off_time Time that the LED is dark when blinking.
     * @see stratcom_set_led_blink_interval(), stratcom_read_led_blink_intervals()
     */
    LIBSTRATCOM_API void stratcom_get_led_blink_interval(stratcom_device* device,
                                                         unsigned char* out_on_time, unsigned char* out_off_time);

    /** Set the blink intervals for blinking LEDs.
     * This function will send a feature report to update the blink state on the physical device.
     * The blink intervals are the same for all buttons.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @param[in] on_time Time that the LED is lit when blinking.
     * @param[in] off_time Time that the LED is dark when blinking.
     * @return STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_get_led_blink_interval(), stratcom_read_led_blink_intervals()
     */
    LIBSTRATCOM_API stratcom_return stratcom_set_led_blink_interval(stratcom_device* device,
                                                                    unsigned char on_time, unsigned char off_time);

    /** Read the button led state from the physical device.
     * Upon successful execution, this function will overwrite the internal led state with the values
     * obtained from the physical device. This function will cause a feature report to be read from the device.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @return STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_get_button_led_state()
     */
    LIBSTRATCOM_API stratcom_return stratcom_read_button_led_state(stratcom_device* device);

    /** Read the led blink intervals from the physical device.
     * Upon successful execution, this function will overwrite the internal state blink intervals with the values
     * obtained from the physical device. This function will cause a feature report to be read from the device.
     * @param[in] device A device structure returned from stratcom_open_device() or stratcom_open_device_on_path().
     * @return STRATCOM_RET_SUCCESS on success, STRATCOM_RET_ERROR on error.
     * @see stratcom_get_button_led_state()
     */
    LIBSTRATCOM_API stratcom_return stratcom_read_led_blink_intervals(stratcom_device* device);

    /** Obtain the matching LED for a given button.
     * @param[in] button Button of which to retrieve the LED.
     * @return The corresponding LED of the button. STRATCOM_LEDBUTTON_NONE if the button does not have a LED.
     */
    LIBSTRATCOM_API stratcom_button_led stratcom_get_led_for_button(stratcom_button button);

    /** @} */

    /** @name Device Input.
     * @{
     */

    LIBSTRATCOM_API stratcom_return stratcom_read_input(stratcom_device* device);

    LIBSTRATCOM_API stratcom_return stratcom_read_input_with_timeout(stratcom_device* device, int timeout_milliseconds);

    LIBSTRATCOM_API stratcom_return stratcom_read_input_non_blocking(stratcom_device* device);

    LIBSTRATCOM_API stratcom_input_state stratcom_get_input_state(stratcom_device* device);

    LIBSTRATCOM_API int stratcom_is_button_pressed(stratcom_device* device, stratcom_button button);

    LIBSTRATCOM_API stratcom_axis_word stratcom_get_axis_value(stratcom_device* device, stratcom_axis axis);

    LIBSTRATCOM_API stratcom_slider_state stratcom_get_slider_state(stratcom_device* device);

    /** @} */

    /** @name Iterating Button Identifiers.
     * @{
     */

    LIBSTRATCOM_API stratcom_button stratcom_iterate_buttons_range_begin();

    LIBSTRATCOM_API stratcom_button stratcom_iterate_buttons_range_end();

    LIBSTRATCOM_API stratcom_button stratcom_iterate_buttons_range_increment(stratcom_button button);

    /** @} */

    /** @name Input Events.
     * @{
     */

    LIBSTRATCOM_API stratcom_input_event* stratcom_create_input_events_from_states(stratcom_input_state* old_state,
                                                                                   stratcom_input_state* new_state);

    LIBSTRATCOM_API void stratcom_free_input_events(stratcom_input_event* events);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif
