  //LEDs
  #include <FastLED.h>
  #define NUM_LEDS    144
  #define LED_PIN     13
  long unsigned int prevTime = 0;
  int resetDelay = 250;
  CRGB leds[NUM_LEDS];
  
  //Buttons
  const char UP = 12;
  const char DOWN = 11;
  const char LEFT = 10;
  const char RIGHT = 9;
  const char PLACE = 8;
  
  // Directions
  #define DIR_UP    0
  #define DIR_DOWN  1
  #define DIR_LEFT  2
  #define DIR_RIGHT 3
  
  // Positions
  int x_pos = 0;
  int y_pos = 0;
  
  // Gameboard
  const char PIECE_X = 'x';
  const char PIECE_O = 'o';
  const char PIECE_EMPTY = '-';
  const int NUM_SHIPS = 4;
  int game1[9][8]; // player 2 shots
  int game2[9][8]; // player 1 shots
  bool winner;
  char who;
  int ships[4][2] = { {2, 1}, {1, 2}, {3, 1}, {1, 3} };
  
  
  // LED grid mappings 
  int board_side_1[9][8] = { {0, 1, 2, 3, 4, 5, 6, 7},
                             {15, 14, 13, 12, 11, 10, 9, 8},
                             {16, 17, 18, 19, 20, 21, 22, 23},
                             {31, 30, 29, 28, 27, 26, 25, 24},
                             {32, 33, 34, 35, 36, 37, 38, 39},
                             {47, 46, 45, 44, 43, 42, 41, 40},
                             {48, 49, 50, 51, 52, 53, 54, 55},
                             {63, 62, 61, 60, 59, 58, 57, 56},
                             {64, 65, 66, 67, 68, 69, 70, 71} };

  int board_side_2[9][8] = { {136, 137, 138, 139, 140, 141, 142, 143},
                             {135, 134, 133, 132, 131, 130, 129, 128},
                             {120, 121, 122, 123, 124, 125, 126, 127},
                             {119, 118, 117, 116, 115, 114, 113, 112},
                             {104, 105, 106, 107, 108, 109, 110, 111},
                             {103, 102, 101, 100, 99, 98, 97, 96},
                             {88, 89, 90, 91, 92, 93, 94, 95},
                             {87, 86, 85, 84, 83, 82, 81, 80},
                             {72, 73, 74, 75, 76, 77, 78, 79} };
  
  CRGB hitColor = CRGB(171, 35, 40); // dark red
  CRGB shipColor = CRGB(255, 215, 0);  // gold
  CRGB waterColor = CRGB(0, 0, 255);  // blue
  CRGB deadColor = CRGB(12, 4, 4); // black-ish
  CRGB missColor = CRGB(255, 255, 255); // white
  CRGB eColor = CRGB(0, 0, 0);        // clear/empty
  
  void setup() {
      // reset positions
      x_pos = 0;
      y_pos = 0;
      winner = false;
      who = PIECE_X;
  
      // setup buttons
      pinMode(UP, INPUT_PULLUP);
      pinMode(DOWN, INPUT_PULLUP);
      pinMode(LEFT, INPUT_PULLUP);
      pinMode(RIGHT, INPUT_PULLUP);
      pinMode(PLACE, INPUT_PULLUP);
  
      // setup ws2812b leds
      FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
      // reset led board to be empty
      for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 8; j++) {
              leds[board_side_1[i][j]] = eColor;
              leds[board_side_2[i][j]] = eColor;
          }
      }
  
      // reset game to be empty
      for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 8; j++) {
              game1[i][j] = PIECE_EMPTY;
              game2[i][j] = PIECE_EMPTY;
          }
      }

      // player 1 places ships
      for (int i = 0; i < NUM_SHIPS; i++) {
        int dimx = ships[i][0];
        int dimy = ships[i][1];
        cursor(dimx, dimy);
      }

      // player 2 places ships
      who = opposite(who);
      for (int i = 0; i < NUM_SHIPS; i++) {
        int dimx = ships[i][0];
        int dimy = ships[i][1];
        cursor(dimx, dimy);
      }
  }
  
  char opposite(char piece) {
  // Return opposite of PIECE
      if (piece == PIECE_X) {
          return PIECE_O;
      }
      return PIECE_X;
  }
  
  void place(int dimx, int dimy, int x, int y) {
  // Place WHO ship at (x, y) with dimensions (dimx, dimy), assume placeable
    if (who == PIECE_X)
    {
      for (int i = x; i < x + dimx; i++) {
        for (int j = y; j < y + dimy; j++) {
          board_side_1[i][j] = shipColor;
        }
      }
    } else {
      for (int i = x; i < x + dimx; i++) {
        for (int j = y; j < y + dimy; j++) {
          board_side_2[i][j] = shipColor;
        }
      }
    }
    FastLED.show();
  }

  bool placeable(int dimx, int dimy, int x, int y) {
  // Place WHO ship at (x, y) with dimensions (dimx, dimy), assume placeable
    if (who == PIECE_X)
    {
      for (int i = x; i < x + dimx; i++) {
        for (int j = y; j < y + dimy; j++) {
          if ((i < 0 || i >= 9 || j < 0 || j >= 8) || leds[board_side_1[i][j]] != eColor) {
            return false; 
          }
        }
      }
      return true;
    } else {
      for (int i = x; i < x + dimx; i++) {
        for (int j = y; j < y + dimy; j++) {
          if ((i < 0 || i >= 9 || j < 0 || j >= 8) || leds[board_side_2[i][j]] != eColor) {
            return false;
          }
        }
      }
      return true;
    }
  }

  void winnerblink() {
    // modify to blink only winning squares later
    
  }
  
  void cursor(int x, int y) {
    bool placed = false;
    while (!placed) {
        cursorblink(x, y);
        if (digitalRead(LEFT) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = max(y_pos-1, 0);
            }
            prevTime = currTime;
        }
        else if (digitalRead(RIGHT) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                y_pos = min(y_pos+1, 8);
            }
            prevTime = currTime;
        }
        else if (digitalRead(DOWN) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = min(x_pos+1, 9);
            }
            prevTime = currTime;
        }
        else if (digitalRead(UP) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                x_pos = min(x_pos-1, 0);
            }
            prevTime = currTime;
        }
        else if (digitalRead(PLACE) == LOW) {
            long unsigned int currTime = millis();
            if (currTime - prevTime > 10) {
                place(x, y, x_pos, y_pos);
            }
            placed = true;
            prevTime = currTime;
        }
    }
    return;
  }

  
  void cursorblink(int dimx, int dimy) {
      // blink for placement of ship
      if (who == PIECE_X && placeable(dimx, dimy, x_pos, y_pos)) {
        for (int i = x_pos; i < x_pos + dimx; i++) {
          for (int j = y_pos; j < y_pos + dimy; j++) {
            leds[board_side_1[i][j]] = shipColor;
          }
        }
        FastLED.show();
        delay(500);
        for (int i = x_pos; i < x_pos + dimx; i++) {
          for (int j = y_pos; j < y_pos + dimy; j++) {
            leds[board_side_1[i][j]] = waterColor;
          }
        }
        FastLED.show();
        delay(100);
      }
      if (who == PIECE_O && placeable(dimx, dimy, x_pos, y_pos)) {
        for (int i = x_pos; i < x_pos + dimx; i++) {
          for (int j = y_pos; j < y_pos + dimy; j++) {
            leds[board_side_2[i][j]] = shipColor;
          }
        }
        FastLED.show();
        delay(500);
        for (int i = x_pos; i < x_pos + dimx; i++) {
          for (int j = y_pos; j < y_pos + dimy; j++) {
            leds[board_side_2[i][j]] = waterColor;
          }
        }
        FastLED.show();
        delay(100);
      }
      return;
  }

  
  void loop() {
      
  }

  void reset() {
    winner = false;
    who = PIECE_X;
    x_pos = 0;
    y_pos = 7;
    
    // reset led board to be empty
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 8; j++) {
            leds[board_side_1[i][j]] = eColor;
            leds[board_side_2[i][j]] = eColor;
        }
    }

    // reset game to be empty
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 8; j++) {
            game1[i][j] = PIECE_EMPTY;
            game2[i][j] = PIECE_EMPTY;
        }
    }
  }