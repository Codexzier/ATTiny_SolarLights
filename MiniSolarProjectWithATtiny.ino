// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Mini Solar Projekt mit ATtiny
// Author:        Johannes P. Langner
// Controller:    ATtiny25/45/85 and Setup to internal 1MHz
// Power supply:  Solar panel 2V 40mA
// Actor:         5 x LEDs (RED or ORANGE)
// Description:   Ein kleines Projekt für ein Deko Objekt, dass mit Solar Betrieben wird. 
//                Durch optimieren der Lichtzustände ist der Verbrauch bis auf ein 
//                fünftel reduziert.
// Stand:         01.07.2018
// ========================================================================================


// ========================================================================================
// LightController for the energy saving
bool mLightsOn[5];
int mLastTime = 0;
int mCurrentLED = 0;

// ========================================================================================
// counter up is the value to show in binar with the LEDs.
int mVal = 0;

// ========================================================================================
void setup(){
  for(int i = 0; i < 5; i++) {
    pinMode(i, OUTPUT);
    mLightsOn[i] = false;
  }
}

// ========================================================================================
void loop() {

  LightControllerShow();
  
  int currentTime = millis();

  if(currentTime - mLastTime > 1000) {
    mLastTime = currentTime;
    Next();
  }
}

// ========================================================================================
// Count up the value and transfform the value
void Next() {
  
  if(mVal > 32) {
    mVal = 0;
    RunLight();
  }
  else {
    mVal++;
  }

  DecimalToBinar(mVal);
}

// ========================================================================================
// Prepare Decimal to binar. Write to the bool array to setup the ligthController.
void DecimalToBinar(int val) {

  int bin = 16, index = 0;

  SetOnTo(-1);
  while(true) {
    
    if(val >= bin) {
      mLightsOn[index] = true;
      val -= bin;
    }
    else {
      mLightsOn[index] = false;
    }
  
    index++;
    bin /= 2;
  
    if(val <= 0 || index > 4 || bin == 0) {
      break;
    }
  }
}

// ========================================================================================
// Close running with controlling ligth out of the loop
// The light drive 10 times left to right and back.
void RunLight() {
  int iteration = 0,lastTime = 0, lightOn = 0;
  bool up = true;

  SetOnTo(lightOn);
  
  while(true) {

    if(iteration > 10) {
      break;
    }
    
    LightControllerShow();
  
    int currentTime = millis();
    if(currentTime - lastTime > 200) {
      lastTime = currentTime;
      if(up) {
        lightOn++;
      }
      else {
        lightOn--;
      }
  
      if(lightOn >= 5) {
        up = false;
        lightOn = 4;
      }
  
      if(lightOn < 0) {
        up = true;
        lightOn = 0;
        iteration++;
      }
  
      SetOnTo(lightOn);
    }
  }
}

// ========================================================================================
// prepare light state to off.
// ----------------------------------------------------------------------------------------
// setup on light to on. Set -1 to for all off.
void SetOnTo(int indexLight){
  for(int i = 0; i < 5; i++) {
    mLightsOn[i] = false;
  }

  if(indexLight < 0) {
    return;
  }
  mLightsOn[indexLight] = true;
}

// ========================================================================================
// update light to next light up led. This is the primary solution to save energy.
void LightControllerShow() {

  if(mCurrentLED < 4) 
    mCurrentLED++;  
  else
    mCurrentLED = 0;
  
  for(int i = 0; i < 5; i++) {

    if(mCurrentLED == i && mLightsOn[i] == true)
      digitalWrite(i, HIGH);
    else
      digitalWrite(i, LOW);
  }
}
