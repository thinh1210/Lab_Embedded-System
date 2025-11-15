/*
 * lab4.c
 *
 *  Created on: Nov 11, 2025
 *      Author: Divu
 */
/*Include --------------------------------------------*/
#include "lab4.h"
/*Define ---------------------------------------------*/

/*Enum   --------------------------------------------*/

/*Struct --------------------------------------------*/

/*Variables------------------------------------------*/

/*Function -------------------------------------------*/
uint8_t Blink = 0;
TIME_LAB4 pos;
uint8_t isENpress = 0;
uint8_t isBACKpress = 0;
uint8_t isNEXTpress = 0;
uint8_t isUPpress = 0;
uint8_t isOnePress = 0;
uint8_t isLongPress = 0;
uint8_t isButtonPress = 0;
uint8_t countAlarm = 0;
lAB4_MAIN_STATE lab4_state = NORMAL;
EDIT_TIME_STATE edit_time_state = EDIT_TIME_SEC;
EDIT_ALARM_STATE edit_alarm_state = EDIT_ALARM_SEC;
ALARM_STATE alarm_state = NO_ALARM;

uint8_t bufferTime[7] = {0};
uint8_t bufferAlarm[5] = {0};
void initLab4(void)
{
    timer2Init();
    lcd_init();
    buttonInit();
    ds3231_init();
    lcd_clear(BLACK);
    timerInit(0, 200, 200, ds3231_ReadTime);
    timerInit(1, 1000, 1000, checkAlarm);
    timerInit(2, 50, 50, buttonScan);
    timerInit(3, 100, 100, mainFsm);
    timerInit(4, 250, 250, blinkLed);
    timerInit(5, 500, 500, updateTimeLCD);
    ds3231_ClearAlarmFlags();
    disableTask(4);
    lab4_state = NORMAL;
    edit_time_state = TIME_SEC;
    edit_alarm_state = EDIT_ALARM_SEC;
    initNormal();
}
void runLab4()
{
    doTask();
}
void checkAlarm()
{
    ds3231_ReadAlarmFlags();
    switch (alarm_state)
    {
    case NO_ALARM:
        /* code */
        if (ds3231_alarm1_flag == 1)
        {
            alarm_state = ALARM;
            lcd_show_string(0, 40, "ALARM!!!", YELLOW, BLACK, 32, 0);
            countAlarm = TIME_WARNING;
            ds3231_alarm1_flag = 0;
            ds3231_ClearAlarmFlags();
        }
        break;
    case ALARM:
        countAlarm--;
        if (countAlarm <= 0)
        {
            alarm_state = NO_ALARM;
            lcd_show_string(0, 40, "ALARM!!!", BLACK, BLACK, 32, 0);
        }
    default:
        break;
    }
}
void displayLCD(uint8_t number, TIME_LAB4 pos, uint8_t hiden)
{
    uint16_t x = 0;
    uint16_t y = 0;
    switch (pos)
    {
    case TIME_SEC:
        x = 70;
        y = 100;
        break;
    case TIME_MIN:
        /* code */
        x = 110;
        y = 100;
        break;
    case TIME_HOUR:
        x = 150;
        y = 100;
        /* code */
        break;
    case TIME_DATE:
        x = 70;
        y = 130;
        /* code */
        break;
    case TIME_MONTH:
        /* code */
        x = 110;
        y = 130;
        break;
    case TIME_YEAR:

        /* code */
        x = 150;
        y = 130;
        break;
    case TIME_DAY:
        x = 20;
        y = 130;
        break;

    default:
        break;
    }
    if (hiden == 1)
        lcd_show_string(x, y, "--", BLACK, BLACK, 24, 0);
    else
    {
        lcd_show_int_num(x, y, number, 2, YELLOW, BLACK, 24);
        if (number == ALARM_SKIP)
            lcd_show_string(x, y, "--", YELLOW, BLACK, 24, 0);
    }
}
void blinkLed()
{
    if (lab4_state == NORMAL)
        return;

    uint8_t number;
    TIME_LAB4 pos_to_blink;
    uint16_t x = 0, y = 0;

    if (lab4_state == EDIT_TIME)
    {
        pos_to_blink = (TIME_LAB4)edit_time_state;
        number = bufferTime[pos_to_blink];
    }
    else
    {
        pos_to_blink = (TIME_LAB4)edit_alarm_state;
        number = bufferAlarm[pos_to_blink];
    }

    // *** LỖI CẦN SỬA: PHẢI LẤY TỌA ĐỘ X, Y ***
    switch (pos_to_blink)
    {
    case TIME_SEC:
        x = 70;
        y = 100;
        break;
    case TIME_MIN:
        x = 110;
        y = 100;
        break;
    case TIME_HOUR:
        x = 150;
        y = 100;
        break;
    case TIME_DATE:
        x = 70;
        y = 130;
        break;
    case TIME_MONTH:
        x = 110;
        y = 130;
        break;
    case TIME_YEAR:
        x = 150;
        y = 130;
        break;
    case TIME_DAY:
        x = 20;
        y = 130;
        break;
    default:
        return;
    }

    Blink = 1 - Blink; // Lật cờ 0 -> 1 -> 0

    if (Blink)
    {
        // Hiện: Vẽ màu VÀNG
        if (number == ALARM_SKIP)
            lcd_show_string(x, y, "--", YELLOW, BLACK, 24, 0);
        else
            lcd_show_int_num(x, y, number, 2, YELLOW, BLACK, 24);
    }
    else
    {
        // Ẩn: Vẽ màu ĐEN
        if (number == ALARM_SKIP)
            lcd_show_string(x, y, "--", BLACK, BLACK, 24, 0);
        else
            lcd_show_int_num(x, y, number, 2, BLACK, BLACK, 24);
    }
}

void updateTimeLCD()
{
    ds3231_ReadTime();
    bufferTime[TIME_SEC] = ds3231_sec;
    bufferTime[TIME_MIN] = ds3231_min;
    bufferTime[TIME_HOUR] = ds3231_hours;
    bufferTime[TIME_DATE] = ds3231_date;
    bufferTime[TIME_MONTH] = ds3231_month;
    bufferTime[TIME_YEAR] = ds3231_year;
    bufferTime[TIME_DAY] = ds3231_day;

    displayLCD(bufferTime[TIME_SEC], TIME_SEC, 0);
    displayLCD(bufferTime[TIME_MIN], TIME_MIN, 0);
    displayLCD(bufferTime[TIME_HOUR], TIME_HOUR, 0);
    displayLCD(bufferTime[TIME_DATE], TIME_DATE, 0);
    displayLCD(bufferTime[TIME_MONTH], TIME_MONTH, 0);
    displayLCD(bufferTime[TIME_YEAR], TIME_YEAR, 0);
    displayLCD(bufferTime[TIME_DAY], TIME_DAY, 0);
}

void initNormal(void)
{

    lcd_show_string(0, 5, DELETE_FILL, BLACK, BLACK, 24, 0);
    lcd_show_string(0, 5, NORMAL_MODE, WHITE, BLACK, 24, 0);
    updateTimeLCD();
    ds3231_ClearAlarmFlags();
    enableTask(5); // enable task update information every seconds
    enableTask(1);
    disableTask(4); // disable task blink led
}
void initEditTime(void)
{
    lcd_show_string(0, 5, DELETE_FILL, BLACK, BLACK, 24, 0);
    lcd_show_string(0, 5, EDIT_TIME_MODE, WHITE, BLACK, 24, 0);

    displayLCD(bufferTime[TIME_SEC], TIME_SEC, 0);
    displayLCD(bufferTime[TIME_MIN], TIME_MIN, 0);
    displayLCD(bufferTime[TIME_HOUR], TIME_HOUR, 0);
    displayLCD(bufferTime[TIME_DATE], TIME_DATE, 0);
    displayLCD(bufferTime[TIME_MONTH], TIME_MONTH, 0);
    displayLCD(bufferTime[TIME_YEAR], TIME_YEAR, 0);
    displayLCD(bufferTime[TIME_DAY], TIME_DAY, 0);
    edit_time_state = TIME_SEC;
    disableTask(1);
    enableTask(4);  // enable task update information every seconds
    disableTask(5); // disable task blink led
}

void initEditAlarm(void)
{
    lcd_show_string(0, 5, DELETE_FILL, BLACK, BLACK, 24, 0);
    lcd_show_string(0, 5, EDIT_ALARM_MODE, WHITE, BLACK, 24, 0);

    ds3231_ReadAlarmSettings();
    bufferAlarm[EDIT_ALARM_SEC] = ds3231_a1_second;
    bufferAlarm[EDIT_ALARM_MIN] = ds3231_a1_minute;
    bufferAlarm[EDIT_ALARM_HOUR] = ds3231_a1_hour;
    bufferAlarm[EDIT_ALARM_DATE] = ds3231_a1_day;
    bufferAlarm[EDIT_ALARM_DAY] = ds3231_a1_isDayOfWeek;

    displayLCD(bufferAlarm[EDIT_ALARM_SEC], EDIT_ALARM_SEC, 0);
    displayLCD(bufferAlarm[EDIT_ALARM_MIN], EDIT_ALARM_MIN, 0);
    displayLCD(bufferAlarm[EDIT_ALARM_HOUR], EDIT_ALARM_HOUR, 0);
    displayLCD(bufferAlarm[EDIT_ALARM_DATE], EDIT_ALARM_DATE, 0);
    displayLCD(bufferAlarm[EDIT_ALARM_DAY], EDIT_ALARM_DAY, 0);
    displayLCD(bufferAlarm[EDIT_ALARM_DAY], TIME_MONTH, 1);
    displayLCD(bufferAlarm[EDIT_ALARM_DAY], TIME_YEAR, 1);
    edit_alarm_state = EDIT_ALARM_SEC;
    disableTask(1);
    enableTask(4);  // enable task update information every seconds
    disableTask(5); // disable task blink led
}

void mainFsm(void)
{
    //  BUTTONEN -> save all -> next into MAIN STATE
    //  BUTTON1 -> change edit element in tw o mode
    // BUTTONBACK ->  don't need to check  -> alarm to ALARM SKIP -> next state when alarm skip

    //  BUTTONNEXT-> next into MAIN STATE
    //  BUTTONUP -> edit each  -> up for each kinds of time
    //  longpress -> update every

    isENpress = buttonFirstPress(BUTTON_EN); //
    isBACKpress = buttonFirstPress(BUTTON_BACK);
    isNEXTpress = buttonFirstPress(BUTTON_RIGHT);
    isUPpress = buttonFirstPress(BUTTON_UP);
    isOnePress = buttonFirstPress(BUTTON_1);

    // Hiển thị nút nhấn lên LCD
    if (isENpress)
        lcd_show_string(0, 160, "EN       ", WHITE, BLACK, 16, 0);
    else if (isBACKpress)
        lcd_show_string(0, 160, "BACK     ", WHITE, BLACK, 16, 0);
    else if (isNEXTpress)
        lcd_show_string(0, 160, "NEXT     ", WHITE, BLACK, 16, 0);
    else if (isUPpress)
        lcd_show_string(0, 160, "UP       ", WHITE, BLACK, 16, 0);
    else if (isOnePress)
        lcd_show_string(0, 160, "BUTTON 1 ", WHITE, BLACK, 16, 0);
    else if (longPressTrigger)
        lcd_show_string(0, 160, "LONG HOLD", WHITE, BLACK, 16, 0);
    else
        lcd_show_string(0, 160, "         ", BLACK, BLACK, 16, 0);

    isButtonPress = isENpress || isBACKpress || isNEXTpress || isUPpress || isOnePress || longPressTrigger;
    if (isButtonPress == 0)
        return;
    switch (lab4_state)
    {
    case NORMAL:
        /* code */
        if (isNEXTpress)
        {
            isENpress = 0;
            lab4_state = EDIT_TIME;
            initEditTime();
            return;
        }
        break;
    case EDIT_TIME:
        if (isNEXTpress)
        {
            isENpress = 0;
            lab4_state = EDIT_ALARM;
            initEditAlarm();
            return;
        }
        if (isBACKpress)
            return;

        editTimeFsm();

        break;
    case EDIT_ALARM:
        if (isNEXTpress)
        {
            isENpress = 0;
            lab4_state = NORMAL;
            initNormal();
            return;
        }
        editAlarmFsm();
        break;
    default:

        break;
    }
}

/**
 * @brief Tính toán Thứ trong tuần (Day of Week)
 * @param year  Năm (0-99, nghĩa là 2000-2099)
 * @param month Tháng (1-12)
 * @param date  Ngày (1-31)
 * @return Thứ trong tuần (1=Chủ Nhật, 2=Thứ Hai, ..., 7=Thứ Bảy)
 * @note   DS3231 dùng quy ước 1-7 (1=CN, 2=T2, ... 7=T7)
 */
uint8_t ds3231_calculateDayOfWeek(uint8_t year, uint8_t month, uint8_t date)
{
    // 1. Chuyển đổi năm (0-99) sang năm đầy đủ (2000-2099)
    uint16_t y = year + 2000;
    uint8_t m = month;
    uint8_t d = date;

    // 2. Bảng tra (Lookup table) cho thuật toán Sakamoto
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    // 3. Nếu tháng là 1 (Jan) hoặc 2 (Feb), nó được coi là
    //    tháng 13 và 14 của năm trước.
    if (m < 3)
    {
        y--;
    }

    // 4. Công thức tính toán
    //    Kết quả (dow) sẽ là 0=Chủ Nhật, 1=Thứ Hai, ..., 6=Thứ Bảy
    uint8_t dow = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;

    // 5. Chuyển đổi sang chuẩn của DS3231 (1-7)
    //    0 (CN) -> 1
    //    1 (T2) -> 2
    //    ...
    //    6 (T7) -> 7
    return dow + 1;
}
void editTimeFsm(void)
{
    //  BUTTONEN -> save all -> next into MAIN STATE
    //  BUTTON1 -> change edit element in two mode
    // BUTTONBACK ->  don't need to check  -> alarm to ALARM SKIP -> next state when alarm skip

    //  BUTTONNEXT-> next into MAIN STATE
    //  BUTTONUP -> edit each  -> up for each kinds of time
    //  longpress -> update every
    /* Bên trong hàm mainFsm() */

    if (isENpress)
    {
        isENpress = 0;

        // 1. TÍNH TOÁN LẠI THỨ (DAY) TỪ NGÀY/THÁNG/NĂM
        //    Hàm này sẽ tự động cập nhật bufferTime[EDIT_TIME_DAY]
        bufferTime[EDIT_TIME_DAY] = ds3231_calculateDayOfWeek(
            bufferTime[EDIT_TIME_YEAR],
            bufferTime[EDIT_TIME_MONTH],
            bufferTime[EDIT_TIME_DATE]);

        // 2. LƯU TẤT CẢ DỮ LIỆU VÀO DS3231
        //    (Giờ đây bufferTime[EDIT_TIME_DAY] đã chính xác)
        ds3231_Write(ADDRESS_SEC, bufferTime[EDIT_TIME_SEC]);
        ds3231_Write(ADDRESS_MIN, bufferTime[EDIT_TIME_MIN]);
        ds3231_Write(ADDRESS_HOUR, bufferTime[EDIT_TIME_HOUR]);
        ds3231_Write(ADDRESS_DAY, bufferTime[EDIT_TIME_DAY]); // Ghi thứ đã được tính
        ds3231_Write(ADDRESS_DATE, bufferTime[EDIT_TIME_DATE]);
        ds3231_Write(ADDRESS_MONTH, bufferTime[EDIT_TIME_MONTH]);
        ds3231_Write(ADDRESS_YEAR, bufferTime[EDIT_TIME_YEAR]);

        // 3. CHUYỂN TRẠNG THÁI
        lab4_state = EDIT_ALARM;
        initEditAlarm();
        return;
    }

    switch (edit_time_state)
    {
    case EDIT_TIME_SEC:
        /* code */
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_MIN;
            displayLCD(bufferTime[EDIT_TIME_SEC], TIME_SEC, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeSec();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeSec();
        }

        break;

    case EDIT_TIME_MIN:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_HOUR;
            displayLCD(bufferTime[EDIT_TIME_MIN], TIME_MIN, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeMin();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeMin();
        }

        /* code */
        break;
    case EDIT_TIME_HOUR:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_DATE;
            displayLCD(bufferTime[EDIT_TIME_HOUR], TIME_HOUR, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeHour();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeHour();
        }

        break;
    case EDIT_TIME_DATE:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_MONTH;
            displayLCD(bufferTime[EDIT_TIME_DATE], TIME_DATE, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeDate();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeDate();
        }

        break;
    case EDIT_TIME_MONTH:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_YEAR;
            displayLCD(bufferTime[EDIT_TIME_MONTH], TIME_MONTH, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeMonth();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeMonth();
        }

        break;
    case EDIT_TIME_YEAR:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_time_state = EDIT_TIME_SEC;
            displayLCD(bufferTime[EDIT_TIME_YEAR], TIME_YEAR, 0);
        }
        if (isUPpress)
        {
            isUPpress = 0;
            updateTimeYear();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateTimeYear();
        }

        break;
        // case EDIT_TIME_DAY:
        //     if (isOnePress)
        //     {
        //         isOnePress = 0;
        //         /* wrap around to first field */
        //         edit_time_state = EDIT_TIME_MONTH;
        //     } // switch mode
        //     if (isUPpress)
        //     {
        //         isUPpress = 0;
        //         updateTimeDay();
        //     }
        //     if (longPressTrigger)
        //     {
        //         longPressTrigger = 0;
        //         updateTimeDay();
        //     }
        //     break;

    default:
        break;
    }
    // displayLCD(bufferTime[edit_time_state], (TIME_LAB4)edit_time_state, 0);
}
void editAlarmFsm(void)
{
    if (isENpress)
    {
        isENpress = 0;

        ds3231_SetAlarm1(bufferAlarm[EDIT_ALARM_DATE],
                         bufferAlarm[EDIT_ALARM_DAY],
                         bufferAlarm[EDIT_ALARM_HOUR],
                         bufferAlarm[EDIT_ALARM_MIN],
                         bufferAlarm[EDIT_ALARM_SEC]);
        lab4_state = NORMAL;
        initNormal();
        return;
    }

    switch (edit_alarm_state)
    {
    case EDIT_ALARM_SEC:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_alarm_state = EDIT_ALARM_MIN;
            displayLCD(bufferAlarm[EDIT_ALARM_SEC], TIME_SEC, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateAlarmSec();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateAlarmSec();
        }
        if (isBACKpress)
        {
            isBACKpress = 0;
            bufferAlarm[EDIT_ALARM_SEC] = ALARM_SKIP;
        }

        /* code */
        break;
    case EDIT_ALARM_MIN:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_alarm_state = EDIT_ALARM_HOUR;
            displayLCD(bufferAlarm[EDIT_ALARM_MIN], TIME_MIN, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateAlarmMin();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateAlarmMin();
        }
        if (isBACKpress)
        {
            isBACKpress = 0;
            bufferAlarm[EDIT_ALARM_MIN] = ALARM_SKIP;
            displayLCD(bufferAlarm[EDIT_ALARM_MIN], EDIT_ALARM_MIN, 0);
        }

        break;
    case EDIT_ALARM_HOUR:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_alarm_state = EDIT_ALARM_DATE;
            displayLCD(bufferAlarm[EDIT_ALARM_HOUR], TIME_HOUR, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateAlarmHour();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateAlarmHour();
        }
        if (isBACKpress)
        {
            isBACKpress = 0;
            bufferAlarm[EDIT_ALARM_HOUR] = ALARM_SKIP;
            displayLCD(bufferAlarm[EDIT_ALARM_HOUR], EDIT_ALARM_HOUR, 0);
        }

        break;
    case EDIT_ALARM_DATE:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_alarm_state = EDIT_ALARM_DAY;
            displayLCD(bufferAlarm[EDIT_ALARM_DATE], TIME_DATE, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateAlarmDate();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateAlarmDate();
        }
        if (isBACKpress)
        {
            isBACKpress = 0;
            bufferAlarm[EDIT_ALARM_DATE] = ALARM_SKIP;
            displayLCD(bufferAlarm[EDIT_ALARM_DATE], EDIT_ALARM_DATE, 0);
        }

        break;
    case EDIT_ALARM_DAY:
        if (isOnePress)
        {
            isOnePress = 0;
            edit_alarm_state = EDIT_ALARM_SEC;
            displayLCD(bufferAlarm[EDIT_ALARM_DAY], TIME_DAY, 0);
        } // switch mode

        if (isUPpress)
        {
            isUPpress = 0;
            updateAlarmDay();
        }
        if (longPressTrigger)
        {
            longPressTrigger = 0;
            updateAlarmDay();
        }
        if (isBACKpress)
        {
            isBACKpress = 0;
            bufferAlarm[EDIT_ALARM_DAY] = ALARM_SKIP;
            displayLCD(bufferAlarm[EDIT_ALARM_DAY], EDIT_ALARM_DAY, 0);
        }

        break;
    default:
        break;
        // displayLCD(bufferAlarm[edit_alarm_state], (TIME_LAB4)edit_alarm_state, 0);
    }
}

void updateTimeSec(void)
{
    bufferTime[EDIT_TIME_SEC]++;
    if (bufferTime[EDIT_TIME_SEC] > 59)
    {
        bufferTime[EDIT_TIME_SEC] = 0;
    }
    // Hiển thị giá trị mới lên LCD
}

/**
 * @brief Tăng Phút, xoay vòng từ 59 về 0
 */
void updateTimeMin(void)
{
    bufferTime[EDIT_TIME_MIN]++;
    if (bufferTime[EDIT_TIME_MIN] > 59)
    {
        bufferTime[EDIT_TIME_MIN] = 0;
    }
}

/**
 * @brief Tăng Giờ, xoay vòng từ 23 về 0
 */
void updateTimeHour(void)
{
    bufferTime[EDIT_TIME_HOUR]++;
    if (bufferTime[EDIT_TIME_HOUR] > 23)
    {
        bufferTime[EDIT_TIME_HOUR] = 0;
    }
}

/**
 * @brief Tăng Ngày, xoay vòng từ 31 về 1
 * @note  Để đơn giản, hàm này không kiểm tra tháng
 */
void updateTimeDate(void)
{
    bufferTime[EDIT_TIME_DATE]++;
    if (bufferTime[EDIT_TIME_DATE] > 31)
    {
        bufferTime[EDIT_TIME_DATE] = 1;
    }
    else if (bufferTime[EDIT_TIME_DATE] == 0)
    {
        bufferTime[EDIT_TIME_DATE] = 1; // Đảm bảo bắt đầu từ 1
    }
}

/**
 * @brief Tăng Tháng, xoay vòng từ 12 về 1
 */
void updateTimeMonth(void)
{
    bufferTime[EDIT_TIME_MONTH]++;
    if (bufferTime[EDIT_TIME_MONTH] > 12)
    {
        bufferTime[EDIT_TIME_MONTH] = 1;
    }
    else if (bufferTime[EDIT_TIME_MONTH] <= 0)
    {
        bufferTime[EDIT_TIME_MONTH] = 1; // Đảm bảo bắt đầu từ 1
    }
}

/**
 * @brief Tăng Năm, xoay vòng từ 99 về 0
 */
void updateTimeYear(void)
{
    bufferTime[EDIT_TIME_YEAR]++;
    if (bufferTime[EDIT_TIME_YEAR] > 99)
    {
        bufferTime[EDIT_TIME_YEAR] = 0;
    }
    else if (bufferTime[EDIT_TIME_YEAR] < 0)
    {
        bufferTime[EDIT_TIME_YEAR] = 0;
    }
}

/**
 * @brief Tăng Thứ, xoay vòng từ 7 về 1 (Giả sử 1=Chủ Nhật, 7=Thứ Bảy)
 */
void updateTimeDay(void)
{
    bufferTime[EDIT_TIME_DAY]++;
    if (bufferTime[EDIT_TIME_DAY] > 7)
    {
        bufferTime[EDIT_TIME_DAY] = 1;
    }
    else if (bufferTime[EDIT_TIME_DAY] == 0)
    {
        bufferTime[EDIT_TIME_DAY] = 1; // Đảm bảo bắt đầu từ 1
    }
}

/*================================================================*/
/* CÁC HÀM UPDATE BÁO THỨC (ALARM)                        */
/*================================================================*/

/**
 * @brief Tăng Giây báo thức, xoay vòng từ 59 về 0
 */
void updateAlarmSec(void)
{
    bufferAlarm[EDIT_ALARM_SEC]++;
    if (bufferAlarm[EDIT_ALARM_SEC] > 59)
    {
        bufferAlarm[EDIT_ALARM_SEC] = 0;
    }
    // Hiển thị giá trị mới lên LCD (dùng vị trí của TIME_SEC)
}

/**
 * @brief Tăng Phút báo thức, xoay vòng từ 59 về 0
 */
void updateAlarmMin(void)
{
    bufferAlarm[EDIT_ALARM_MIN]++;
    if (bufferAlarm[EDIT_ALARM_MIN] > 59)
    {
        bufferAlarm[EDIT_ALARM_MIN] = 0;
    }
}

/**
 * @brief Tăng Giờ báo thức, xoay vòng từ 23 về 0
 */
void updateAlarmHour(void)
{
    bufferAlarm[EDIT_ALARM_HOUR]++;
    if (bufferAlarm[EDIT_ALARM_HOUR] > 23)
    {
        bufferAlarm[EDIT_ALARM_HOUR] = 0;
    }
}

/**
 * @brief Tăng Ngày báo thức, xoay vòng từ 31 về 1
 */
void updateAlarmDate(void)
{
    bufferAlarm[EDIT_ALARM_DATE]++;
    if (bufferAlarm[EDIT_ALARM_DATE] > 31)
    {
        bufferAlarm[EDIT_ALARM_DATE] = 1;
    }
    else if (bufferAlarm[EDIT_ALARM_DATE] == 0)
    {
        bufferAlarm[EDIT_ALARM_DATE] = 1;
    }
}

/**
 * @brief Tăng Thứ báo thức, xoay vòng từ 7 về 1
 */
void updateAlarmDay(void)
{
    bufferAlarm[EDIT_ALARM_DAY]++;
    if (bufferAlarm[EDIT_ALARM_DAY] > 7)
    {
        bufferAlarm[EDIT_ALARM_DAY] = 1;
    }
    else if (bufferAlarm[EDIT_ALARM_DAY] == 0)
    {
        bufferAlarm[EDIT_ALARM_DAY] = 1;
    }
}