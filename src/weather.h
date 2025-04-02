#ifndef _WEATHER_H
#define _WEATHER_H

#include "Arduino.h"
#include "ArduinoJson.h"
#include "ArduinoUZlib.h"
#include "HTTPClient.h"

//定义天气类
class Weather
{
    public:
        Weather();
        void SetApi(String apiKey);
        void SetLocation(String location);
        void SetApiHost(String apiHost);//设置API地址
        bool updateWeather();  //发送一次请求信息，获取天气数据

        int getTemp();
        int getWeather();
        String getWeatherText();
        int getHumidity();
    private:
        String apiKey;                  //天气API密钥
        String location;                //地点
        String apiHost;             //天气API地址
        JsonDocument doc;   //Json容器
        JsonObject now;     //可根据返回的json信息自行定义其他天气数据
};

#endif
