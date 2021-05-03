#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include </home/lis01/Atmega16/lcd_128/main.c>


#define RST	PD7
#define RS	PD6
#define RW	PD5
#define  E	PD4

#define SET_CGRAM   	0x80	// установка адреса CGRAM
#define FUNCTION_SET 	0x20
	#define RE          0x04		// 1 - набор расширенных комманд
	#define DL          0x10		// 1 - 8-ми битный режим
#define DISPLAY_STATUS	0x08
	#define B           0x01		// моргание курсора
	#define C           0x02		// включение курсора
	#define D           0x04		// включение дисплея
#define ENTRY_MODE_SET	0x04
	#define  S          0x01		// 
	#define ID          0x02		// движение курсора слева на право
#define RETURN_HOME    	0x02	// положение курсора в начало
#define CLEAR		    0x01	// очистка экрана



void write_instruction(unsigned char instruction);
void init_LCD(void);
void write_data(unsigned char data);
void busy(void);

void print_string(unsigned char pos, char *str);


void main(void)
{
	DDRA = 0xff;	PORTA = 0xff;
	DDRC = 0xff;
	DDRD |= (1 << RST) | (1 << RS) | (1 << RW) | (1 << E);
	PORTD = 0x00;
	PORTC = 0x00;

    uart_setup();

	init_LCD();
    print_string(0, "Hello");
    _delay_ms(1000);
    write_instruction(CLEAR);
    print_string(0, "World");
    _delay_ms(1000);
    write_instruction(CLEAR);

   
    char ch;
    char str[64];
    int i = 0;
    while( 1 ) {
        ch = receive();
        send(ch);
        sendStr("\n\r");
        sendStr("> ");
         
        write_data(ch);
    }
    
}


void init_LCD(void)
{
	PORTD &= ~(1 << RST);
	_delay_ms(500);
	PORTD |= (1 << RST); // вывод экрана из состояния сброса
	_delay_ms(40);
	write_instruction(FUNCTION_SET | DL);
	_delay_us(100);
	write_instruction(FUNCTION_SET | DL);
	_delay_us(37);
	write_instruction(DISPLAY_STATUS | D);
	_delay_us(100);
	write_instruction(CLEAR);
	_delay_ms(10);
	write_instruction(ENTRY_MODE_SET | ID);
	_delay_us(72);
}



void write_instruction(unsigned char instruction)
{
	PORTC = instruction;
	PORTD &= ~(1 << RS);
	PORTD |= (1 << E);
	_delay_us(5);
	PORTD &= ~(1 << E);
	_delay_us(40);
}

void write_data(unsigned char data)
{
	PORTC = data;
	PORTD |= (1 << RS);
	PORTD |= (1 << E);
	_delay_us(5);
	PORTD &= ~(1 << E);
}




void busy(void)
{
	PORTD &= ~(1 << RS);
	PORTD |= (1 << RW);
	PORTD |= (1 << E);

	while(PINC & (1 << 7))
	{
		;
	}

}

void print_string(unsigned char pos, char *str)
{
	unsigned char i = 0;
	
	write_instruction(SET_CGRAM | pos);
	_delay_ms(1);
	while(str[i] != '\0')
	{
		write_data(str[i]);
		_delay_us(30);
		i++;
	}
}

