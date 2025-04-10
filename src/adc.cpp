#include <adc.h>

// 定义缓冲区大小
#define ADC_FILTER_SIZE 2

// 为每个通道创建缓冲区
static float voltage0_buffer[ADC_FILTER_SIZE] = {0};
static float voltage1_buffer[ADC_FILTER_SIZE] = {0};
static float voltage2_buffer[ADC_FILTER_SIZE] = {0};
static float voltage3_buffer[ADC_FILTER_SIZE] = {0};
static uint8_t buffer_index = 0;

// 更新缓冲区并计算平均值
float updateBuffer(float newValue, float* buffer) {
    buffer[buffer_index] = newValue;
    float sum = 0;
    for(uint8_t i = 0; i < ADC_FILTER_SIZE; i++) {
        sum += buffer[i];
    }
    return sum / ADC_FILTER_SIZE;
}

void adc_init()
{
    analogReadResolution(12);
    adc_timer = lv_timer_create(adc_task, 100, NULL);
}

// 读取和平滑处理
void readVoltages() {
    // 读取原始值
    float raw0 = voltage0_adc * (float)(analogReadMilliVolts(ADC0_PIN)) * 1e-3;
    float raw1 = voltage1_adc * (float)(analogReadMilliVolts(ADC1_PIN)) * 1e-3;
    float raw2 = voltage2_adc * (float)(analogReadMilliVolts(ADC2_PIN)) * 1e-3;
    float raw3 = voltage3_adc * (float)(analogReadMilliVolts(ADC3_PIN)) * 1e-3;

    if(raw0 < 2.0) {
        raw0 = 0;
    }
    if(raw1 < 2.0) {
        raw1 = 0;
    }
    if(raw2 < 2.0) {
        raw2 = 0;
    }
    if(raw3 < 2.0) {
        raw3 = 0;
    }
    
    // 平滑处理
    voltage0 = updateBuffer(raw0, voltage0_buffer);
    voltage1 = updateBuffer(raw1, voltage1_buffer);
    voltage2 = updateBuffer(raw2, voltage2_buffer);
    voltage3 = updateBuffer(raw3, voltage3_buffer);
    
    // 更新缓冲区索引
    buffer_index = (buffer_index + 1) % ADC_FILTER_SIZE;
}

void adc_task(lv_timer_t *timer)
{
    readVoltages();
    
    int voltage0_full = round(voltage0 * 10);
    int voltage0_int = voltage0_full / 10;
    int voltage0_frac = voltage0_full % 10;
    
    int voltage1_full = round(voltage1 * 10);
    int voltage1_int = voltage1_full / 10;
    int voltage1_frac = voltage1_full % 10;
    
    int voltage2_full = round(voltage2 * 10);
    int voltage2_int = voltage2_full / 10;
    int voltage2_frac = voltage2_full % 10;
    
    int voltage3_full = round(voltage3 * 10);
    int voltage3_int = voltage3_full / 10;
    int voltage3_frac = voltage3_full % 10;
    
    lv_label_set_text_fmt(ui_VoltageUSBA, "%02d.%01dV", voltage0_int, voltage0_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC3, "%02d.%01dV", voltage1_int, voltage1_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC2, "%02d.%01dV", voltage2_int, voltage2_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC1, "%02d.%01dV", voltage3_int, voltage3_frac);

    lv_label_set_text_fmt(ui_VoltageUSBAADC, "%02d.%01dV", voltage0_int, voltage0_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC3ADC, "%02d.%01dV", voltage1_int, voltage1_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC2ADC, "%02d.%01dV", voltage2_int, voltage2_frac);
    lv_label_set_text_fmt(ui_VoltageUSBC1ADC, "%02d.%01dV", voltage3_int, voltage3_frac);
}
