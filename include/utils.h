#pragma once

#include <nvs.h>
#include <time.h>
#include <string>
#include <sstream>
#include <esp_sleep.h>
#include <esp_timer.h>
#include <esp_random.h>
#include <driver/rtc_io.h>

using namespace std;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

inline uint32_t random(uint32_t max){
    if(max == 0){
        return 0;
    }
    uint32_t val = esp_random();
    return val % max;
}

inline uint32_t random(uint32_t min, uint32_t max){
    if(min >= max){
        return min;
    }
    return random(max - min) + min;
}

inline uint8_t getBatteryLevel(){
    // TODO: check battery level
    //uint8_t calculate = 0;
    //return MIN(100, MAX(0, calculate));
    return random(1, 10) * 10;
}

inline int64_t millis(){
    return esp_timer_get_time() / 1000LL;
}

inline int64_t getCurrentMillis(){
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000LL + now.tv_usec / 1000LL;
}

inline int64_t getCurrentMicros(){
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000LL + now.tv_usec;
}

inline void strReplace(string& origin, string replace, string str){
    auto index = origin.find(replace);
    if(index == string::npos){
        return;
    }
    origin.replace(index, replace.length(), str);
}

inline void lightSleep(gpio_num_t pin, int level, uint64_t time = 0){
    rtc_gpio_pullup_en(pin);
    esp_sleep_enable_ext0_wakeup(pin, level);
    if(time > 0){
        esp_sleep_enable_timer_wakeup(time);
    }
    esp_light_sleep_start();
}

inline void deepSleep(gpio_num_t pin, int level, uint64_t time = 0){
    rtc_gpio_pullup_en(pin);
    esp_sleep_enable_ext0_wakeup(pin, level);
    if(time > 0){
        esp_sleep_enable_timer_wakeup(time);
    }
    esp_deep_sleep_start();
}

void hibernate(){
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);

    #if SOC_PM_SUPPORT_RTC_PERIPH_PD
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    #endif

    #if SOC_PM_SUPPORT_RTC_SLOW_MEM_PD
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    #endif

    #if SOC_PM_SUPPORT_RTC_FAST_MEM_PD
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    #endif
    esp_deep_sleep_start();
}