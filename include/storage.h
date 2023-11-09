#pragma once

#include <nvs.h>
#include <string>

using namespace std;

namespace storage{
    static string deviceId = "";
    static nvs_handle_t nvsHandle;

    esp_err_t begin(){
        esp_err_t res = nvs_open("checker", NVS_READWRITE, &nvsHandle);
        if(res != ESP_OK){
            //log_e("Unable to open NVS namespace: %d", res);
        }
        return res;
    }

    uint8_t getUint8(const char* key, uint8_t def = 255){
        uint8_t data = 0;
        if(nvs_get_u8(nvsHandle, key, &data) != ESP_OK){
            return def;
        }
        return data;
    }

    string getString(string key, size_t length){
        char data[length] = {0};
        esp_err_t err = nvs_get_str(nvsHandle, key.c_str(), data, &length);
        if(err != ESP_OK){
            //log_e("Failed to load data '%s'. length: %d, error code: %s", key.c_str(), lenData, esp_err_to_name(err));
            return "";
        }
        return string(data);
    }

    string getString(string key){
        uint16_t lenData;
        esp_err_t err = nvs_get_u16(nvsHandle, (key + "$l").c_str(), &lenData);
        if(err != ESP_OK){
            //log_e("Failed to load data '%s$l'. length: %d, error code: %s", key.c_str(), lenData, esp_err_to_name(err));
            return "";
        }
        return getString(key, lenData);
    }

    bool setString(string key, string value, bool saveLength = true){
        if(saveLength && nvs_set_u16(nvsHandle, (key + "$l").c_str(), value.length() + 1) != ESP_OK){
            return false;
        }
        return nvs_set_str(nvsHandle, key.c_str(), value.c_str()) == ESP_OK;
    }

    string getDeviceId(){
        if(deviceId.length() > 0){
            return deviceId;
        }

        string id = getString("DEVICE_ID", 11);
        if(id.length() == 10){
            return deviceId = id;
        }else{
            stringstream stream;
            for(uint8_t i = 0; i < 5; ++i){
                stream << (char) random('a', 'z');
            }
            stream << '_';
            for(uint8_t i = 0; i < 4; ++i){
                stream << (char) random('0', '9');
            }
            setString("DEVICE_ID", deviceId = stream.str(), false);
        }
        return deviceId;
    }
}