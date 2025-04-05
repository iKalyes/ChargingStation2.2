#include <flash.h>

Preferences flash;

void flash_init()
{
    get_free_space(); /* Get the free space of the flash */
    load_setting(); /* Load the setting from the flash */
}

void get_free_space()
{
    flash.begin("setting");
    free_space = flash.freeEntries();
    flash.end();
}

void save_setting()
{
    flash.begin("setting");
    flash.putUInt("brightness", brightness);
    flash.putUInt("sleep_time", sleep_time);
    flash.putBool("fan_switch", fan_switch);
    flash.putBool("USBC_Switch", USBC_Switch);
    flash.putBool("USBA_Switch", USBA_Switch);
    flash.end();
}

void load_setting()
{
    flash.begin("setting");
    brightness = flash.getUInt("brightness", 128);
    sleep_time = flash.getUInt("sleep_time", 0);
    fan_switch = flash.getBool("fan_switch", false);
    USBC_Switch = flash.getBool("USBC_Switch", false);
    USBA_Switch = flash.getBool("USBA_Switch", false);
    voltage0_adc = flash.getFloat("voltage0_adc", 10.85f);
    voltage1_adc = flash.getFloat("voltage1_adc", 10.85f);
    voltage2_adc = flash.getFloat("voltage2_adc", 10.85f);
    voltage3_adc = flash.getFloat("voltage3_adc", 10.85f);
    flash.end();
}

void save_USBA_setting()
{
    flash.begin("setting");
    flash.putBool("USBA_Switch", USBA_Switch);
    flash.end();
}

void save_USBC_setting()
{
    flash.begin("setting");
    flash.putBool("USBC_Switch", USBC_Switch);
    flash.end();
}

void save_adc_setting()
{
    flash.begin("setting");
    flash.putFloat("voltage0_adc", voltage0_adc);
    flash.putFloat("voltage1_adc", voltage1_adc);
    flash.putFloat("voltage2_adc", voltage2_adc);
    flash.putFloat("voltage3_adc", voltage3_adc);
    flash.end();
}

void save_web_config()
{
    flash.begin("setting");
    flash.putString("CityCode", CityCode);
    flash.putString("qApiHost", qWeather_ApiHost);
    flash.putString("qWeather_Key", qWeather_Key);
    flash.putUInt("TimeZone", TimeZone);
    flash.putString("NTPServer", NTPServer);
    flash.putUInt("SyncTime", SyncTime);
    flash.end();
}

void load_web_config()
{
    flash.begin("setting");
    uint32_t CityCode_Temp = (flash.getString("CityCode", "101280101")).toInt();
    if(CityCode_Temp>=101000000 && CityCode_Temp<=102000000)
    {
        CityCode = (String)(CityCode_Temp);
    }
    else
    {
        CityCode = "101280101";
    }
    qWeather_ApiHost = flash.getString("qApiHost", "");
    qWeather_Key = flash.getString("qWeather_Key", "");
    TimeZone = flash.getUInt("TimeZone", 8);
    NTPServer = flash.getString("NTPServer", "pool.ntp.org");
    SyncTime = flash.getUInt("SyncTime", 1);
    flash.end();
}

void save_wifi_config()
{
    flash.begin("setting");
    flash.putString("sta_ssid", wifisetting.sta_ssid);
    flash.putString("sta_pwd", wifisetting.sta_pwd);
    flash.end();
}

void load_wifi_config()
{
    flash.begin("setting");
    strcpy(wifisetting.sta_ssid, flash.getString("sta_ssid", "").c_str());
    strcpy(wifisetting.sta_pwd, flash.getString("sta_pwd", "").c_str());
    flash.end();
}

void delete_wifi_config()
{
    flash.begin("setting");
    flash.remove("sta_ssid");
    flash.remove("sta_pwd");
    flash.end();
}