#include <wifi_server.h>

WiFiManager wm;

//WEB配网函数
void Webconfig()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  
  delay(1000);
  wm.resetSettings(); // wipe settings
  
  WiFiManagerParameter  custom_citycode("CityCode","CityCode","101280101",9);
  WiFiManagerParameter  custom_qweatherapihost("qWeatherApiHost","QWeather API Host","",32);
  WiFiManagerParameter  custom_qweatherkey("qWeatherKey","QWeather User Key","",32);
  WiFiManagerParameter  custom_ntpserver("NTPServer","NTP Server","pool.ntp.org",32);
  WiFiManagerParameter  custom_timezone("TimeZone","Time Zone","8",2);
  WiFiManagerParameter  custom_synctime("SyncTime","NTP And Weather Update Time(Hour)","1",2);
  WiFiManagerParameter  p_lineBreak_notext("<p></p>");

  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_citycode);
  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_qweatherapihost);
  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_qweatherkey);
  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_ntpserver);
  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_timezone);
  wm.addParameter(&p_lineBreak_notext);
  wm.addParameter(&custom_synctime);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi","restart"};
  wm.setMenu(menu);
  wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%

  bool res;
   res = wm.autoConnect("CharingStation"); // anonymous ap

  while(!res);
}

String getParam(String name){
    //read parameter from server, for customhmtl input
    String value;
    if(wm.server->hasArg(name)) {
      value = wm.server->arg(name);
    }
    return value;
}

void saveParamCallback(){
    //将从页面中获取的数据保存
    CityCode =  getParam("CityCode");
    qWeather_ApiHost = getParam("qWeatherApiHost");
    qWeather_Key = getParam("qWeatherKey");
    TimeZone = getParam("TimeZone").toInt();
    NTPServer = getParam("NTPServer");
    SyncTime = getParam("SyncTime").toInt();

    save_web_config();
    time_server_setting(NTPServer.c_str(), TimeZone, SyncTime);
    weather_init(qWeather_Key, CityCode, qWeather_ApiHost);

}


bool time_get_strap_unconnect = false;
int sleep_millis = 0;
void wificonfig()
{
  if(wificonfig_flag == true)
  {
    while(WiFi.status() != WL_CONNECTED)
    {
        lv_label_set_text(ui_WIFIStatus, "请连接至热点");
        lv_label_set_text(ui_SSID, "CharingStation");
        lv_label_set_text(ui_IPADDR, "192.168.4.1");
        int count = 0;
        int count_end = 10000;
        while(count < count_end)
        {
          count++;
          lv_task_handler();
        }
          Webconfig();
    }

    if(WiFi.status() == WL_CONNECTED)
    {
        strcpy(wifisetting.sta_ssid, WiFi.SSID().c_str());
        strcpy(wifisetting.sta_pwd, WiFi.psk().c_str());
        save_wifi_config();

        time_server_forceupdate();
        weather_update();

        lv_label_set_text(ui_WIFIStatus, "已连接");
        lv_label_set_text(ui_TextWIFIStart, "更新天气时间");
        lv_label_set_text(ui_SSID, WiFi.SSID().c_str());
        lv_label_set_text(ui_IPADDR, WiFi.localIP().toString().c_str());

        lv_img_set_src(ui_ImageWiFi, &ui_img_593743026);

        wificonfig_flag = false;

        _ui_screen_change( &ui_MainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_MainScreen_screen_init);
        lvgl_group_to_main();
    }
  }
  else
  {
    if(time_get_strap_unconnect == false)
    {
      time_get_strap_unconnect = true;
      sleep_millis = millis();
    }

    if(sleep_time == 0)
    {
      return;
    }
    else
    {
      if(millis() - sleep_millis >= sleep_time * 60000)
      {
        esp_wifi_stop();
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);
        esp_deep_sleep_start();
      }
    }
  }
}

void wifireset()
{
  wm.resetSettings();
  delete_wifi_config();
  ESP.restart();
}

void wificonnect()
{
  load_wifi_config();
  WiFi.begin(wifisetting.sta_ssid, wifisetting.sta_pwd);
  WiFi.setAutoReconnect(true);
  while (WiFi.status() != WL_CONNECTED)
  {
    lv_task_handler();
    wificonfig();
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    lv_label_set_text(ui_WIFIStatus, "已连接");
    lv_label_set_text(ui_TextWIFIStart, "更新天气时间");
    lv_label_set_text(ui_SSID, WiFi.SSID().c_str());
    lv_label_set_text(ui_IPADDR, WiFi.localIP().toString().c_str());

    lv_img_set_src(ui_ImageWiFi, &ui_img_593743026);
  }
}
