#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/garbage.h"
#include "images/startScreen.h"
#include "images/gameOverScreen.h"
#include "images/apple.h"
#include "images/snake2.h"
#include "images/snake1.h"
#include "images/excited.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  CHARACTER,
  PLAY,
  END,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;
  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      state = START;
    }
    waitForVBlank();
    
    int time = vBlankCounter / 60;
    int col = 70;
    switch (state) {
  
      case START:
        a.score = '0';
        a.height = APPLE_HEIGHT;
        a.width = APPLE_WIDTH;
        s.height = 37;
        s.width = 50;
        s.row = 20;
        s.col = 20;
        a.row = 20;
        a.col = 100;
        
        //disp start screen
        drawFullScreenImageDMA(startScreen);
        undrawImageDMA(120, col + 30, GARBAGE_WIDTH, GARBAGE_HEIGHT, startScreen);
        drawImageDMA(120, col, EXCITED_WIDTH, EXCITED_HEIGHT, excited);
        if (time % 2 == 0) {
            undrawImageDMA(120, col, GARBAGE_WIDTH, GARBAGE_HEIGHT, startScreen);
            drawImageDMA(120, col + 40, EXCITED_WIDTH, EXCITED_HEIGHT, excited);
        }
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = CHARACTER; //change to CHARACTER state
          break;
        }
        state = START;
        break;
      
      case CHARACTER:
        fillScreenDMA(BLUE); //fillScreen with blue
        drawImageDMA(20, 90, SNAKE1_WIDTH, SNAKE1_HEIGHT, snake1); //draw snake1 image
        drawImageDMA(70, 90, SNAKE2_WIDTH, SNAKE2_HEIGHT, snake2); //draw snake2 image
      
        //up arrow
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
          p.image = snake1;
          state = PLAY;
        }
        //down arrow
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
          p.image = snake2;
          state = PLAY;
        }
        drawString(130, 70, "CHOOSE PLAYER: ", BLACK); 
        break;

      case PLAY:
        fillScreenDMA(YELLOW); //fillScreen with yellow
        drawImageDMA(a.row, a.col, a.width, a.height, apple); //draw apple image
        drawImageDMA(s.row, s.col, s.width, s.height, p.image); //draw player

        //up arrow
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons) && (s.row > 10)) {
          s.row = s.row - 5;
        }

        //down arrow
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons) && (s.row + s.height < 150)) {
          s.row = s.row + 5;
        }

        //right arrow
        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons) && (s.col + s.width < 230)) {
          s.col = s.col + 5;
        }

        ///left arrow
        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons) && (s.col > 10)) {
          s.col = s.col - 5;
        }

        if((a.row == s.row) && (a.col == s.col)) {
          a.score += 1;
          if(a.score == '1') {
            a.row = 100;
            a.col = 80;
          } else if(a.score == '2') {
            a.row = 50;
            a.col = 30;
          } else if(a.score == '3') {
            a.row = 40;
            a.col = 80;
          } else if(a.score == '4') {
            a.row = 100;
            a.col = 70;
          } else if(a.score == '5') {
            state = END;
            break;
          }
        }
        drawString(150, 0, "SCORE: ", BLUE); //draw score label
        drawString(150, 40, &a.score, GREEN); //draw score
        state = PLAY;
        break;

      case END:
        drawFullScreenImageDMA(gameOverScreen);
        if(KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}
