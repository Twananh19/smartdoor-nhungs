#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include <Arduino.h>

// Định nghĩa mã hex cho từng nút IR remote (dùng giá trị thực tế bạn đã test)
#define IR_CODE_1   0xF30CFF00UL
#define IR_CODE_2   0xE718FF00UL
#define IR_CODE_3   0xA15EFF00UL
#define IR_CODE_4   0xF708FF00UL
#define IR_CODE_5   0xE31CFF00UL
#define IR_CODE_6   0xA55AFF00UL
#define IR_CODE_7   0xBD42FF00UL
#define IR_CODE_8   0xAD52FF00UL
#define IR_CODE_9   0xB54AFF00UL
#define IR_CODE_C   0xBC43FF00UL   // Cancel (Xóa)
#define IR_CODE_E   0xBB44FF00UL   // OK (Enter)

// Nếu bạn muốn dùng thêm các số khác, hãy bổ sung mã hex tương ứng
// #define IR_CODE_0   ... 
// #define IR_CODE_5   ... 
// v.v.

// Để tương thích với hàm getIRKey, bạn có thể dùng decodedRawData thay vì command

void initIR(int pin);
bool getIRKey(uint64_t &keyCode); // Sửa lại kiểu dữ liệu cho đúng với mã hex thực tế

#endif