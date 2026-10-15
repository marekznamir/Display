#include "DisplayController.hpp"
#include "Callbacks.hpp"

#include <u8g2.h>

u8g2_t u8g2;

DisplayController::DisplayController() {
    // Constructor implementation
}

void DisplayController::init() {
    #ifdef _WIN32
        // u8g2_Setup_uc1698_240x160(
        //     &u8g2,
        //     U8G2_R0,
        //     [](u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) -> uint8_t {
        //         return 1; // Dummy implementation for Windows for byte sending
        //     },
        //     [](u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) -> uint8_t {
        //         return 1; // Dummy implementation for Windows for GPIO and delay
        //     }
        // );
    #endif

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetContrast(&u8g2, 50);
    u8g2_ClearBuffer(&u8g2);
}