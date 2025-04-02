#include <main.h>

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    gpio_init(); /* Initialize the GPIO */
    flash_init(); /* Initialize the flash */

    display_init(); /* Initialize the display and UI */
    ina219_init(); /* Initialize the INA219 sensor */
    adc_init(); /* Initialize the ADC input device */
    encoder_lvgl_init(); /* Initialize the encoder input device */
    lvgl_group_init(); /* Initialize the LVGL group */

    load_web_config(); /* Load the web configuration */

    wificonnect(); /* Connect to the wifi */
    time_server_init(NTPServer.c_str(), TimeZone, SyncTime); /* Initialize the time server */
    weather_init(qWeather_Key, CityCode, qWeather_ApiHost); /* Initialize the weather server */
}

void loop()
{
    display_task(); /* let the GUI do its work */
    time_server_update();
    wificonfig();
    deep_sleep();
}
