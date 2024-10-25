#include "esphome/core/log.h"
#include "debug_mqtt.h"

#if defined(USE_ESP8266)
#include <AsyncMqttClient.h>
AsyncMqttClient *mqtt_client{nullptr};
#elif defined(USE_ESP32)
#include <mqtt_client.h>
esp_mqtt_client_handle_t mqtt_client{nullptr};
#endif

namespace esphome
{
    namespace samsung_ac
    {
        bool debug_mqtt_connected()
        {
            if (mqtt_client == nullptr)
                return false;

#if defined(USE_ESP8266)
            return mqtt_client->connected();
#elif defined(USE_ESP32)
            return true;
#endif
        }

        void debug_mqtt_connect(const std::string &host, const uint16_t port, const std::string &username, const std::string &password)
        {
            if (host.empty())
                return;

#if defined(USE_ESP8266)
            if (mqtt_client == nullptr)
            {
                mqtt_client = new AsyncMqttClient();
                mqtt_client->setServer(host.c_str(), port);
                if (!username.empty())
                    mqtt_client->setCredentials(username.c_str(), password.c_str());
            }

            if (!mqtt_client->connected())
                mqtt_client->connect();

#elif defined(USE_ESP32)
            if (mqtt_client == nullptr)
            {
                esp_mqtt_client_config_t mqtt_cfg = {};
                mqtt_cfg.host = host.c_str();
                mqtt_cfg.port = port;
                if (!username.empty())
                {
                    mqtt_cfg.username = username.c_str();
                    mqtt_cfg.password = password.c_str();
                }
                mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
                esp_mqtt_client_start(mqtt_client);
            }
#endif
        }

        bool debug_mqtt_publish(const std::string &topic, const std::string &payload)
        {
            if (mqtt_client == nullptr)
                return false;

#if defined(USE_ESP8266)
            return mqtt_client->publish(topic.c_str(), 0, false, payload.c_str()) != 0;
#elif defined(USE_ESP32)
            return esp_mqtt_client_publish(mqtt_client, topic.c_str(), payload.c_str(), payload.length(), 0, false) != -1;
#endif
        }
    } // namespace samsung_ac
} // namespace esphome
