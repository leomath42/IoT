#include "Logger.h"
#include <string.h>

LiquidCrystal display(D7, D6, D5, D3, D2, D1);


// função para loggar mensagens no Monitor Serial e no Display.
// chama internamente logger_serial_print ou logger_display_print.
char* teste = "TESTE";

void display_setup()
{
    display.begin(16, 2);
}

void logger_print(const char* str){

    if(str != NULL)
    {
        Serial.println("TESTING::::");
        // Serial.println(str);
        size_t size = 0 ; //strlen(str);    

        display.clear();

        if(size > 16)
        {   
            Serial.println(size);

            char* temp = NULL;
            char* temp2 = NULL;

            for(int i = 0; i<16; i++)
            {
                *(temp+1) = *(str+1);
            }

            display.setCursor(0, 1);
            display.println(temp);
            Serial.println(temp);

            for(int i = 16; i<size; i++)
            {
                *(temp2+1) = *(str+1);
            }

            display.setCursor(0, 2);
            display.println(temp2);
            Serial.println(temp2);

        }
        else {
            Serial.println("False");
            Serial.println(size);
            display.setCursor(0, 1);
            display.println(str);
        }
    }

}

// função para loggar mensagens no Monitor Serial.
void logger_serial_print(const char* str){}

// função para loggar mensagens no Display.
void logger_display_print(const char* str){}