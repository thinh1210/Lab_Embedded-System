/*
 * ds3232.c
 *
 *  Created on: Nov 7, 2025
 *      Author: Divu
 */

#include "ds3231.h"

#define DS3231_ADDRESS 0x68 << 1

uint8_t ds3231_alarm1_flag = 0;
uint8_t ds3231_alarm2_flag = 0;

uint8_t ds3231_buffer[7];

uint8_t ds3231_hours;
uint8_t ds3231_min;
uint8_t ds3231_sec;
uint8_t ds3231_date;
uint8_t ds3231_day;
uint8_t ds3231_month;
uint8_t ds3231_year;

uint8_t ds3231_a1_second;
uint8_t ds3231_a1_minute;
uint8_t ds3231_a1_hour;
uint8_t ds3231_a1_day;
uint8_t ds3231_a1_isDayOfWeek;

uint8_t ds3231_a2_minute;
uint8_t ds3231_a2_hour;
uint8_t ds3231_a2_day;
uint8_t ds3231_a2_isDayOfWeek;

void ds3231_init()
{
	ds3231_buffer[0] = DEC2BCD(30); // second
	ds3231_buffer[1] = DEC2BCD(22); // minute
	ds3231_buffer[2] = DEC2BCD(21); // hour
	ds3231_buffer[3] = DEC2BCD(6);	// day
	ds3231_buffer[4] = DEC2BCD(15); // date
	ds3231_buffer[5] = DEC2BCD(9);	// month
	ds3231_buffer[6] = DEC2BCD(23); // year
	if (HAL_I2C_IsDeviceReady(&hi2c1, DS3231_ADDRESS, 3, 50) != HAL_OK)
	{
		while (1)
			;
	};
}

void ds3231_Write(uint8_t address, uint8_t value)
{
	uint8_t temp = DEC2BCD(value);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, address, I2C_MEMADD_SIZE_8BIT, &temp, 1, 10);
}

void ds3231_ReadTime()
{
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, ds3231_buffer, 7, 10);
	ds3231_sec = BCD2DEC(ds3231_buffer[0]);
	ds3231_min = BCD2DEC(ds3231_buffer[1]);
	ds3231_hours = BCD2DEC(ds3231_buffer[2]);
	ds3231_day = BCD2DEC(ds3231_buffer[3]);
	ds3231_date = BCD2DEC(ds3231_buffer[4]);
	ds3231_month = BCD2DEC(ds3231_buffer[5]);
	ds3231_year = BCD2DEC(ds3231_buffer[6]);
}

/**
 * @brief HÀM 1: GHI (Cài đặt) ALARM1
 */
void ds3231_SetAlarm1(uint8_t day, uint8_t isDayOfWeek, uint8_t hour, uint8_t minute, uint8_t second)
{
	// Mảng 4 byte để chứa dữ liệu 4 thanh ghi A1 (0x07 - 0x0A)
	uint8_t alarm_data[4];

	// 1. Xử lý Giây (Thanh ghi 0x07)
	if (second == ALARM_SKIP)
	{
		alarm_data[0] = 0x80; // Set Bit 7 (A1M1) để mask (bỏ qua)
	}
	else
	{
		alarm_data[0] = DEC2BCD(second) & 0x7F; // Clear Bit 7 (A1M1)
	}

	// 2. Xử lý Phút (Thanh ghi 0x08)
	if (minute == ALARM_SKIP)
	{
		alarm_data[1] = 0x80; // Set Bit 7 (A1M2)
	}
	else
	{
		alarm_data[1] = DEC2BCD(minute) & 0x7F; // Clear Bit 7
	}

	// 3. Xử lý Giờ (Thanh ghi 0x09) - Luôn ở chế độ 24h
	if (hour == ALARM_SKIP)
	{
		alarm_data[2] = 0x80; // Set Bit 7 (A1M3)
	}
	else
	{
		alarm_data[2] = DEC2BCD(hour) & 0xBF; // Clear Bit 7 (A1M3) VÀ Bit 6 (chế độ 24h)
	}

	// 4. Xử lý Ngày/Thứ (Thanh ghi 0x0A)
	if (day == ALARM_SKIP)
	{
		alarm_data[3] = 0x80; // Set Bit 7 (A1M4)
	}
	else
	{
		alarm_data[3] = DEC2BCD(day) & 0x7F; // Clear Bit 7 (A1M4)
		if (isDayOfWeek)
		{
			alarm_data[3] |= (1 << 6); // Set Bit 6 (DY/DT) để so khớp Thứ
		}
		// (Nếu isDayOfWeek=0, bit 6 đã = 0, nên sẽ so khớp Ngày)
	}

	// 5. Ghi 4 byte dữ liệu vào DS3231 một lúc
	// Bắt đầu từ địa chỉ 0x07 (Thanh ghi Giây ALARM1)
	HAL_I2C_Mem_Write(&hi2c1,
					  DS3231_ADDRESS,
					  0x07, // Địa chỉ bắt đầu của ALARM1
					  I2C_MEMADD_SIZE_8BIT,
					  alarm_data,
					  4,	// Ghi 4 byte
					  100); // Timeout 100ms
}

/**
 * @brief HÀM 2: ĐỌC "TÍN HIỆU" (Cờ báo)
 */
void ds3231_ReadAlarmFlags(void)
{
	uint8_t status_reg;

	// Đọc thanh ghi Status (địa chỉ 0x0F)
	HAL_I2C_Mem_Read(&hi2c1,
					 DS3231_ADDRESS,
					 0x0F, // Địa chỉ thanh ghi Status
					 I2C_MEMADD_SIZE_8BIT,
					 &status_reg,
					 1,
					 100);

	// Cập nhật các biến "tín hiệu" toàn cục
	ds3231_alarm1_flag = (status_reg & 0x01); // Bit 0 là A1F
	ds3231_alarm2_flag = (status_reg & 0x02); // Bit 1 là A2F
}

/**
 * @brief HÀM 3: XÓA "TÍN HIỆU" (Cờ báo)
 */
void ds3231_ClearAlarmFlags(void)
{
	uint8_t status_reg;

	// 1. Đọc giá trị hiện tại của thanh ghi Status
	HAL_I2C_Mem_Read(&hi2c1,
					 DS3231_ADDRESS,
					 0x0F,
					 I2C_MEMADD_SIZE_8BIT,
					 &status_reg,
					 1,
					 100);

	// 2. Xóa Bit 0 (A1F) và Bit 1 (A2F)
	// Bằng cách ghi lại chính thanh ghi đó với 2 bit này = 0
	uint8_t new_status = status_reg & 0xFC; // 0xFC = 11111100

	// 3. Ghi giá trị mới trở lại
	HAL_I2C_Mem_Write(&hi2c1,
					  DS3231_ADDRESS,
					  0x0F,
					  I2C_MEMADD_SIZE_8BIT,
					  &new_status,
					  1,
					  100);
}

/**
 * @brief Hàm phụ trợ: Chuyển đổi BCD của Giờ (chế độ 24h) sang Thập phân.
 * @note  Hàm BCD2DEC(val) thông thường SẼ BỊ LỖI với thanh ghi Giờ.
 */
uint8_t bcd2dec_hour_24h(uint8_t val)
{
	uint8_t hour = 0;
	val &= 0x3F; // Mask off Bit 7 (Mask) và Bit 6 (12/24)

	hour = (val & 0x0F); // Hàng đơn vị (Bits 0-3)

	if (val & 0x10) // Bit 4 (10 Giờ)
		hour += 10;
	if (val & 0x20) // Bit 5 (20 Giờ)
		hour += 20;

	return hour;
}

/**
 * @brief HÀM MỚI: Đọc tất cả cài đặt của ALARM1 và ALARM2.
 */
void ds3231_ReadAlarmSettings(void)
{
	// Mảng 7 byte để đọc A1 (4 byte) và A2 (3 byte)
	uint8_t alarm_buffer[7];
	uint8_t temp;

	// Đọc một lúc 7 byte, bắt đầu từ địa chỉ 0x07 (A1 Giây)
	HAL_I2C_Mem_Read(&hi2c1,
					 DS3231_ADDRESS,
					 0x07, // Địa chỉ bắt đầu (ALARM1 Seconds)
					 I2C_MEMADD_SIZE_8BIT,
					 alarm_buffer,
					 7, // Đọc 7 byte (0x07 đến 0x0D)
					 100);

	// --- Phân tích ALARM 1 (alarm_buffer[0] đến [3]) ---

	// A1 Giây (buffer[0])
	temp = alarm_buffer[0];
	if (temp & 0x80)
		ds3231_a1_second = ALARM_SKIP; // Bị mask
	else
		ds3231_a1_second = BCD2DEC(temp & 0x7F);

	// A1 Phút (buffer[1])
	temp = alarm_buffer[1];
	if (temp & 0x80)
		ds3231_a1_minute = ALARM_SKIP; // Bị mask
	else
		ds3231_a1_minute = BCD2DEC(temp & 0x7F);

	// A1 Giờ (buffer[2])
	temp = alarm_buffer[2];
	if (temp & 0x80)
		ds3231_a1_hour = ALARM_SKIP; // Bị mask
	else
		ds3231_a1_hour = bcd2dec_hour_24h(temp); // Dùng hàm helper mới

	// A1 Ngày/Thứ (buffer[3])
	temp = alarm_buffer[3];
	if (temp & 0x80)
	{
		ds3231_a1_day = ALARM_SKIP; // Bị mask
		ds3231_a1_isDayOfWeek = 0;	// Không quan trọng
	}
	else
	{
		ds3231_a1_isDayOfWeek = (temp & 0x40) ? 1 : 0; // Kiểm tra bit 6 (DY/DT)
		ds3231_a1_day = BCD2DEC(temp & 0x3F);		   // Mask bit 7 và 6
	}

	// --- Phân tích ALARM 2 (alarm_buffer[4] đến [6]) ---

	// A2 Phút (buffer[4])
	temp = alarm_buffer[4];
	if (temp & 0x80)
		ds3231_a2_minute = ALARM_SKIP;
	else
		ds3231_a2_minute = BCD2DEC(temp & 0x7F);

	// A2 Giờ (buffer[5])
	temp = alarm_buffer[5];
	if (temp & 0x80)
		ds3231_a2_hour = ALARM_SKIP;
	else
		ds3231_a2_hour = bcd2dec_hour_24h(temp);

	// A2 Ngày/Thứ (buffer[6])
	temp = alarm_buffer[6];
	if (temp & 0x80)
	{
		ds3231_a2_day = ALARM_SKIP;
		ds3231_a2_isDayOfWeek = 0;
	}
	else
	{
		ds3231_a2_isDayOfWeek = (temp & 0x40) ? 1 : 0;
		ds3231_a2_day = BCD2DEC(temp & 0x3F);
	}
}