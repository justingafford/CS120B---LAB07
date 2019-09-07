## Exercises

1. Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive the LCD display, initially displaying 0. Pressing PA0 increments the display (stopping at 9). Pressing PA1 decrements the display (stopping at 0). If both buttons are depressed (even if not initially simultaneously), the display resets to 0. If a button is held, then the display continues to increment (or decrement) at a rate of once per second. Use a synchronous state machine captured in C.

Note: The LCD display displays ASCII values so 9 is not the same as ‘9’. For
displaying numbers 0 thru 9, a quick conversion technique is to add the
character ‘0’ to the number: 

			LCD_WriteData( 9 ); // will display nothing on the LCD
			LCD_WriteData( 9 + ‘0’ ); // will display 9 on the LCD

2.(Challenge) Extend the earlier light game to maintain a score on the LCD display. The initial score is 5. Each time the user presses the button at the right time, the score increments. Each time the user fails, the score decrements. When reaching 9, show victory somehow. 

## Google Doc Link: 
https://docs.google.com/document/d/1JclkRYstx2RJlfOtnWH54n4izDS_emJbsdcthNEFl40/edit?usp=sharing
