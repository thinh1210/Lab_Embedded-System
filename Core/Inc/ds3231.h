/*
 * ds3231.h
 *
 *  Created on: Nov 7, 2025
 *      Author: Divu
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "i2c.h"
#include "utils.h"

extern uint8_t ds3231_alarm1_flag;
extern uint8_t ds3231_alarm2_flag;

#define ADDRESS_SEC 0x00
#define ADDRESS_MIN 0x01
#define ADDRESS_HOUR 0x02
#define ADDRESS_DAY 0x03
#define ADDRESS_DATE 0x04
#define ADDRESS_MONTH 0x05
#define ADDRESS_YEAR 0x06

#define ALARM_SKIP 99
#define DS3231_ADDRESS 0x68 << 1

extern uint8_t ds3231_hours;
extern uint8_t ds3231_min;
extern uint8_t ds3231_sec;
extern uint8_t ds3231_date;
extern uint8_t ds3231_day;
extern uint8_t ds3231_month;
extern uint8_t ds3231_year;

// ALARM 1
extern uint8_t ds3231_a1_second;
extern uint8_t ds3231_a1_minute;
extern uint8_t ds3231_a1_hour;
extern uint8_t ds3231_a1_day;
extern uint8_t ds3231_a1_isDayOfWeek; // 1 = Day, 0 = Date
// ALARM 2
extern uint8_t ds3231_a2_minute;
extern uint8_t ds3231_a2_hour;
extern uint8_t ds3231_a2_day;
extern uint8_t ds3231_a2_isDayOfWeek; // 1 = Day, 0 = Date
void ds3231_init();

void ds3231_Write(uint8_t address, uint8_t value);

void ds3231_ReadTime();

/**
 * @brief Cài đặt ALARM1 (Ghi vào thanh ghi)
 * @note  Dùng ALARM_SKIP (99) cho bất kỳ tham số nào muốn bỏ qua.
 * @note  isDayOfWeek: 0 = So khớp Ngày (Date), 1 = So khớp Thứ (Day)
 */
void ds3231_SetAlarm1(uint8_t day, uint8_t isDayOfWeek, uint8_t hour, uint8_t minute, uint8_t second);

/**
 * @brief Đọc "tín hiệu" (cờ báo) từ thanh ghi Status
 * @note  Sẽ cập nhật biến toàn cục: ds3231_alarm1_flag, ds3231_alarm2_flag
 */
void ds3231_ReadAlarmFlags(void);

/**
 * @brief Xóa cờ báo A1F và A2F sau khi đã xử lý.
 * @note  BẮT BUỘC phải gọi hàm này sau khi phát hiện báo thức.
 */
void ds3231_ClearAlarmFlags(void);

/**
 * @brief Đọc tất cả cài đặt của ALARM1 và ALARM2.
 * @note  Cập nhật các biến global ds3231_a1_... và ds3231_a2_...
 * @note  Nếu một giá trị bị mask (bỏ qua), nó sẽ được gán là ALARM_SKIP (99).
 */
void ds3231_ReadAlarmSettings(void);

#endif /* INC_DS3231_H_ */
