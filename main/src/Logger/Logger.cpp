#include "Logger.h"
#include <string.h>

LiquidCrystal display(D7, D6, D5, D3, D2, D1);


// função para loggar mensagens no Monitor Serial e no Display.
// chama internamente logger_serial_print ou logger_display_print.

void display_setup()
{
    display.begin(16, 2);
}

// log informations to serial and display(16x2)
// this break lines at display if *str* causes an overflow (line size is 16).
void logger_print(const char* str){

    if(str != NULL)
    {
        size_t size = strlen(str);    

        display.clear();

        if(size > 16)
        {   
            char temp[17];
            char temp2[17];
            
            // Line 1
            int i = 0;
            for(i; i<16; i++)
            {
                *(temp+i) = *(str+i);
            }
            // NULL Byte
            *(temp+i) = '\0';

            Serial.println(temp);
            display.setCursor(0, 0);
            display.print(temp);

            // Line 2
            int j = 0;
            for(j; j+i<size; j++)
            {
                *(temp2+j) = *(str+j+i);
            }
            // NULL Byte
            *(temp2+j) = '\0';

            Serial.println(temp2);
            display.setCursor(0, 1);
            display.print(temp2);
        }
        else {
            Serial.println(size);
            display.setCursor(0, 0);
            display.println(str);
        }
    }

}

// função para loggar mensagens no Monitor Serial.
void logger_serial_print(const char* str){}

// função para loggar mensagens no Display.
void logger_display_print(const char* str){}