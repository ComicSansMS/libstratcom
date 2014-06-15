#include <stratcom.h>

#include <hidapi.h>

#include <memory>
#include <cstdint>
#include <vector>

namespace {
    /*  Magic Constants
     */
    const unsigned short HID_VENDOR_ID  = 0x045e;
    const unsigned short HID_PRODUCT_ID = 0x0033;
    /***/
}

template<typename T, void(*Deleter)(T*)>
class hidapi_resource_wrapper
{
private:
    std::unique_ptr<T, void(*)(T*)> m_ptr;
public:
    hidapi_resource_wrapper()
        :m_ptr(nullptr, [](T*) {})
    {
    }

    hidapi_resource_wrapper(T* dev)
        :m_ptr(dev, Deleter)
    {
    }

    operator T*()
    {
        return m_ptr.get();
    }

    operator T const*() const
    {
        return m_ptr.get();
    }

    T* operator->()
    {
        return m_ptr.get();
    }

    T const* operator->() const
    {
        return m_ptr.get();
    }

    bool operator!() const
    {
        return !m_ptr;
    }
};

typedef hidapi_resource_wrapper<hid_device, hid_close> hid_device_wrapper;
typedef hidapi_resource_wrapper<hid_device_info, hid_free_enumeration> hid_device_info_wrapper;

struct stratcom_device_ {
    hid_device_wrapper device;
    std::uint16_t led_button_state;
    bool led_button_state_has_unflushed_changes;

    stratcom_device(hid_device* dev)
        :device(dev), led_button_state(0), led_button_state_has_unflushed_changes(true)
    {
    }
};

struct feature_report {
    std::uint8_t b0;
    std::uint8_t b1;
    std::uint8_t b2;
};

void stratcom_init()
{
    if(hid_init() != 0)
    {
        // error
    }
}

void stratcom_shutdown()
{
    if(hid_exit() != 0)
    {
        // error
    }
}

stratcom_device* stratcom_open_device()
{
    hid_device_info_wrapper dev_info_list(hid_enumerate(HID_VENDOR_ID, HID_PRODUCT_ID));
    stratcom_device* ret = nullptr;
    if(dev_info_list) {
        ret = stratcom_open_device_on_path(dev_info_list->path);
    }
    return ret;
}

stratcom_device* stratcom_open_device_on_path(char const* device_path)
{
    auto dev = hid_open_path(device_path);
    if (dev) {
        return new stratcom_device(dev);
    }
    return nullptr;
}

void stratcom_close_device(stratcom_device* device)
{
    delete device;
}

void stratcom_set_button_led_state(stratcom_device* device,
                                   stratcom_button_led led, stratcom_led_state state)
{
    stratcom_set_button_led_state_without_flushing(device, led, state);
    stratcom_flush_button_led_state(device);
}

void stratcom_set_button_led_state_without_flushing(stratcom_device* device,
                                                    stratcom_button_led led,
                                                    stratcom_led_state state)
{
    /*
     * LEDs are set by sending a feature report of the following form:
     * b0 = 0x01
     * b1 = LED state Buttons 1-4
     * b1 = LED state Buttons 1-6
     * Each button occupies two bits: A low bit that indicates a lit LED and a higher bit indicating
     * a blinking LED. Those are mutually exclusive, attempting to set both will result in the 
     * feature report being rejected.
     * Therefore, the class keeps track of LED states in the led_button_state member which mimics the
     * bitmask send to the device.
     * Values of the ButtonLed_T correspond to the bitmasks for LED On bits in led_button_state.
     */
    auto const button_mask = static_cast<std::uint16_t>(led);
    switch(state) {
    case STRATCOM_LED_BLINK:
        device->led_button_state &= (~button_mask);                //< clear LED_ON bits
        device->led_button_state |= (button_mask << 1);            //< set LED_BLINK bits
        break;
    case STRATCOM_LED_ON:
        device->led_button_state &= (~(button_mask << 1));         //< clear LED_BLINK bits
        device->led_button_state |= button_mask;                   //< set LED_ON bits
        break;
    case STRATCOM_LED_OFF:
        device->led_button_state &= (~button_mask);                //< clear LED_ON bits
        device->led_button_state &= (~(button_mask << 1));         //< clear LED_BLINK bits
        break;
    }
    device->led_button_state_has_unflushed_changes = true;
}

void stratcom_flush_button_led_state(stratcom_device* device)
{
    feature_report report;
    report.b0 = 0x01;
    report.b1 = (device->led_button_state & 0xff);
    report.b2 = ((device->led_button_state >> 8) & 0xff);
    hid_send_feature_report(device->device, &report.b0, sizeof(report));
    device->led_button_state_has_unflushed_changes = false;
}

int stratcom_led_state_has_unflushed_changes(stratcom_device* device)
{
    return device->led_button_state_has_unflushed_changes;
}

