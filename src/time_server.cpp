#include <time_server.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Weather weather;

lv_timer_t* NTP_timer = NULL;
lv_timer_t* NTP_Update_timer = NULL;

void time_server_init(const char* poolServerName, long timeOffset, unsigned long updateInterval)
{
    timeClient.setPoolServerName(poolServerName);    
    timeClient.setTimeOffset(timeOffset * 3600);
    timeClient.setUpdateInterval(updateInterval * 3600000);
    timeClient.begin();
    NTP_timer = lv_timer_create(time_server_refresh, 500, NULL);
}

void time_server_setting(const char* poolServerName, long timeOffset, unsigned long updateInterval)
{
    timeClient.setPoolServerName(poolServerName);
    timeClient.setTimeOffset(timeOffset * 3600);
    timeClient.setUpdateInterval(updateInterval * 3600000);
}

uint8_t hour, minute, second, week;
int reconnect_timer = 0;
void time_server_refresh(lv_timer_t *timer)
{
    if(timeClient.isTimeSet() == true)
    {
        hour = timeClient.getHours();
        minute = timeClient.getMinutes();
        second = timeClient.getSeconds();

        lv_label_set_text_fmt(ui_Hour, "%02d", hour);
        lv_label_set_text_fmt(ui_Minute, "%02d", minute);
        lv_label_set_text_fmt(ui_Second, "%02d", second);
        
        lv_label_set_text_fmt(ui_WHour, "%02d", hour);
        lv_label_set_text_fmt(ui_WMinute, "%02d", minute);
        lv_label_set_text_fmt(ui_WSecond, "%02d", second);
    }
}

int reconnect_wifi_timer = 0;
int reconnect_wifi_wait = 0;
int reconnect_ui_timer = 0;
bool time_server_update_flag;
void time_server_update()
{
    if(WiFi.status() == WL_CONNECTED)
    {
        time_server_update_flag = timeClient.update();
        reconnect_wifi_timer = millis();
        reconnect_ui_timer = millis();
        if(time_server_update_flag == true)
        {
        weather_update();

        week = timeClient.getDay();
        hour = timeClient.getHours();
        minute = timeClient.getMinutes();
        second = timeClient.getSeconds();
        lv_label_set_text_fmt(ui_SYNCTIME, "%02d:%02d:%02d", hour, minute, second);
            switch (week)
        {
        case 0:
            lv_label_set_text(ui_Week, "周日");
            break;
        case 1:
            lv_label_set_text(ui_Week, "周一");
            break;
        case 2:
            lv_label_set_text(ui_Week, "周二");
            break;
        case 3:
            lv_label_set_text(ui_Week, "周三");
            break;
        case 4:
            lv_label_set_text(ui_Week, "周四");
            break;
        case 5:
            lv_label_set_text(ui_Week, "周五");
            break;
        case 6:
            lv_label_set_text(ui_Week, "周六");
            break;
        }
        }
    }
    else
    {
        if(millis() - reconnect_ui_timer > 1000)
        {
            lv_label_set_text(ui_WIFIStatus, "未连接");
            lv_label_set_text(ui_SSID, "NONE");
            lv_label_set_text(ui_IPADDR, "NONE");
            lv_label_set_text(ui_SYNCTIME, "00:00:00");
            lv_label_set_text(ui_TextWIFIStart, "启动无线配网");
            lv_img_set_src(ui_ImageWiFi, &ui_img_1338783594);
    
            extern const lv_img_dsc_t* get_weather_icon(int code);
            const lv_img_dsc_t* weather_icon = get_weather_icon(999);
          
            lv_img_set_src(ui_WeatherICON, weather_icon);
            lv_label_set_text(ui_Temperature, "000℃");
            lv_label_set_text(ui_Humidity, "000%");
            lv_label_set_text(ui_WeatherCHN, weather.getWeatherText().c_str());
    
            if(timeClient.isTimeSet() == false)
            {
                lv_label_set_text(ui_Week, "错误");
            }
            lv_label_set_text(ui_WeatherCHN, "错误");
            reconnect_ui_timer = millis();
        }

        if(millis() - reconnect_wifi_timer > 60000)
        {
            reconnect_wifi_wait = millis();
            WiFi.begin(wifisetting.sta_ssid, wifisetting.sta_pwd);
            while(millis() - reconnect_wifi_wait < 5000)
            {   
                lv_task_handler();
            }

            if(WiFi.status() == WL_CONNECTED)
            {
                lv_label_set_text(ui_WIFIStatus, "已连接");
                lv_label_set_text(ui_TextWIFIStart, "更新天气时间");
                lv_label_set_text(ui_SSID, WiFi.SSID().c_str());
                lv_label_set_text(ui_IPADDR, WiFi.localIP().toString().c_str());

                lv_img_set_src(ui_ImageWiFi, &ui_img_593743026);
                time_server_forceupdate();
                weather_update();
            }
            else
            {
                reconnect_wifi_timer = millis();
                WiFi.disconnect();
            }
        }
    }
}

void time_server_forceupdate()
{
    if(WiFi.status() == WL_CONNECTED && timeClient.isTimeSet() == false)
    {
        timeClient.forceUpdate();
        int week_temp = timeClient.getDay();
        int hour = timeClient.getHours();
        int minute = timeClient.getMinutes();
        int second = timeClient.getSeconds();
        lv_label_set_text_fmt(ui_SYNCTIME, "%02d:%02d:%02d", hour, minute, second);
        switch (week_temp)
        {
        case 0:
            lv_label_set_text(ui_Week, "周日");
            break;
        case 1:
            lv_label_set_text(ui_Week, "周一");
            break;
        case 2:
            lv_label_set_text(ui_Week, "周二");
            break;
        case 3:
            lv_label_set_text(ui_Week, "周三");
            break;
        case 4:
            lv_label_set_text(ui_Week, "周四");
            break;
        case 5:
            lv_label_set_text(ui_Week, "周五");
            break;
        case 6:
            lv_label_set_text(ui_Week, "周六");
            break;
        }
    }
}

void weather_init(String apiKey, String location, String ApiHost)
{
    weather.SetApi(apiKey);
    weather.SetLocation(location);
    weather.SetApiHost(ApiHost);
}

void weather_update()
{
    if(qWeather_Key != NULL && qWeather_ApiHost != NULL)
    {
        weather.updateWeather();

        int weatherCode = weather.getWeather();

        extern const lv_img_dsc_t* get_weather_icon(int code);
        const lv_img_dsc_t* weather_icon = get_weather_icon(weatherCode);
      
        lv_img_set_src(ui_WeatherICON, weather_icon);
        lv_label_set_text_fmt(ui_Temperature, "%03d℃", weather.getTemp());
        lv_label_set_text_fmt(ui_Humidity, "%03d%%", weather.getHumidity());
        lv_label_set_text(ui_WeatherCHN, weather.getWeatherText().c_str());
    }
}

const lv_img_dsc_t* get_weather_icon(int code)
{
    // 根据天气代码返回对应的图标指针
    switch(code) {
        case 100: return &ui_img_100_png;
        case 101: return &ui_img_101_png;
        case 102: return &ui_img_102_png;
        case 103: return &ui_img_103_png;
        case 104: return &ui_img_104_png;
        case 150: return &ui_img_150_png;
        case 151: return &ui_img_151_png;
        case 152: return &ui_img_152_png;
        case 153: return &ui_img_153_png;
        case 300: return &ui_img_300_png;
        case 301: return &ui_img_301_png;
        case 302: return &ui_img_302_png;
        case 303: return &ui_img_303_png;
        case 304: return &ui_img_304_png;
        case 305: return &ui_img_305_png;
        case 306: return &ui_img_306_png;
        case 307: return &ui_img_307_png;
        case 308: return &ui_img_308_png;
        case 309: return &ui_img_309_png;
        case 310: return &ui_img_310_png;
        case 311: return &ui_img_311_png;
        case 312: return &ui_img_312_png;
        case 313: return &ui_img_313_png;
        case 314: return &ui_img_314_png;
        case 315: return &ui_img_315_png;
        case 316: return &ui_img_316_png;
        case 317: return &ui_img_317_png;
        case 318: return &ui_img_318_png;
        case 350: return &ui_img_350_png;
        case 351: return &ui_img_351_png;
        case 399: return &ui_img_399_png;
        case 400: return &ui_img_400_png;
        case 401: return &ui_img_401_png;
        case 402: return &ui_img_402_png;
        case 403: return &ui_img_403_png;
        case 404: return &ui_img_404_png;
        case 405: return &ui_img_405_png;
        case 406: return &ui_img_406_png;
        case 407: return &ui_img_407_png;
        case 408: return &ui_img_408_png;
        case 409: return &ui_img_409_png;
        case 410: return &ui_img_410_png;
        case 456: return &ui_img_456_png;
        case 457: return &ui_img_457_png;
        case 499: return &ui_img_499_png;
        case 500: return &ui_img_500_png;
        case 501: return &ui_img_501_png;
        case 502: return &ui_img_502_png;
        case 503: return &ui_img_503_png;
        case 504: return &ui_img_504_png;
        case 507: return &ui_img_507_png;
        case 508: return &ui_img_508_png;
        case 509: return &ui_img_509_png;
        case 510: return &ui_img_510_png;
        case 511: return &ui_img_511_png;
        case 512: return &ui_img_512_png;
        case 513: return &ui_img_513_png;
        case 514: return &ui_img_514_png;
        case 515: return &ui_img_515_png;
        case 900: return &ui_img_900_png;
        case 901: return &ui_img_901_png;
        case 999: return &ui_img_999_png;
        default: return &ui_img_999_png; // 默认天气图标
    }
}

bool time_get_strap = false;
int sleep_EpochTime = 0;
void deep_sleep()
{
    if(time_get_strap == false)
    {
        time_get_strap = true;
        sleep_EpochTime = timeClient.getEpochTime() / 60;
    }
    if(timeClient.isTimeSet() == true)
    {   
        if(sleep_time == 0)
        {
            return;
        }
        else
        {
            if((timeClient.getEpochTime() / 60) - sleep_EpochTime >= sleep_time)
            {
                esp_wifi_stop();
                esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);
                esp_deep_sleep_start();
            }
        }
    }
}