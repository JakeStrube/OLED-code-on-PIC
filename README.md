## SSD1306 Library for PIC18F47Q10

This is a library driver for the PIC18F47Q10 for use with the ssd1306 128x64 OLED display.

This has been tested on the Teyleten 0.96 inch LCD OLED Display.

### Setup
The setup for using the library is start by adding I2C_host to MCC and setting the sped to 400KHz.
After generating the MCC go under the projects tab and select your project. Right click on the headers folder and go new
xc8_header.h name it ssd1306 and create new header. You can then copy in the code from ss1306.h file into that headerfile.
Then do the same but for ssd1306font.h. Next right click on source files and go new then choose C main file and name it ssd1306 and copy in the ssd1306.c code.


### Example

 ```
  /*

  This is example code of how to change the screen using a button.
  In the example the buttons are default low and setting them high
  activates the screen.

  */

  #include <stdio.h>
  #include "mcc_generated_files/system/system.h"
  #include "ssd1306.h"


  int main(void)
  {
      SYSTEM_Initialize();

      ssd1306_init(0x3C);

      ssd1306_clear(); // Used to get rid of the noise when starting up the OLED screen


      while(1)
      {

          // Activate the OLED screen on the press of button 1
          if (IO_RE2_GetValue() == 1)
          {
              ssd1306_clear(); // Clears screen
              ssd1306_string_pos(0, 0, "This is a test"); // Text in the top left corner 
              ssd1306_string_pos(10, 2, "10, 2"); // (X,Y, "String") is the setup
              ssd1306_string_pos(6, 7, "6, 7");
          }

  	// Activate the OLED screen on the press of button 2
          if(IO_RA0_GetValue()==1)
          {
              ssd1306_clear(); // Clears the screen
              ssd1306_string_pos(0,0,"Test screen 2");
              ssd1306_string_pos(123,7,"!"); // Furthest it can go without moving to the top again
            
            
              //Invert screen every second example
              //ssd1306_invert(1);
              //__delay_ms(1000);
              //ssd1306_invert(0);
          }
     
      }

  }

```
### Some Issues
Some problems with this library is that the scaling does not work so the text is stuck on one size, as well as changing the font does not work at the moment.

