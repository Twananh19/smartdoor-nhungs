// #include "wifihandler.h"
// #include "secrets.h"
// #include "firebasehandler.h"
// #include "rfidhandler.h"
// #include "realtime.h"
// #include "lcdhandler.h"
// #include "keypadhandler.h"
// #include "servo.h"
// #include <ArduinoJson.h>
// #include "uart.h"
// #include "irhandler.h" 

// // std::map<String, int> scanCount;
// // #include "servo.h"
// struct CardData
// {
//   String uuid;
//   String password;
//   int money;
//   String time;
//   int scanCount; // Thêm trường này để lưu số lần quét
// };

// CardData parseCardData(const String &jsonString)
// {
//   CardData data;

//   JsonDocument doc;
//   DeserializationError error = deserializeJson(doc, jsonString);

//   if (!error)
//   {
//     data.uuid = doc["uuid"] | "";
//     data.password = doc["password"] | "";
//     data.money = doc["money"] | 0;
//     data.time = doc["time"] | " ";
//     data.scanCount = doc["scanCount"] | 0; // Thêm dòng này

//   }
//   else
//   {
//     Serial.print("Failed to parse JSON: ");
//     Serial.println(error.f_str());
//   }

//   return data;
// }

// String lastUID = "";
// unsigned long lastReadTime = 0;
// const unsigned long debounceTime = 3000;

// CardData checkPassword(const String &password) {
//   String allData = fb.getJson("data");
//   CardData found;
//   if (allData.length() > 0) {
//     JsonDocument doc;
//     DeserializationError error = deserializeJson(doc, allData);
//     if (!error) {
//       for (JsonPair kv : doc.as<JsonObject>()) {
//         JsonObject user = kv.value().as<JsonObject>();
//         String pw = user["password"] | "";
//         if (pw == password) {
//           found.uuid = user["uuid"] | "";
//           found.password = pw;
//           found.money = user["money"] | 0;
//           found.time = user["time"] | "";
//           return found;
//         }
//       }
//     }
//   }
//   found.password = "";
//   return found;
// }

// String trimString(const String& str) {
//     int start = 0;
//     while (start < str.length() && isspace(str[start])) start++;
//     int end = str.length() - 1;
//     while (end >= 0 && isspace(str[end])) end--;
//     if (end < start) return "";
//     return str.substring(start, end + 1);
// }


// // Biến lưu trạng thái thẻ mới chờ đăng ký khi nhận từ web hoặc quét trực tiếp
// static String pendingNewUID = "";
// static String pendingPassword = "";
// static String pendingTime = "";

// void setup()
// {
//   Serial.begin(9600);
//   bool connected = connectWiFi(WIFI_SSID, WIFI_PASSWORD);
//   if (!connected)
//   {
//     Serial.println("Cannot proceed without WiFi.");
//     // while(true);
//   }
//   bool connect_firebase = ConnectFirebase();
//   initRFID();
//   initLCD();
//   initRTC();
//   initServo(12); // Chỉnh lại số chân phù hợp với mạch của bạn
//   initUART(9600); // Khởi tạo UART với tốc độ 115200
//   initIR(13);
//   // initKeypad();

//   openGate();
//   delay(2000);
//   closeGate();
// }


// void loop() {
//   String uartCmd = readUART();
//   uartCmd = trimString(uartCmd); // Thêm dòng này
//   if (uartCmd.length() > 0) {
//     Serial.print("ESP32 nhan UART: ");
//     Serial.println(uartCmd);
//     Serial.println("]");

//      // Nếu nhận được JSON thẻ mới từ ESP8266 (khi quẹt thẻ lạ trên web)
//     if (uartCmd.startsWith("{\"uuid\":")) {
//       // Parse thủ công hoặc dùng ArduinoJson
//       JsonDocument doc;
//       DeserializationError error = deserializeJson(doc, uartCmd);
//       if (!error) {
//         String uuid = doc["uuid"] | "";
//         String time = doc["time"] | "";
//         String money = doc["money"] | "0";
//         pendingNewUID = uuid;
//         pendingPassword = "1234"; // hoặc lấy từ web nếu muốn
//         pendingTime = time;
//         Serial.println("Luu pendingNewUID tu UART: " + pendingNewUID);
//       }
//     }

//     // if (uartCmd == "IN" || uartCmd == "OUT") {
//     //   Serial.println("Nhan duoc lenh mo cong tu UART: " + uartCmd);
//     //   openGate();
//     //   clearLCD();
//     //   printLCD(0, 0, "Mo cong tu xa");
//     //   delay(2000);
//     //   clearLCD();
//     //   // Nếu có pendingNewUID thì đăng ký thẻ mới
//     //   if (pendingNewUID != "") {
//     //     String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + pendingPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
//     //     updateDataByUUID(pendingNewUID, json);
//     //     pendingNewUID = "";
//     //     pendingPassword = "";
//     //     pendingTime = "";
//     //   }
//     //   return;
//     // }

//     // if (uartCmd == "CLOSE") {
//     //   Serial.println("Nhan duoc lenh dong cong tu UART");
//     //   closeGate();
//     //   clearLCD();
//     //   printLCD(0, 0, "Dong cong tu xa");
//     //   delay(2000);
//     //   clearLCD();
//     //   return;
//     // }
//   }


//   if ((uartCmd == "IN" || uartCmd == "OUT") && pendingNewUID != "") { 
//     Serial.println("Nhan duoc lenh mo cong tu UART: " + uartCmd);
//     String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + pendingPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
//     updateDataByUUID(pendingNewUID, json);
//     openGate();
//     clearLCD();
//     printLCD(0, 0, "Mo cong tu xa");
//     delay(2000);
//     clearLCD();
//     pendingNewUID = "";
//     pendingPassword = "";
//     pendingTime = "";
//     return;
//   }
//   if (uartCmd == "IN" || uartCmd == "OUT") {
//       Serial.println("Nhan duoc lenh mo cong tu UART: " + uartCmd);
//       openGate();
//       clearLCD();
//       printLCD(0, 0, "Mo cong tu xa");
//       delay(2000);
//       clearLCD();
//       // Nếu có pendingNewUID thì đăng ký thẻ mới
//       if (pendingNewUID != "") {
//         String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + pendingPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
//         updateDataByUUID(pendingNewUID, json);
//         pendingNewUID = "";
//         pendingPassword = "";
//         pendingTime = "";
//       }
//       return;
//   }

//     // if (uartCmd == "CLOSE") {
//     //   Serial.println("Nhan duoc lenh dong cong tu UART");
//     //   closeGate();
//     //   clearLCD();
//     //   printLCD(0, 0, "Dong cong tu xa");
//     //   delay(2000);
//     //   clearLCD();
//     //   return;
//     // }
  

//   String cardUID = readCardUID();
//   if (cardUID != "") {
//     Serial.print("UID quet duoc: ");
//     Serial.println(cardUID);
//     unsigned long now = millis();
//     if (cardUID != lastUID || now - lastReadTime > debounceTime) {
//       lastUID = cardUID;
//       lastReadTime = now;

//       String data = getDataByUUID(cardUID);
//       Serial.print("Data tra ve tu Firebase: ");
//       Serial.println(data);
//       if (data.length() > 0 && data != "null") {
//         CardData card = parseCardData(data);
//         sendUART(card.uuid + " đang qua cổng");
//         card.scanCount++; // Tăng số lượt quét

//         if (card.scanCount >= 2) {
//           card.money += 5000; // Cộng dồn 5000đ cho mỗi lượt (2 lần quét)
//           card.scanCount = 0; // Reset lượt
//         }
//         DateTime nowTime = getCurrentTime();
//         String timeStr = formatDateTime(nowTime);
//         card.time = timeStr;

//         // Cập nhật dữ liệu lên Firebase
//         String json = "{\"uuid\":\"" + card.uuid + "\",\"password\":\"" + card.password + "\",\"money\":" + String(card.money) + ",\"time\":\"" + card.time + "\",\"scanCount\":" + String(card.scanCount) + "}";
//         updateDataByUUID(card.uuid, json);
//         clearLCD();
//         printLCD(0, 0, "Quet: " + timeStr);
//         printLCD(0, 1, "No: " + String(card.money));
//         openGate();
//         delay(2000);
//         // clearLCD();
//       } else {
//         clearLCD();
//         printLCD(0, 0, "The moi! Dang ky");
//         String inputPassword = "";
//         clearLCD();
//         printLCD(0, 0, "Nhap mat khau:");
//         printLCD(0, 1, inputPassword);
//         // String password = "1234"; // Hoặc gán tạm mật khẩu mặc định
//         // String timeStr = formatDateTime(getCurrentTime());
        
//         // String json = "{\"uuid\":\"" + cardUID + "\",\"password\":\"" + password + "\",\"money\":0,\"time\":\"" + timeStr + "\",\"scanCount\":0}";
//         // sendUART("phát hiện UUID mới: " + cardUID);
//         // pendingNewUID = cardUID;
//         // pendingPassword = password;
//         // pendingTime = timeStr;
//         // updateDataByUUID(cardUID, json);
//         // printLCD(0, 0, "Dang ky thanh cong");
//         // delay(2000);
//         // clearLCD();



//          // --- Thêm chức năng mở cổng bằng IR remote ---
//         if (pendingNewUID != "") {
//             String inputPassword = "";
//             clearLCD();
//             printLCD(0, 0, "Nhap mat khau IR:");
//             unsigned long startTime = millis();
//             while (millis() - startTime < 15000) { // Cho phép nhập trong 15 giây
//                 uint32_t keyCode;
//                 bool updated = false;
//                 if (getIRKey(keyCode)) {
//                     if (keyCode == IR_CODE_OK) break; // Nhấn OK để xác nhận
//                     if (keyCode == IR_CODE_DEL && inputPassword.length() > 0) {
//                         inputPassword.remove(inputPassword.length() - 1);
//                         updated = true;
//                     }
//                     if (keyCode == IR_CODE_0) { inputPassword += '0'; updated = true; }
//                     else if (keyCode == IR_CODE_1) { inputPassword += '1'; updated = true; }
//                     else if (keyCode == IR_CODE_2) { inputPassword += '2'; updated = true; }
//                     else if (keyCode == IR_CODE_3) { inputPassword += '3'; updated = true; }
//                     else if (keyCode == IR_CODE_4) { inputPassword += '4'; updated = true; }
//                     else if (keyCode == IR_CODE_5) { inputPassword += '5'; updated = true; }
//                     else if (keyCode == IR_CODE_6) { inputPassword += '6'; updated = true; }
//                     else if (keyCode == IR_CODE_7) { inputPassword += '7'; updated = true; }
//                     else if (keyCode == IR_CODE_8) { inputPassword += '8'; updated = true; }
//                     else if (keyCode == IR_CODE_9) { inputPassword += '9'; updated = true; }
//                     if (updated) {
//                         clearLCD();
//                         printLCD(0, 0, "Nhap mat khau IR:");
//                         printLCD(0, 1, inputPassword);
//                         Serial.print("Mat khau da nhap: ");
//                         Serial.println(inputPassword);
//                     }
//                   }
//                 }
//             if (inputPassword == "1234") { // So sánh với mật khẩu mặc định
//                 // Đăng ký thẻ mới
//                 String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + inputPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
//                 updateDataByUUID(pendingNewUID, json);
//                 openGate();
//                 printLCD(0, 0, "Dang ky thanh cong");
//                 delay(2000);
//                 clearLCD();
//                 pendingNewUID = "";
//                 pendingPassword = "";
//                 pendingTime = "";
//             } else {
//                 printLCD(0, 0, "Sai mat khau!");
//                 delay(2000);
//                 clearLCD();
//             }
//         }
//       }
//     }
//   }
//   handleGate();
// }

#include "wifihandler.h"
#include "secrets.h"
#include "firebasehandler.h"
#include "rfidhandler.h"
#include "realtime.h"
#include "lcdhandler.h"
#include "keypadhandler.h"
#include "servo.h"
#include <ArduinoJson.h>
#include "uart.h"
#include "irhandler.h" 
#include "hcsr04handler.h"

struct CardData
{
  String uuid;
  String password;
  int money;
  String time;
  int scanCount;
};

CardData parseCardData(const String &jsonString)
{
  CardData data;
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  if (!error)
  {
    data.uuid = doc["uuid"] | "";
    data.password = doc["password"] | "";
    data.money = doc["money"] | 0;
    data.time = doc["time"] | " ";
    data.scanCount = doc["scanCount"] | 0;
  }
  else
  {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.f_str());
  }
  return data;
}

String lastUID = "";
unsigned long lastReadTime = 0;
const unsigned long debounceTime = 3000;

String trimString(const String& str) {
    int start = 0;
    while (start < str.length() && isspace(str[start])) start++;
    int end = str.length() - 1;
    while (end >= 0 && isspace(str[end])) end--;
    if (end < start) return "";
    return str.substring(start, end + 1);
}

// Biến lưu trạng thái thẻ mới chờ đăng ký khi nhận từ web hoặc quét trực tiếp
static String pendingNewUID = "";
static String pendingPassword = "";
static String pendingTime = "";

void setup()
{
  Serial.begin(9600);
  bool connected = connectWiFi(WIFI_SSID, WIFI_PASSWORD);
  if (!connected)
  {
    Serial.println("Cannot proceed without WiFi.");
    // while(true);
  }
  bool connect_firebase = ConnectFirebase();
  initRFID();
  initLCD();
  initRTC();
  initServo(12); // Chỉnh lại số chân phù hợp với mạch của bạn
  initUART(9600); // Khởi tạo UART với tốc độ 9600
  initIR(13);
  initHCSR04(22, 14); // Thay chân phù hợp với phần cứng của bạn
  // initKeypad();
  float distance = getDistanceCM();
  closeGate();
  // openGate();
  // delay(2000);
  // closeGate();
}

void loop() {
  String uartCmd = readUART();
  uartCmd = trimString(uartCmd);
  if (uartCmd.length() > 0) {
    Serial.print("ESP32 nhan UART: ");
    Serial.println(uartCmd);
    Serial.println("]");

    // Nếu nhận được JSON thẻ mới từ ESP8266 (khi quẹt thẻ lạ trên web)
    if (uartCmd.startsWith("{\"uuid\":")) {
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, uartCmd);
      if (!error) {
        String uuid = doc["uuid"] | "";
        String time = doc["time"] | "";
        String money = doc["money"] | "0";
        pendingNewUID = uuid;
        pendingPassword = "1234"; // hoặc lấy từ web nếu muốn
        pendingTime = time;
        Serial.println("Luu pendingNewUID tu UART: " + pendingNewUID);
      }
    }

    // Nếu nhận lệnh mở cổng từ web (IN/OUT)
    if ((uartCmd == "IN" || uartCmd == "OUT") && pendingNewUID != "") { 
      Serial.println("Nhan duoc lenh mo cong tu UART: " + uartCmd);
      String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + pendingPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
      updateDataByUUID(pendingNewUID, json);
      openGate();
      clearLCD();
      printLCD(0, 0, "Mo cong tu xa");
      delay(2000);
      clearLCD();
      pendingNewUID = "";
      pendingPassword = "";
      pendingTime = "";
      return;
    }
  }

  // Đọc thẻ RFID
  String cardUID = readCardUID();
  if (cardUID != "") {
    Serial.print("UID quet duoc: ");
    Serial.println(cardUID);
    unsigned long now = millis();
    if (cardUID != lastUID || now - lastReadTime > debounceTime) {
      lastUID = cardUID;
      lastReadTime = now;

      String data = getDataByUUID(cardUID);
      Serial.print("Data tra ve tu Firebase: ");
      Serial.println(data);
      if (data.length() > 0 && data != "null") {
        CardData card = parseCardData(data);
        sendUART(card.uuid + " đang qua cổng");
        card.scanCount++; // Tăng số lượt quét

        if (card.scanCount >= 2) {
          card.money += 5000; // Cộng dồn 5000đ cho mỗi lượt (2 lần quét)
          card.scanCount = 0; // Reset lượt
        }
        DateTime nowTime = getCurrentTime();
        String timeStr = formatDateTime(nowTime);
        card.time = timeStr;

        // Cập nhật dữ liệu lên Firebase
        String json = "{\"uuid\":\"" + card.uuid + "\",\"password\":\"" + card.password + "\",\"money\":" + String(card.money) + ",\"time\":\"" + card.time + "\",\"scanCount\":" + String(card.scanCount) + "}";
        updateDataByUUID(card.uuid, json);
        clearLCD();
        printLCD(0, 0, "Quet: " + timeStr);
        printLCD(0, 1, "No: " + String(card.money));
        openGate();
        delay(2000);
        // clearLCD();
      } else {
        // Thẻ mới, chưa có trên Firebase
        clearLCD();
        printLCD(0, 0, "The moi! Dang ky");
        String inputPassword = "";
        clearLCD();
        printLCD(0, 0, "Nhap mat khau IR:");
        printLCD(0, 1, inputPassword);

        // Gửi thông báo lên web để đăng ký nếu muốn
        // sendUART("phát hiện UUID mới: " + cardUID);

        // Lưu trạng thái chờ đăng ký
        pendingNewUID = cardUID;
        pendingPassword = "";
        DateTime nowTime = getCurrentTime();
        pendingTime = formatDateTime(nowTime);

        // Nhập mật khẩu bằng IR remote
        unsigned long startTime = millis();
        while (millis() - startTime < 15000 && pendingNewUID != "") { // Cho phép nhập trong 15 giây
          uint64_t keyCode;
          bool updated = false;
          if (getIRKey(keyCode)) {
            char key = '?';
            switch (keyCode) {
              case IR_CODE_1: key = '1'; break;
              case IR_CODE_2: key = '2'; break;
              case IR_CODE_3: key = '3'; break;
              case IR_CODE_4: key = '4'; break;
              case IR_CODE_5: key = '5'; break;
              case IR_CODE_6: key = '6'; break;
              case IR_CODE_7: key = '7'; break;
              case IR_CODE_8: key = '8'; break;
              case IR_CODE_9: key = '9'; break;
              case IR_CODE_C: key = 'C'; break; // Cancel
              case IR_CODE_E: key = 'E'; break; // OK/Enter
            }
            if (key != '?') {
              Serial.print("IR Key pressed: ");
              Serial.println(key); // In ra ký tự nút nhấn
              Serial.print("IR Raw code: 0x");
              Serial.println((uint32_t)keyCode, HEX);
              if (key == '0') {
                inputPassword += '0';
                updated = true;
              } else if (key == '1') {
                inputPassword += '1';
                updated = true;
              } else if (key == '2') {
                inputPassword += '2';
                updated = true;
              } else if (key == '3') {
                inputPassword += '3';
                updated = true;
              } else if (key == '4') {
                inputPassword += '4';
                updated = true;
              } else if (key == '5') {
                inputPassword += '5';
                updated = true;
              } else if (key == '6') {
                inputPassword += '6';
                updated = true;
              } else if (key == '7') {
                inputPassword += '7';
                updated = true;
              } else if (key == '8') {
                inputPassword += '8';
                updated = true;
              } else if (key == '9') {
                inputPassword += '9';
                updated = true;
              } else
              if (key == 'C') {
                inputPassword = "";
                updated = true;
              } else if (key == 'E') {
                Serial.print("Mat khau da nhap: ");
                Serial.println(inputPassword);
                if (inputPassword == "1234") { // So sánh với mật khẩu mặc định
                  String json = "{\"uuid\":\"" + pendingNewUID + "\",\"password\":\"" + inputPassword + "\",\"money\":0,\"time\":\"" + pendingTime + "\",\"scanCount\":0}";
                  updateDataByUUID(pendingNewUID, json);
                  openGate();
                  printLCD(0, 0, "Dang ky thanh cong");
                  delay(2000);
                  clearLCD();
                  pendingNewUID = "";
                  pendingPassword = "";
                  pendingTime = "";
                } else {
                  printLCD(0, 0, "Sai mat khau!");
                  delay(2000);
                  clearLCD();
                  inputPassword = "";
                  updated = true;
                }
                break; // Thoát khỏi vòng nhập mật khẩu
              } else {
                inputPassword += key;
                updated = true;
              }
            }
          }
          if (updated) {
            clearLCD();
            printLCD(0, 0, "Nhap mat khau IR:");
            printLCD(0, 1, inputPassword);
            Serial.print("Mat khau da nhap: ");
            Serial.println(inputPassword);
          }
        }
        // Nếu hết thời gian mà chưa nhập xong
        if (pendingNewUID != "") {
          printLCD(0, 0, "Het thoi gian!");
          delay(2000);
          clearLCD();
          pendingNewUID = "";
          pendingPassword = "";
          pendingTime = "";
        }
        return;
      }
    }
  }

  handleGate();
}