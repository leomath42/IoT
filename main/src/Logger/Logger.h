#ifndef LOGGER_H
#define LOGGER_H

#include <LiquidCrystal.h>
#include <HardwareSerial.h>
//#include "../Pinout.h"
#include <pins_arduino.h>

#define LOGGER_LIMIT_SIZE 32

// função para loggar mensagens no Monitor Serial e no Display.
// chama internamente logger_serial_print ou logger_display_print.
void logger_print(const char* str);

void display_setup();

// função para loggar mensagens no Monitor Serial.
void logger_serial_print(const char* str);

// função para loggar mensagens no Display.
void logger_display_print(const char* str);

#endif LOGGER_H
