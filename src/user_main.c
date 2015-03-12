/*
Copyright (c) 2015 Fredrik Olofsson <frol@hupp.se>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#define WIFI_SSID "SSID"
#define WIFI_PASS "PASSWORD"
#define MQTT_HOST "10.0.0.1"

#define DEEPSLEEP_DATA_NUM 20
#define DEEPSLEEP_RTC_CNT_ADDR 120
#define DEEPSLEEP_TIME 30000000

#include <ets_sys.h>
#include <driver/uart.h>
#include <osapi.h>
#include <mqtt.h>
#include <gpio.h>
#include <user_interface.h>
#include <mem.h>


static uint32_t vdd33 = 0;
static os_timer_t wifi_timer;
static MQTT_Client mqttClient;


static void
mqttPublishedCb(uint32_t *args)
{
  MQTT_Client *client = (MQTT_Client*)args;

  if (QUEUE_IsEmpty(&client->msgQueue))
     system_deep_sleep(DEEPSLEEP_TIME);
}


static void
wifi_timer_func(void *arg)
{
  uint8_t wifi_status = wifi_station_get_connect_status();
#if 0
  switch (wifi_status) {
  case STATION_IDLE:
    os_printf("STATION_IDLE\n");
    break;
  case STATION_CONNECTING:
    os_printf("STATION_CONNECTING\n");
    break;
  case STATION_WRONG_PASSWORD:
    os_printf("STATION_WRONG_PASSWORD\n");
    break;
  case STATION_NO_AP_FOUND:
    os_printf("STATION_NO_AP_FOUND\n");
    break;
  case STATION_CONNECT_FAIL:
    os_printf("STATION_CONNECT_FAIL\n");
    break;
  case STATION_GOT_IP:
    os_printf("STATION_GOT_IP\n");
    break;
  default:
    os_printf("unknown status\n");
    break;
  }
#endif

  /* Wait for IP then connect to MQTT server. Will sleep again when
     MQTT message queue is empty. */
  if (wifi_status == STATION_GOT_IP) {
    os_timer_disarm(&wifi_timer);
    MQTT_Connect(&mqttClient);
  }
}


/* Send all sensor data to MQTT server. */
static void
normal_cb(void)
{
  MQTT_InitConnection(&mqttClient, MQTT_HOST, 1883, 0);
  uint8 sta_mac[6];
  char id[32];
  wifi_get_macaddr(0 , sta_mac);
  os_sprintf(id, "esp-" MACSTR, MAC2STR(sta_mac));
  os_printf("%s\n", id);
  MQTT_InitClient(&mqttClient, id, "", "", 120, 1);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  {
    char topic[32];
    os_sprintf(topic, "%s/vdd33", id);
    char buffer[16];
    os_sprintf(buffer, "%d", vdd33);
    MQTT_Publish(&mqttClient, topic, buffer, os_strlen(buffer), 0, 0);
  }

  os_timer_disarm(&wifi_timer);
  os_timer_setfn(&wifi_timer, (os_timer_func_t *)wifi_timer_func, NULL);
  os_timer_arm(&wifi_timer, 100, 1);
}


/* Log sensors to rtc memory or whatever. */
static void
sleep_cb(void)
{
  os_printf("sleep_cb()\n");
  system_deep_sleep(DEEPSLEEP_TIME);
}


void
user_init(void)
{
  uart_init(BIT_RATE_115200, BIT_RATE_115200);

  uint32 rtc_cnt;
  system_rtc_mem_read(DEEPSLEEP_RTC_CNT_ADDR, &rtc_cnt, 4);

  os_printf("\n\nuser_init()\nSDK version:%s, chip_id:%x\n",
	    system_get_sdk_version(),
	    system_get_chip_id());

  /* First time after new flash. Configure STA mode. */
  if (SOFTAP_MODE == wifi_get_opmode()) {
    os_printf("First boot, chaning to STATION_MODE\n");
    wifi_set_opmode(STATION_MODE);
    wifi_station_set_auto_connect(TRUE);

    struct station_config sta_config = {
      .ssid = WIFI_SSID,
      .password = WIFI_PASS,
    };
    wifi_station_set_config(&sta_config);
  }

  if (rtc_cnt == 0) {
    // Normal mode.
    rtc_cnt++;
    system_deep_sleep_set_option(4);
    system_init_done_cb(normal_cb);
    /* Call readvdd33() before starting WIFI. */
    vdd33 = readvdd33();
  } else if (rtc_cnt >= DEEPSLEEP_DATA_NUM) {
    // Read sensors and go to sleep, next wakeup is normal.
    rtc_cnt = 0;
    system_deep_sleep_set_option(1); // Normal mode for next wake-up.
    system_init_done_cb(sleep_cb);
  } else {
    // Only read sensors and go to sleep.
    rtc_cnt++;
    system_deep_sleep_set_option(4);
    system_init_done_cb(sleep_cb);
  }

  system_rtc_mem_write(DEEPSLEEP_RTC_CNT_ADDR, &rtc_cnt, 4);
}
