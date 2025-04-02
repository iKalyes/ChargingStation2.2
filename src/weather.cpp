#include "weather.h"

Weather::Weather()
{

}

void Weather::SetApi(String apiKey)
{
    this->apiKey = apiKey;
}

void Weather::SetLocation(String location)
{
    this->location = location;
}

void Weather::SetApiHost(String apiHost)
{
    this->apiHost = apiHost;
}

bool Weather::updateWeather()
{
    HTTPClient http;   //用于访问网络
    WiFiClient *stream;
    int size;
    
    http.begin("https://"+ this->apiHost + "/v7/weather/now?location="+ this->location + "&key=" + this->apiKey); //获取天气信息
    int httpcode = http.GET();   //发送GET请求
    if(httpcode > 0)
    {
        if(httpcode == HTTP_CODE_OK)
        {
            stream = http.getStreamPtr();   //读取服务器返回的数据
            size = http.getSize();
        }
    }
    else
    {
        return false;
    }
    http.end();   //结束当前连接
    
    uint8_t inbuff[size];
    stream->readBytes(inbuff, size);
    uint8_t *outbuf = NULL;
    uint32_t out_size = 0;
    int result = ArduinoUZlib::decompress(inbuff,size, outbuf, out_size);
    deserializeJson(doc, outbuf);
    now = doc["now"].as<JsonObject>();
    return true;
}

int Weather::getTemp()
{
    return now["temp"].as<int>();
}

int Weather::getWeather()
{
    return now["icon"].as<int>();
}

String Weather::getWeatherText()
{
    return now["text"].as<String>();
}

int Weather::getHumidity()
{
    return now["humidity"].as<int>();
}