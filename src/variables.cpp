#include <variables.h>

//INA219
float voltage = 0.0f;
float current = 0.0f;
float power = 0.0f;
lv_timer_t* ina219_timer = NULL;
//SEETING SCREEN//
uint8_t brightness = 0;
uint16_t sleep_time = 0;
bool fan_switch = false;
uint8_t free_space;
//ADC ADJUST//
float voltage0 = 0.0f;
float voltage1 = 0.0f;
float voltage2 = 0.0f;
float voltage3 = 0.0f;
float voltage0_adc = 10.85f;
float voltage1_adc = 10.85f;
float voltage2_adc = 10.85f;
float voltage3_adc = 10.85f;
lv_timer_t* adc_timer = NULL;
//MAIN SCREEN//
bool USBC_Switch = false;
bool USBA_Switch = false;
//FOR WEB AND WEATHER//
uint16_t UpdateWeater_Time = 10; //10分钟更新
String CityCode = "101280101"; //广州
String qWeather_ApiHost = "";
String qWeather_Key = "";
uint8_t TimeZone = 8;
uint8_t SyncTime = 1;
String NTPServer = "pool.ntp.org";
config_type wifisetting ={{""},{""}};
bool wificonfig_flag = false;