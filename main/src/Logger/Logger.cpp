#include "Logger.h"



LiquidCrystal display(D7, D6, D5, D3, D2, D1);

// função para loggar mensagens no Monitor Serial e no Display.
// chama internamente logger_serial_print ou logger_display_print.
void logger_print(const char* str){
    Serial.println(str);
    display.println(str);
}

// função para loggar mensagens no Monitor Serial.
void logger_serial_print(const char* str){}

// função para loggar mensagens no Display.
void logger_display_print(const char* str){}