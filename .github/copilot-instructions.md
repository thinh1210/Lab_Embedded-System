# AI Agent Instructions for Lab_Embedded-System

## Project Overview
This is an STM32F407-based embedded system project focused on traffic light control and LED/LCD interfacing. The project uses STM32CubeIDE and HAL libraries for development.

## Key Components and Architecture

### Hardware Components
- STM32F407ZGT6 microcontroller
- LCD display (via FSMC interface)
- 7-segment LED displays
- Traffic lights
- Push buttons
- Status LEDs

### Software Structure
- `/Core/Inc`: Header files defining component interfaces
- `/Core/Src`: Implementation files
- `/Drivers`: STM32F4 HAL and CMSIS drivers

### Critical Components:
1. **Timer System** (`software_timer.c/h`)
   - Manages time-based events and scheduling
   - Uses HAL timer interfaces for precise timing

2. **Display Interface** (`lcd.c/h`)
   - FSMC-based LCD controller
   - Graphics and text rendering functions
   - Example usage in `lab3.c:Background()`

3. **Traffic Light Control** (`lab3.c/h`)
   - State machine implementation
   - Multiple modes: NORMAL, MODIFY_RED, MODIFY_YELLOW, MODIFY_GREEN
   - Button-based mode switching

## Development Patterns

### State Management
```c
typedef enum {
    STATE_EW_GO,
    STATE_EW_STOP,
    // ... other states
} TrafficState;
```
Always use enum-based state machines for mode/state tracking.

### Timer Usage
Use software timer functions for non-blocking delays:
```c
setTimer1(1000);  // Set timer for 1000ms
if (timer1_flag) {
    // Handle timer expiration
}
```

### Hardware Initialization
Components require specific initialization order:
1. HAL peripherals (GPIO, FSMC, SPI)
2. Software timers
3. Display components
4. Application logic

## Project Conventions

### Coding Style
- Use `/* USER CODE BEGIN/END */` sections for custom code
- Maintain HAL-generated code sections unchanged
- Follow STM32CubeIDE project structure

### Function Organization
- Hardware initialization in `*_Init()` functions
- State handlers in separate functions
- Background/UI updates in dedicated display functions

### Error Handling
- Use HAL_Status return values for error checking
- Initialize all hardware with proper error handling
- Check peripheral status before operations

## Common Operations

### Adding New Features
1. Create header in `/Core/Inc`
2. Implement in `/Core/Src`
3. Add initialization in `main.c`
4. Update state machine if needed

### Building and Flashing
- Use STM32CubeIDE's built-in tools
- Debug configuration in `BKIT_LAB1 Debug.launch`
- Flash using ST-Link/V2 programmer

## Key Files for Reference
- `main.c`: System initialization and main loop
- `lab3.c`: Traffic light control implementation
- `lcd.c`: Display interface functions
- `software_timer.c`: Timer management system