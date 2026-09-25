#include <SMS_STS.h>
#include <driver/twai.h>

#define NORMAL_RECEIVE_ID 0x410

#define TWAI_TX_PIN D0
#define TWAI_RX_PIN D1

#define ARM_TX_PIN TX
#define ARM_RX_PIN RX

#define SLAVE1_WHEEL_CONTROL_ID 0x310     // タイヤ
#define SLAVE2_DISHES_ARM_ID 0x710        // お皿
#define SLAVE3_ZEUS_ARM_STS3215_ID 0x410  // 万能手腕
#define SLAVE4_SQUID_ARM_ID 0x110         // いかさん
#define SLAVE5_MARKER_ARM_ID 0x210        // マーカー
#define SLAVE6_ZEUS_ARM_SHOUKOU_ID 0x150  // 万能手腕昇降
#define SLAVEX_BUTSUDAN_LED_ID 0x115      // 仏壇

#define SERVO_FIRST 2048

#define ARM_ID_1_TSUKAMU 1700
#define ARM_ID_2_TSUKAMU 2380
#define ARM_ID_3_TSUKAMU 2680

#define ARM_ID_1_TSUKAMU_BASEBALL 1903
#define ARM_ID_2_TSUKAMU_BASEBALL 2254
#define ARM_ID_3_TSUKAMU_BASEBALL 2433

#define ARM_ID_1_TSUKAMU_MUTSUGORO 1645
#define ARM_ID_2_TSUKAMU_MUTSUGORO 2390
#define ARM_ID_3_TSUKAMU_MUTSUGORO 2660

#define ARM_ID_1_CHOITSUKAMU 1980
#define ARM_ID_2_CHOITSUKAMU 2047
#define ARM_ID_3_CHOITSUKAMU 2290

#define ARM_ID_1_HANASU 2300
#define ARM_ID_2_HANASU 1700
#define ARM_ID_3_HANASU 2030

#define LEG_ID_1_OUT 4065
#define LEG_ID_1_OUT_CENTER 2600
#define LEG_ID_1_CENTER 2116
#define LEG_ID_1_IN 300
#define LEG_ID_1_MUTSUGORO 1603

#define LEG_ID_2_OUT 30
#define LEG_ID_2_OUT_CENTER 1400
#define LEG_ID_2_CENTER 1921
#define LEG_ID_2_IN 3700
#define LEG_ID_2_MUTSUGORO 2310

#define NEMOTO_YOKO 100
#define NEMOTO_SHITA 1050
#define NEMOTO_KINKYU 850
#define NEMOTO_MUTSUGORO 884

#define BASEBALL_BALL 484
#define TENNIS_BALL 744

SMS_STS st;

uint16_t ID = 0;
int8_t data[8] = { 0 };

twai_message_t receiveframe;

void send(uint16_t ID /*ID*/, int8_t data1 /*識別子*/, int8_t data2 /*データ*/, int8_t data3, int8_t data4, int8_t data5, int8_t data6, int8_t data7, int8_t data8) {

  twai_message_t SendFrame;

  SendFrame.identifier = ID;
  SendFrame.rtr = 0;
  SendFrame.extd = 0;
  SendFrame.data_length_code = 8;
  SendFrame.data[0] = data1;
  SendFrame.data[1] = data2;
  SendFrame.data[2] = data3;
  SendFrame.data[3] = data4;
  SendFrame.data[4] = data5;
  SendFrame.data[5] = data6;
  SendFrame.data[6] = data7;
  SendFrame.data[7] = data8;

  if (twai_transmit(&SendFrame, pdMS_TO_TICKS(10)) == ESP_OK) {
    Serial.printf("ID = %x, data1 = %x, data2 = %d, data3 = %d, data4 = %d, data5 = %d, data6 = %d, data7 = %d, data8 = %d\n", ID, data1, data2, data3, data4, data5, data6, data7, data8);
    Serial.println("送信成功");
  } else {
    Serial.printf("ID = %x, data1 = %x, data2 = %d, data3 = %d, data4 = %d, data5 = %d, data6 = %d, data7 = %d, data8 = %d\n", ID, data1, data2, data3, data4, data5, data6, data7, data8);
    Serial.println("送信失敗");
  }
  return;
}

void motor_control() {

  switch (data[0]) {

    case 3:

      switch (data[1]) {

        case 2:
          st.RegWritePosEx(6, data[2] == 0 ? TENNIS_BALL : BASEBALL_BALL, 1500);
          st.RegWritePosEx(4, LEG_ID_1_OUT, 1500);
          st.RegWritePosEx(5, LEG_ID_2_OUT, 1500);
          st.RegWriteAction();
          vTaskDelay(pdMS_TO_TICKS(1000));
          st.RegWritePosEx(1, ARM_ID_1_HANASU, 1500);
          st.RegWritePosEx(2, ARM_ID_2_HANASU, 1500);
          st.RegWritePosEx(3, ARM_ID_3_HANASU, 1500);
          st.RegWriteAction();
          send(SLAVE6_ZEUS_ARM_SHOUKOU_ID, 4, 1, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA);  //それっぽいとこ
          break;

        case 3:
          send(SLAVE6_ZEUS_ARM_SHOUKOU_ID, 2, data[2] == 0 ? 1 : 2, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA);  //高さを下げる
          vTaskDelay(pdMS_TO_TICKS(1000));
          st.RegWritePosEx(1, data[2] == 0 ? ARM_ID_1_TSUKAMU : ARM_ID_1_TSUKAMU_BASEBALL, 1500);
          st.RegWritePosEx(2, data[2] == 0 ? ARM_ID_2_TSUKAMU : ARM_ID_2_TSUKAMU_BASEBALL, 1500);
          st.RegWritePosEx(3, data[2] == 0 ? ARM_ID_3_TSUKAMU : ARM_ID_3_TSUKAMU_BASEBALL, 1500);
          st.RegWriteAction();
          vTaskDelay(pdMS_TO_TICKS(500));
          send(SLAVE6_ZEUS_ARM_SHOUKOU_ID, 4, 1, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA);  //それっぽいとこ
          vTaskDelay(pdMS_TO_TICKS(500));
          st.RegWritePosEx(6, NEMOTO_YOKO, 1500);
          st.RegWritePosEx(4, LEG_ID_1_OUT_CENTER, 1500);
          st.RegWritePosEx(5, LEG_ID_2_OUT_CENTER, 1500);
          st.RegWriteAction();
          break;

        case 4:  //設置位置に上げる
          send(SLAVE6_ZEUS_ARM_SHOUKOU_ID, 3, data[2] == 0 ? 1 : 2, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA);
          break;

        case 5:
          st.RegWritePosEx(1, ARM_ID_1_HANASU, 1500);
          st.RegWritePosEx(2, ARM_ID_2_HANASU, 1500);
          st.RegWritePosEx(3, ARM_ID_3_HANASU, 1500);
          st.RegWriteAction();
          break;

        default:
          break;
      }
      break;

    case 4:
      switch (data[1]) {
        case 1:
          st.WritePosEx(6, (st.ReadPos(6) + 50), 3400);
          Serial.printf("ID6 : %d\n", st.ReadPos(6));
          vTaskDelay(pdMS_TO_TICKS(20));
          break;

        case 2:
          st.WritePosEx(6, (st.ReadPos(6) - 50), 3400);
          Serial.printf("ID6 : %d\n", st.ReadPos(6));
          vTaskDelay(pdMS_TO_TICKS(20));
          break;

        default:
          break;
      }
      break;

    case 5:
      st.WritePosEx(6, BASEBALL_BALL, 1500);
      vTaskDelay(pdMS_TO_TICKS(1));
      break;

    case 6:
      st.WritePosEx(6, TENNIS_BALL, 1500);
      vTaskDelay(pdMS_TO_TICKS(1));
      break;

    case 7:
      st.RegWritePosEx(4, (st.ReadPos(4) + 50), 1500);
      st.RegWritePosEx(5, (st.ReadPos(5) - 50), 1500);
      st.RegWriteAction();
      Serial.printf("ID4 : %d, ID5 : %d\n", st.ReadPos(4), st.ReadPos(5));
      break;

    case 8:
      st.RegWritePosEx(4, (st.ReadPos(4) - 50), 1500);
      st.RegWritePosEx(5, (st.ReadPos(5) + 50), 1500);
      st.RegWriteAction();
      Serial.printf("ID4 : %d, ID5 : %d\n", st.ReadPos(4), st.ReadPos(5));
      break;

    case 10:
      st.RegWritePosEx(1, ARM_ID_1_CHOITSUKAMU, 1500);
      st.RegWritePosEx(2, ARM_ID_2_CHOITSUKAMU, 1500);
      st.RegWritePosEx(3, ARM_ID_3_CHOITSUKAMU, 1500);
      st.RegWritePosEx(4, LEG_ID_1_MUTSUGORO, 1500);
      st.RegWritePosEx(5, LEG_ID_2_MUTSUGORO, 1500);
      st.RegWritePosEx(6, NEMOTO_MUTSUGORO, 1500);
      st.RegWriteAction();
      break;

    case 11:
      st.RegWritePosEx(1, ARM_ID_1_TSUKAMU_MUTSUGORO, 1500);
      st.RegWritePosEx(2, ARM_ID_2_TSUKAMU_MUTSUGORO, 1500);
      st.RegWritePosEx(3, ARM_ID_3_TSUKAMU_MUTSUGORO, 1500);
      st.RegWriteAction();
      break;

    case 12:
      st.WritePosEx(6, NEMOTO_YOKO, 1500);
      break;

    case 13:
      st.RegWritePosEx(1, ARM_ID_1_CHOITSUKAMU, 1500);
      st.RegWritePosEx(2, ARM_ID_2_CHOITSUKAMU, 1500);
      st.RegWritePosEx(3, ARM_ID_3_CHOITSUKAMU, 1500);
      st.RegWriteAction();
      break;

    default:
      break;
  }
  vTaskDelay(pdMS_TO_TICKS(1));
}

void setup() {

  Serial.begin(115200);

  vTaskDelay(pdMS_TO_TICKS(200));

  /***********************************CAN関連********************************************/
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TWAI_TX_PIN, (gpio_num_t)TWAI_RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  esp_err_t ret = twai_driver_install(&g_config, &t_config, &f_config);
  if (ret == ESP_OK) Serial.println("インストール完了");
  else Serial.println("インストール失敗");
  ret = twai_start();
  if (ret == ESP_OK) Serial.println("CANスタート完了");
  else Serial.println("CANスタート失敗");
  /**************************************************************************************/

  /*STS3215_setup*/
  Serial1.begin(1000000, SERIAL_8N1, ARM_RX_PIN, ARM_TX_PIN);
  st.pSerial = &Serial1;

  vTaskDelay(pdMS_TO_TICKS(3000));

  /*根本*/
  st.RegWritePosEx(6, NEMOTO_KINKYU, 1500);

  /*ハンド*/
  st.RegWritePosEx(1, ARM_ID_1_TSUKAMU, 1500);
  st.RegWritePosEx(2, ARM_ID_2_TSUKAMU, 1500);
  st.RegWritePosEx(3, ARM_ID_3_TSUKAMU, 1500);
  st.RegWriteAction();

  vTaskDelay(pdMS_TO_TICKS(1000));

  /*ラック*/
  st.RegWritePosEx(4, LEG_ID_1_IN, 1500);
  st.RegWritePosEx(5, LEG_ID_2_IN, 1500);
  st.RegWriteAction();

  vTaskDelay(pdMS_TO_TICKS(1000));

  st.WritePosEx(6, NEMOTO_SHITA, 1500);
}

void loop() {

  if (twai_receive(&receiveframe, pdMS_TO_TICKS(10)) == ESP_OK) {
    if (receiveframe.identifier == NORMAL_RECEIVE_ID) {

      ID = receiveframe.identifier;
      data[0] = receiveframe.data[0];  //識別子
      data[1] = receiveframe.data[1];  //数値
      data[2] = receiveframe.data[2];  //ゴミ
      data[3] = receiveframe.data[3];  //ゴミ
      data[4] = receiveframe.data[4];  //ゴミ
      data[5] = receiveframe.data[5];  //ゴミ
      data[6] = receiveframe.data[6];  //ゴミ
      data[7] = receiveframe.data[7];  //ゴミ
      Serial.printf("ID = %x, data1 = %d, data2 = %d, data3 = %d, data4 = %d, data5 = %d, data6 = %d, data7 = %d, data8 = %d\n", ID, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

      motor_control();
    }
  }

  vTaskDelay(pdMS_TO_TICKS(1));
}
