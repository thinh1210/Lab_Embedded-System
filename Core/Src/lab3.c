/*
 * lab3.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Divu
 */

#include <../Inc/lab3.h>

char buffer[100];
int numberLight1 = 0;
int numberLight2 = 0;
int red_time = RED_TIME_LAB3;
int green_time = GREEN_TIME_LAB3;
int yellow_time = YELLOW_TIME_LAB3;

TrafficState currentState = STATE_EW_GO;
MODE mode = NORMAL;
bool button_trigger = false;
bool timer_trigger = false;
bool isChangemode = false;
bool isSave = false;
bool isUp = false;
uint8_t ledState = 0;
uint8_t led = 0;
void Background(void)
{
    lcd_fill(0, 0, 240, 320, BLACK);

    //  lcd_show_string_center(0, 2, "Hello World !!!", RED, BLACK, 16, 1);
    //  lcd_show_string(20, 30, "Test LCD Screen", WHITE, BLACK, 32, 0);
    lcd_show_string(5, 5, "MODE", WHITE, BLACK, 16, 1);
    lcd_show_string(50, 5, NORMAL_MODE, WHITE, BLACK, 16, 1);

    lcd_show_string(0, 40, "LIGHT 1", RED, BLACK, 16, 1);
    lcd_draw_circle(40, 80, GREEN, 10, 1);
    lcd_draw_circle(80, 80, RED, 10, 1);
    lcd_draw_circle(120, 80, YELLOW, 10, 1);
    lcd_show_int_num(180, 80, numberLight1, 2, BLUE, BLACK, 32);

    lcd_show_string(0, 120, "LIGHT 2", RED, BLACK, 16, 1);
    lcd_draw_circle(40, 160, GREEN, 10, 1);
    lcd_draw_circle(80, 160, RED, 10, 1);
    lcd_draw_circle(120, 160, YELLOW, 10, 1);
    lcd_show_int_num(180, 160, numberLight2, 2, BLUE, BLACK, 32);
}

void initlab3(void)
{
    timer2Init();
    lcd_init();
    buttonInit();
    timerInit(0, 50, 50, buttonScan);
    timerInit(1, 100, 100, fsmlab3);
    timerInit(2, 1000, 1000, updatetimer);
    timerInit(3, 250, 250, taskBlinkLed);
    disableTask(3);
    Background();
    mode = NORMAL;
    ledState = 0;

    currentState = STATE_EW_GO;
    // SỬA Ở ĐÂY:
    // Light 1 (NS) đang ĐỎ, nên phải đếm red_time
    numberLight1 = red_time;
    // Light 2 (EW) đang XANH, nên phải đếm green_time
    numberLight2 = green_time;
    EWGo();
}
void runlab3(void)
{
    doTask();
}

void fsmlab3(void)
{
    isChangemode = buttonGetState(BUTTON_1);
    isSave = buttonGetState(BUTTON_2);
    isUp = buttonGetState(BUTTON_UP);
    button_trigger = isChangemode || isSave || isUp;
    if (button_trigger == 1)
    {
        button_trigger = 0;
        if (isChangemode)
        {

            isChangemode = 0;
            lcd_show_string(50, 5, DELETE_FILL, WHITE, BLACK, 16, 0);
            updateMode();
        }
        if (isSave)
        {
            isSave = 0;
            lcd_show_string(50, 5, DELETE_FILL, WHITE, BLACK, 16, 0);
            saveMode();
                }
        if (isUp)
        {
            isUp = 0;
            upTime();
        }
    }

    if (mode == NORMAL && timer_trigger == 1)
    {
        switchTLToNextState();
        timer_trigger = false;
    }
    updateDisplayNumbers();
}

void updateMode()
{
    switch (mode)
    {
    case SET_GREEN: // green
        /* code */
        mode = SET_RED;
        lcd_show_string(50, 5, SETTING_RED, WHITE, BLACK, 16, 1);
        ledState = 1;
        led = 2;
        numberLight1 = red_time;
        numberLight2 = red_time;
        clearLed();
        enableTask(3);
        break;
        // GREEN to RED
    case SET_RED: // red
        /* code */
        mode = SET_YELLOW;
        lcd_show_string(50, 5, SETTING_YELLOW, WHITE, BLACK, 16, 1);
        ledState = 1;
        led = 3;
        numberLight1 = yellow_time;
        numberLight2 = yellow_time;
        clearLed();
        enableTask(3);
        break;
        // RED to YELLOW
    case SET_YELLOW: // yellow
        /* code */
        // YELLOW to NORMAL
        mode = NORMAL;
        lcd_show_string(50, 5, NORMAL_MODE, WHITE, BLACK, 16, 1);
        comeBackTask();
        break;
    case NORMAL:
        // NORMAL to GREEN
        mode = SET_GREEN;
        lcd_show_string(50, 5, SETTING_GREEN, WHITE, BLACK, 16, 1);
        ledState = 1;
        led = 1;
        numberLight1 = green_time;
        numberLight2 = green_time;
        clearLed();
        enableTask(3);
        disableTask(2);
        break;

    default:
        break;
    }
}

void saveMode(void)
{
    if (mode == NORMAL)
        return;

    switch (mode)
    {
    case SET_GREEN: // green
        /* SỬA Ở ĐÂY */
        green_time = numberLight1;

        red_time = green_time + yellow_time;
        break;

    case SET_RED: // red

        red_time = numberLight1;
        green_time = red_time * 0.7;
        yellow_time = red_time - green_time;
        break;

    case SET_YELLOW: // yellow
        /* SỬA Ở ĐÂY */
        yellow_time = numberLight1;
        // Tự động cập nhật lại red_time
        red_time = green_time + yellow_time;
        break;
    default:
        break;
    }
    lcd_show_string(50, 5, NORMAL_MODE, WHITE, BLACK, 16, 1);
    comeBackTask();
    mode = NORMAL;
}

void upTime(void)
{
    if (mode == NORMAL)
        return;
    numberLight1++;
    numberLight2++;
}

void clearLed()
{
    lcd_draw_circle(40, 80, BLACK, 10, 1);
    lcd_draw_circle(40, 160, BLACK, 10, 1); // BLACK is background
    lcd_draw_circle(80, 80, BLACK, 10, 1);
    lcd_draw_circle(80, 160, BLACK, 10, 1);
    lcd_draw_circle(120, 80, BLACK, 10, 1);
    lcd_draw_circle(120, 160, BLACK, 10, 1);
}
void taskBlinkLed()
{
    switch (led)
    {
    case 1: // green
        if (ledState)
        {
            lcd_draw_circle(40, 80, GREEN, 10, 1);
            lcd_draw_circle(40, 160, GREEN, 10, 1);
        }
        else
        {
            lcd_draw_circle(40, 80, BLACK, 10, 1);
            lcd_draw_circle(40, 160, BLACK, 10, 1);
        }

        break;
    case 2: // red
        if (ledState)
        {
            lcd_draw_circle(80, 80, RED, 10, 1);
            lcd_draw_circle(80, 160, RED, 10, 1);
        }
        else
        {
            lcd_draw_circle(80, 80, BLACK, 10, 1);
            lcd_draw_circle(80, 160, BLACK, 10, 1);
        }
        break;
    case 3: // yellow
        if (ledState)
        {
            lcd_draw_circle(120, 80, YELLOW, 10, 1);
            lcd_draw_circle(120, 160, YELLOW, 10, 1);
        }
        else
        {
            lcd_draw_circle(120, 80, BLACK, 10, 1);
            lcd_draw_circle(120, 160, BLACK, 10, 1);
        }
        break;
    default:
        break;
    }
    ledState = 1 - ledState;
}

void updatetimer(void)
{
    numberLight1--;
    numberLight2--;
    if (numberLight1 == 0 || numberLight2 == 0)
    {
        timer_trigger = true;
    }
}

void comeBackTask(void)
{
    disableTask(3);
    enableTask(2);
    switch (currentState)
    {
    case STATE_EW_GO:

        currentState = STATE_EW_GO;
        EWGo(); // Light 1 (NS) = ĐỎ, Light 2 (EW) = XANH

        // SỬA Ở ĐÂY:
        numberLight1 = red_time;   // Light 1 (NS) đếm ngược thời gian ĐỎ
        numberLight2 = green_time; // Light 2 (EW) đếm ngược thời gian XANH
        break;
    case STATE_EW_YELLOW:
        // Trạng thái hiện tại: Light 1 (NS) = ĐỎ, Light 2 (EW) = XANH
        // Hết thời gian XANH của Light 2 -> Chuyển Light 2 sang VÀNG
        currentState = STATE_EW_YELLOW;
        EWYellow();                 // Light 1 (NS) = ĐỎ, Light 2 (EW) = VÀNG
        numberLight2 = yellow_time; // Light 2 đếm ngược thời gian VÀNG
        // Light 1 vẫn tiếp tục đếm thời gian ĐỎ
        break;

    case STATE_NS_GO:

        // Trạng thái hiện tại: Light 1 (NS) = ĐỎ, Light 2 (EW) = VÀNG
        // Hết thời gian VÀNG của Light 2 -> Chuyển Light 1 sang XANH, Light 2 sang ĐỎ
        currentState = STATE_NS_GO;
        NSGo(); // Light 1 (NS) = XANH, Light 2 (EW) = ĐỎ

        // SỬA Ở ĐÂY:
        numberLight1 = green_time; // Light 1 (NS) đếm ngược thời gian XANH
        numberLight2 = red_time;   // Light 2 (EW) đếm ngược thời gian ĐỎ
        break;

    case STATE_NS_YELLOW:
        // Trạng thái hiện tại: Light 1 (NS) = XANH, Light 2 (EW) = ĐỎ
        // Hết thời gian XANH của Light 1 -> Chuyển Light 1 sang VÀNG
        currentState = STATE_NS_YELLOW;
        NSYellow();                 // Light 1 (NS) = VÀNG, Light 2 (EW) = ĐỎ
        numberLight1 = yellow_time; // Light 1 đếm ngược thời gian VÀNG
        // Light 2 vẫn tiếp tục đếm thời gian ĐỎ
        break;
    }
}

void updateDisplayNumbers(void)
{
    lcd_show_int_num(180, 80, numberLight1, 2, BLUE, BLACK, 32);  // Light 1
    lcd_show_int_num(180, 160, numberLight2, 2, BLUE, BLACK, 32); // Light 2
}
void switchTLToNextState(void)
{
    switch (currentState)
    {
    case STATE_EW_GO:
        // Trạng thái hiện tại: Light 1 (NS) = ĐỎ, Light 2 (EW) = XANH
        // Hết thời gian XANH của Light 2 -> Chuyển Light 2 sang VÀNG
        currentState = STATE_EW_YELLOW;
        EWYellow();                 // Light 1 (NS) = ĐỎ, Light 2 (EW) = VÀNG
        numberLight2 = yellow_time; // Light 2 đếm ngược thời gian VÀNG
        // Light 1 vẫn tiếp tục đếm thời gian ĐỎ
        break;

    case STATE_EW_YELLOW:
        // Trạng thái hiện tại: Light 1 (NS) = ĐỎ, Light 2 (EW) = VÀNG
        // Hết thời gian VÀNG của Light 2 -> Chuyển Light 1 sang XANH, Light 2 sang ĐỎ
        currentState = STATE_NS_GO;
        NSGo(); // Light 1 (NS) = XANH, Light 2 (EW) = ĐỎ

        // SỬA Ở ĐÂY:
        numberLight1 = green_time; // Light 1 (NS) đếm ngược thời gian XANH
        numberLight2 = red_time;   // Light 2 (EW) đếm ngược thời gian ĐỎ
        break;

    case STATE_NS_GO:
        // Trạng thái hiện tại: Light 1 (NS) = XANH, Light 2 (EW) = ĐỎ
        // Hết thời gian XANH của Light 1 -> Chuyển Light 1 sang VÀNG
        currentState = STATE_NS_YELLOW;
        NSYellow();                 // Light 1 (NS) = VÀNG, Light 2 (EW) = ĐỎ
        numberLight1 = yellow_time; // Light 1 đếm ngược thời gian VÀNG
        // Light 2 vẫn tiếp tục đếm thời gian ĐỎ
        break;

    case STATE_NS_YELLOW:
        // Trạng thái hiện tại: Light 1 (NS) = VÀNG, Light 2 (EW) = ĐỎ
        // Hết thời gian VÀNG của Light 1 -> Chuyển Light 1 sang ĐỎ, Light 2 sang XANH
        currentState = STATE_EW_GO;
        EWGo(); // Light 1 (NS) = ĐỎ, Light 2 (EW) = XANH

        // SỬA Ở ĐÂY:
        numberLight1 = red_time;   // Light 1 (NS) đếm ngược thời gian ĐỎ
        numberLight2 = green_time; // Light 2 (EW) đếm ngược thời gian XANH
        break;
    }
}

// GREEN RED YELLOW
void NSGo(void)
{
    lcd_draw_circle(40, 80, GREEN, 10, 1);
    lcd_draw_circle(80, 80, BLACK, 10, 1);
    lcd_draw_circle(120, 80, BLACK, 10, 1);

    lcd_draw_circle(40, 160, BLACK, 10, 1);
    lcd_draw_circle(80, 160, RED, 10, 1);
    lcd_draw_circle(120, 160, BLACK, 10, 1);
}

void NSYellow(void)
{
    // North-South: TẮT | VÀNG | TẮT
    lcd_draw_circle(40, 80, BLACK, 10, 1);   // Tắt
    lcd_draw_circle(80, 80, BLACK, 10, 1);   // Vàng
    lcd_draw_circle(120, 80, YELLOW, 10, 1); // Tắt

    // East-West:  TẮT | TẮT | ĐỎ
    lcd_draw_circle(40, 160, BLACK, 10, 1);  // Tắt
    lcd_draw_circle(80, 160, RED, 10, 1);    // Tắt
    lcd_draw_circle(120, 160, BLACK, 10, 1); // Đỏ
}

void EWGo(void)
{
    // North-South: TẮT | TẮT | ĐỎ
    lcd_draw_circle(40, 80, BLACK, 10, 1);  // Tắt
    lcd_draw_circle(80, 80, RED, 10, 1);    // Tắt
    lcd_draw_circle(120, 80, BLACK, 10, 1); // Đỏ

    // East-West:  XANH | TẮT | TẮT
    lcd_draw_circle(40, 160, GREEN, 10, 1);  // Xanh
    lcd_draw_circle(80, 160, BLACK, 10, 1);  // Tắt
    lcd_draw_circle(120, 160, BLACK, 10, 1); // Tắt
}

void EWYellow(void)
{
    // North-South: TẮT | TẮT | ĐỎ
    lcd_draw_circle(40, 80, BLACK, 10, 1);  // Tắt
    lcd_draw_circle(80, 80, RED, 10, 1);    // Tắt
    lcd_draw_circle(120, 80, BLACK, 10, 1); // Đỏ

    // East-West:  TẮT | VÀNG | TẮT
    lcd_draw_circle(40, 160, BLACK, 10, 1);   // Tắt
    lcd_draw_circle(80, 160, BLACK, 10, 1);   // Vàng
    lcd_draw_circle(120, 160, YELLOW, 10, 1); // Tắt
}
