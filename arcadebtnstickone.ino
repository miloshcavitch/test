const int DIRECTION[] = {A1, A2, A3, A4};
int dirCount = 4;
#define BUTTONCOUNT 8
int btnCount = BUTTONCOUNT;

const int buttonPins[BUTTONCOUNT] = {12, 2, 13, 4, 5, 6, 7, 8};
const int kRed = 9;
const int kBlue = 10;
const int kGreen = 11;
const int kPot = A0;
const int kStrobe = A5;
const int kRGBPower = 3;


int btnState[BUTTONCOUNT] = {0, 0, 0, 0, 0, 0, 0, 0};
int prvbtnState[BUTTONCOUNT] = {0, 0, 0, 0, 0, 0, 0, 0};

int leftState = 0;
int upState = 0;
int rightState = 0;
int downState = 0;

int fiveThousand = 0;
int colorPalette[BUTTONCOUNT][3] = {
  {255, 0, 0},
  {145, 0, 170},
  {100, 145, 0},
  {70, 0, 220},
  {0, 255, 0},
  {0, 0, 255},
  {0, 220, 50},
  {0, 100, 170}
};

int spf = analogRead(kPot);
int sped;
int stepto = 0;

int ogRed = 0;
int ogGreen = 0;
int ogBlue = 0;

int prevRed = 0;
int prevGreen = 0;
int prevBlue = 0;

int destinationRed = 0;
int destinationGreen = 0;
int destinationBlue = 0;

void setColor(int red, int green, int blue){
  int rojo = 255 - red;
  analogWrite(kRed, rojo);
  int verde = 255 - green;
  analogWrite(kGreen, verde);
  int azul = 255 - blue;
  analogWrite(kBlue, azul);
}

int largestDiff(int prRed, int prGreen, int prBlue, int nwRed, int nwGreen, int nwBlue){
  int rr = prRed-nwRed;

  int rojo;
  rojo = abs(rr);

  int gg = prGreen-nwGreen;
  int verde = abs(gg);
  int bb = prBlue-nwBlue;
  int azul = abs(bb);
  Serial.println("Largest DIff is: ");
  if (rojo >= verde && rojo >= azul){
    Serial.println(rojo);
    return rojo;
  }
  if (verde >= rojo && verde >= azul){
    Serial.println(verde);
    return verde;
  }
  if (azul >= rojo && azul >= verde){
    Serial.println(azul);
    return azul;
  }
}


void nothingMode(int red, int green, int blue, int steppage, int spd, int time){// send this function prevRed/Green/Blue and steppage is the 100,000/largestdiff
  setColor(red, green, blue);
  int neoRed = red;
  int neoGreen = green;
  int neoBlue = blue;
  int elTiempo = steppage;
  if (red != destinationRed){
    if (red - destinationRed < 0){
      neoRed = red + 5;
    }
    if (red - destinationRed > 0){
      neoRed = red - 5;
    }
    prevRed = neoRed;
  }
  //asdfjalksdfjalksdf
  if (green != destinationGreen){
    if (green - destinationGreen < 0){
      neoGreen = green + 1;
    }
    if (green - destinationGreen > 0){
      neoGreen = green - 1;
    }
    prevGreen = neoGreen;
  }
  if (blue != destinationBlue){
    if (blue - destinationBlue < 0){
      neoBlue = blue + 1;
    }
    if (blue - destinationBlue > 0){
      neoBlue = blue - 1;
    }
    prevBlue = neoBlue;
  }
  for (int i = 0; i < btnCount; i++){
    if (digitalRead(buttonPins[i]) == LOW){
        prvbtnState[i] = btnState[i];
        btnState[i] = 1;
        destinationRed = colorPalette[i][0];
        destinationGreen = colorPalette[i][1];
        destinationBlue = colorPalette[i][2];
          if (btnState[i] != prvbtnState[i] && btnState[i] == 1){
            stepto = 100000/(largestDiff(prevRed, prevGreen, prevBlue, destinationRed, destinationGreen, destinationBlue));
            Serial.println(stepto);
          }

    }
  }
  delayMicroseconds(elTiempo);
}

int stickState = 0;
int prvStickState = 0;
int currentFunction = 5;
int prvFunction;

void setup(){
  Serial.begin(9600);
  for (int i = 0; i < dirCount; i++){
    pinMode(DIRECTION[i], INPUT);
    digitalWrite(DIRECTION[i], HIGH);
  }
  for(int i = 0; i <btnCount; i++){
    pinMode(buttonPins[i], INPUT);
    digitalWrite(buttonPins[i], HIGH);
  }
  pinMode(kRGBPower, OUTPUT);
  digitalWrite(kRGBPower, HIGH);
  pinMode(kRed, OUTPUT);
  pinMode(kBlue, OUTPUT);
  pinMode(kGreen, OUTPUT);
  }


int whichDirection(int left, int up, int right, int down)
{
  int change = 0;
  int DVALS[] = {left, up, right, down};
  for (int i = 0; i < dirCount; i++){
  if (digitalRead(DVALS[i]) == LOW){
    long lastDebounce = millis();
    int yescounter = 0;
    int nocounter = 0;
    while (millis() < (lastDebounce + 10)){
      if(digitalRead(DVALS[i]) == LOW){
        yescounter++;
      }
      if (digitalRead(DVALS[i]) == HIGH){
        nocounter++;
      }
    }
    if (yescounter > nocounter){
      return i;
    }
  }
 }
  return 5;
}



void loop(){
  long loopStart = micros();
  for (int i = 0; i < btnCount; i++){
    if (digitalRead(buttonPins[i]) == HIGH){
      btnState[i] = 0;
    }
  } 
    
    spf = analogRead(kPot);
    sped = map(spf, 0, 1023, 0, 40);
    
  prvStickState = stickState;
  stickState = whichDirection(DIRECTION[0],DIRECTION[1], DIRECTION[2], DIRECTION[3]);
  if (stickState == 0){
    nothingMode(prevRed, prevGreen, prevBlue, stepto, sped, loopStart);
  }
  if (stickState == 1){
    //insert future code here
  }
  if (stickState == 2){
    //insert future code here
  }
  if (stickState == 3){
    //insert future code here
  }
  if (stickState != prvStickState){
    prvFunction = prvStickState;
    switch (stickState){
      case 0:
        Serial.println("left");
        break;
      case 1:
        Serial.println("up");
        break;
      case 2:
        Serial.println("right");
        break;
      case 3:
        Serial.println("down");
        break;
      case 5:
        Serial.println("nothing");
        break;
    }
  }
  long loopTime = micros() - loopStart;
}
      
      
  
