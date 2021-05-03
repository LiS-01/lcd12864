#define F_CPU 16000000UL			/* Define CPU clock Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include defined delay header file */
#include <stdio.h>			/* Include standard i/o library file */
#include <avr/pgmspace.h>
//#include "Image.h"

#define Data_Port			PORTC	/* Define data port for GLCD */
#define Command_Port		PORTD	/* Define command port for GLCD */
#define Data_Port_Dir		DDRC	/* Define data port for GLCD */
#define Command_Port_Dir	DDRD	/* Define command port for GLCD */


#define RS			PD6	/* Define control pins */
#define RW			PD5
#define E			PD4
#define RST			PD7

//my defines
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
#define CLEAR	    	0x01	// очистка экрана

//my tests
#define GDRAM 0x80



#define TotalPage	8


// FUNCTIONS FOR DISPLAY LCD12864
void write_instruction(unsigned char instruction);
void init_LCD(void);
void write_data(unsigned char data);
void busy(void);

void GLCD_Command(char Command)		/* GLCD command function */
{
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << E);	/* Make HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << E);
	_delay_us(5);
}

void GLCD_Data(char Data)		/* GLCD data function */
{
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << E);	/* Make HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << E);
	_delay_us(5);
}

void GLCD_Init()			/* GLCD initialize function */
{
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	Command_Port |=  (1 << RS);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0x00);		/* Set Y address (column=0) */
	GLCD_Command(0x80);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
}

void GLCD_ClearAll()			/* GLCD all display clear function */
{
	int i,j;
	for(i = 0; i < TotalPage; i++)
	{
		GLCD_Command((0xB8) + i);/* Increment page */
		for(j = 0; j < 128; j++)
		{
			GLCD_Data(0);	/* Write zeros to all 64 column */
		}
	}
	GLCD_Command(0x00);		/* Set Y address (column=0) */
	GLCD_Command(0x80);		/* Set x address (page=0) */
}



////////////////////////////////////////////////////////////
void GLCD_String(const int* image)	/* GLCD string write function */
{
	int column,page,page_add=0x80,k=0;
	float page_inc=1;
	int byte;

	//Command_Port |= (1 << CS1);	/* Select Left half of display */
	//Command_Port &= ~(1 << CS2);

    //STOP STOP STOP
    //
	//for(page=0;page<16;page++)	/* Print pages(8 page of each half)*/
	//{
		for(column=0;column<1024;column++)
		{
			//byte = pgm_read_byte(&image[k+column]);
            byte = &image[column];
			GLCD_Data(byte);/* Print 64 column of each page */
		}
		//Command_Port ^= (1 << CS1);/* Change segment controller */
		//Command_Port ^= (1 << CS2);
		//GLCD_Command((page_add+page_inc));/* Increment page address*/
		//page_inc=page_inc+1;
		//k=k+128;			/* Increment pointer */
	//}
	GLCD_Command(0x00);		/* Set Y address (column=0) */
	GLCD_Command(0x80);		/* Set x address (page=0) */
}

///////////////////////////////////////////////////////////

void GLCD_String_mine() /* GLCD string write function */
 {
     int column,page,page_add=0x80,k=0;
     float page_inc=1;
     int byte;
 
     //Command_Port |= (1 << CS1);   /* Select Left half of display */
     //Command_Port &= ~(1 << CS2);
 
     //STOP STOP STOP
     //
     //for(page=0;page<16;page++)    /* Print pages(8 page of each half)*/
     //{
         for(column=0;column<512;column++)
         {
             //byte = pgm_read_byte(&image[k+column]);
             byte = 0x0000;
             GLCD_Data(byte);/* Print 64 column of each page */
         }
         //Command_Port ^= (1 << CS1);/* Change segment controller */
         //Command_Port ^= (1 << CS2);
         //GLCD_Command((page_add+page_inc));/* Increment page address*/
         //page_inc=page_inc+1;
         //k=k+128;          /* Increment pointer */
     //}
     GLCD_Command(0x02);     /* Set Y address (column=0) */
     GLCD_Command(0x80);     /* Set x address (page=0) */
 }



//////////////////////////////////////////////////////////

void GLCD_String_3()
{
    int i,j;
    int byte;

    for(i=0; i<32; i++)
    {

        for(j=0; j<16; j++)
        {
            byte = 0xff00;
            GLCD_Data(byte);
        }

        _delay_ms(2000);

        GLCD_Command(0x00);
        GLCD_Command(0x80);

    }

}



//////////////////////////////////////////////////////////
int main(void)
{	
    const int img[512]={0xffff};

	GLCD_Init();    /* Initialize GLCD */
	GLCD_ClearAll();		/* Clear all GLCD display */

	/*write_instruction(GDRAM);*/
    //GLCD_Command(GDRAM);

	//GLCD_String(img);		/* Print Image Array */
    _delay_ms(1000);
    GLCD_String_mine();
    _delay_ms(1000);
    GLCD_String_3();
    _delay_ms(1000);
	while(1);

}


//FUNCTIONS FOR LCD
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
