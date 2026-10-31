#include "DisplayController.hpp"
#include "Callbacks.hpp"
#include <u8g2.h>

u8g2_t u8g2;

#ifdef _WIN32
#include "Terminal/Terminal.hpp"
namespace cmd = Command;
cmd::Terminal terminal0;
#endif

DisplayController::DisplayController() 
{
    // Constructor implementation
}

DisplayController::~DisplayController()
{
    
}

void DisplayController::init() 
{
    setScreen<TerminalScreen>(terminal0);

    #ifdef _WIN32
        u8g2_Setup_uc1698_240x160_f(
            &u8g2,
            U8G2_R0,
            [](u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) -> uint8_t {
                return 1; // Dummy implementation for Windows for byte sending
            },
            [](u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) -> uint8_t {
                return 1; // Dummy implementation for Windows for GPIO and delay
            }
        );
    #endif

    #ifdef STM32H7A3xx
        u8g2_Setup_uc1698_240x160_f(
            &u8g2,
            U8G2_R0,
            u8x8_byte_4wire_hw_spi,
            u8x8_gpio_and_delay_stm32
        );

    #endif

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetContrast(&u8g2, 50);
    u8g2_ClearBuffer(&u8g2);
}

void DisplayController::draw() 
{
    std::visit([](auto&& screen){
        screen.draw();
    }, currentScreen);
}

template<typename Screen, typename... Args>
void DisplayController::setScreen(Args&&... args)
{
    currentScreen.emplace<Screen>(std::forward<Args>(args)...);
}