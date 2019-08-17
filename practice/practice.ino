//Motor Connections
//Change this if you wish to use another diagram


//Lukewasherelolz
#define EnA 3
#define EnB 2
#define In1 28
#define In2 26
#define In3 24
#define In4 22

#define SENSOR1 53
#define SENSOR2 51
#define SENSOR4 47
#define SENSOR3 49

#define LOGIC HIGH

int buttonState = 0;
int oldstat = 0;
int sen1 = 0;
int sen2 = 0;
int sen3 = 0;
int sen4 = 0;
int first = 1;
int sens[] = {0, 0, 0, 0};
int location[] = {0, 0, 0, 0, 0, 0, 0, 0};
void setup()
{
  // All motor control pins are outputs
  Serial.begin(9600);
  /*pinMode(buttonPin, INPUT);*/
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
}
//prints sensors
void blinkLight(int sen1, int sen2, int sen3, int sen4){
  
  if(sen1 == LOW) {
    
    Serial.print("sen1\n");
   
  }
  
  if(sen2 == LOW) {
    
    Serial.print("sen2\n");
   
  }
  
  if(sen3 == LOW) {
    
    Serial.print("sen3\n");
   
  }
  
  if(sen4 == LOW) {
    
    Serial.print("sen4\n");
   
  }
}
//
//paremeter
//index at 0 for sensors
//4 == left and right mid
//5 == right and left mid
//
// returns true if only that sensor is on
int justSen(int senIndex){
  if(senIndex == 0){
    return !(sen1 == LOGIC && sen2 != LOGIC && sen3 != LOGIC && sen4 != LOGIC);
  }
  else if(senIndex == 1){
    return !(sen1 != LOGIC && sen2 == LOGIC && sen3 != LOGIC && sen4 != LOGIC);
  }
  else if(senIndex == 2){
    return !(sen3 == LOGIC && sen2 != LOGIC && sen1 != LOGIC && sen4 != LOGIC);
  }
  else if(senIndex == 3){
    return !(sen4 == LOGIC && sen2 != LOGIC && sen3 != LOGIC && sen1 != LOGIC);
  }
  else if(senIndex == 4){
    return !(sen1 != LOGIC && sen3 != LOGIC && sen2 == LOGIC && sen4 == LOGIC);
  }
  else if(senIndex ==5){
    return !(sen2 != LOGIC && sen4 != LOGIC && sen1 == LOGIC && sen3 == LOGIC);
  }
}
//
//parameters
//0 (1,2)
//1 (2,3)
//2 (3,4)
//3 (1,2,3)
//4 (2,3,4)
//
// returns true if pair of motors are being sensed
int justPair(int senIndex) {
  if(senIndex == 0){
    return !(sen3 != LOGIC && sen4 != LOGIC && sen2 == LOGIC && sen1 == LOGIC);
  }
  else if(senIndex == 1){
    return !(sen3 == LOGIC && sen2 == LOGIC && sen1 != LOGIC ||  sen4 != LOGIC);
  }
  else if(senIndex == 2){
    return !(sen3 == LOGIC && sen4 == LOGIC && sen2 != LOGIC || sen1 != LOGIC);
  }
  else if(senIndex == 3){
    return !(sen3 == LOGIC && sen4 != LOGIC && sen2 == LOGIC || sen1 == LOGIC);
  }
  else if(senIndex == 4){
    return !(sen3 == LOGIC && sen4 == LOGIC && sen2 == LOGIC || sen1 != LOGIC);
  }
 
}
int checkLast(int sens[]) {
  if((sens[0] != LOGIC && sens[1] != LOGIC && sens[2] != LOGIC && sens[3] != LOGIC) || (sens[0] != LOGIC && sens[1] == LOGIC && sens[2] == LOGIC && sens[3] != LOGIC)) {
      
      //forward
      return 2; //first is still 1
  }
  if((sens[0] == LOGIC && sens[1] != LOGIC && sens[2] != LOGIC && sens[3] != LOGIC) || (sens[0] != LOGIC && sens[1] == LOGIC && sens[2] != LOGIC && sens[3] != LOGIC) || (sens[2] != LOGIC && sens[3] != LOGIC && sens[1] == LOGIC && sens[0] == LOGIC)){ //left
      return 0;
    }
    else if((sens[2] == LOGIC && sens[1] != LOGIC && sens[0] != LOGIC && sens[3] != LOGIC) || (sens[3] == LOGIC && sens[1] != LOGIC && sens[2] != LOGIC && sens[0] != LOGIC) || (sens[2] == LOGIC && sens[3] == LOGIC && sens[1] != LOGIC || sens[0] != LOGIC)){//right
      return 1;
    }
    
    else if( ((sens[0] != LOGIC && sens[2] != LOGIC && sens[1] == LOGIC && sens[3] == LOGIC)) || ((sens[1] != LOGIC && sens[3] != LOGIC && sens[0] == LOGIC && sens[2] == LOGIC)) || ((sens[2] == LOGIC && sens[3] != LOGIC && sens[1] == LOGIC || sens[0] == LOGIC)) || (sens[2] == LOGIC && sens[3] == LOGIC && sens[1] == LOGIC || sens[0] != LOGIC)) { //line
      return 3;
    }
}
//
// parameters
//left 0
//right 1
//straight 2
//line 3
//offtrack 4
//
// returns which way the robot should go
int findOut(int sens[]) {
  //if first time sensing
  
    //if all sensors are not triggered
    Serial.print("first");
    Serial.print(first);
    if(sen1 != LOGIC && sen2 != LOGIC && sen3 != LOGIC && sen4 != LOGIC) {
      if(!first) {
        return checkLast(sens);
      }
      
      return 2; //first is still 1
    }
    first = 0;
    if(justSen(0) || justSen(1) || justPair(0)){ //left
      return 0;
    }
    else if(justSen(2) || justSen(3) || justPair(2)){//right
      return 1;
    }
    else if(justPair(1)){ //straight
      return 2;
    }
    else if( (justSen(4)) || (justSen(5)) || justPair(3) || justPair(4)) { //line
      return 3;
    }
  
}
// checks sensors and runs motors
// returns void, calls other functions to move motors
void go()   
{
  // turn on motor A
  sen1 = digitalRead(SENSOR1);
  sen2 = digitalRead(SENSOR2);
  sen3 = digitalRead(SENSOR3);
  sen4 = digitalRead(SENSOR4);
  
  /*buttonState = digitalRead(buttonPin);*/
  
  
  int calc = findOut(sens);
  sens[0] = sen1;
  sens[1] = sen2;
  sens[2] = sen3;
  sens[3] = sen4;
  
  if(calc == 0){
    Serial.print("move left\n");
    //blinkLight(sen1, sen2, sen3, sen4);
    printSens();
    
    move(0, 0, 120);
    move(0, 0, 0);
    delay(10);
    
  }
  // move right
  else if(calc == 1){
    Serial.print("move right\n");
    //blinkLight(sen1, sen2, sen3, sen4);
    printSens();
    move(0, 0, 0);
    move(1, 0, 120);
    delay(10);
  }
  
  // move forward
  else if(calc == 2) {
    Serial.print("move forward\n");
    //blinkLight(sen1, sen2, sen3, sen4);
    printSens();
    move(0, 0, 150);
    move(1, 0, 150);
    delay(10);
  }
  // turn
  else if(calc == 3){
    Serial.print("Help\n");
  }  
}

// prints the sensor values
// returns void
void printSens() {
  Serial.print(sen1);
  Serial.print(sen2);
  Serial.print(sen3);
  Serial.print(sen4);
}

//
// parameters
// motor 0 - (in1, in2)
// motor 1 - (in3, in4)
// dir 0 - forward
// dir 1 - forward
//
// return void, moves motor
void move(int motor, int dir, int sped){
  if(motor == 0) {
    if(dir == 0) {
      digitalWrite(In2, HIGH);
      digitalWrite(In1, LOW);
      analogWrite(EnA, sped);
    }
    else{
      digitalWrite(In2, LOW);
      digitalWrite(In1, HIGH);
      analogWrite(EnA, sped);
      
    } 
  }
  else if(motor == 1) {
    if(dir == 0) {
      digitalWrite(In4, HIGH);
      digitalWrite(In3, LOW);
      analogWrite(EnB, sped);
    }
    else{
      digitalWrite(In4, LOW);
      digitalWrite(In3, HIGH);
      analogWrite(EnB, sped);
      
    }
    
  }
}

void loop()
{
  go();
  
}
