//UMD ENEE 408R E-bike
//Baed on Evans code (some stuff from internet)
//Modified by JL 05/04/19
//Kinda works on BLDC motors 

//HIGH Sides
#define INHA 9
#define INHB 10 
#define INHC 11
//LOW Sides
#define INLA 5 
#define INLB 6
#define INLC 7

#define HallA 8
#define HallB 12
#define HallC 2  

#define ENABLE 4  //Need for DRV8XXX series pre-driver
                  //Pull high

//Halls 
int hallValA;
int hallValB;
int hallValC;

int pwmOutput = 0; // DUTY factor

float Loop_tim = 0; // loop timer for ramp up

void setup() {
  //HIGH Sides
  //asynchronous pwm from arduino but pre-driver in 3-mode so low should complement and become synchronous should probably confirm
  pinMode(INHA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INHC, OUTPUT);
  //LOW Sides
  pinMode(INLA, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(INLC, OUTPUT);

  /*Clock set*/
  // Set PWM for pins 9,10 to 32 kHz
  //First clear all three prescaler bits:
  int prescalerVal = 0x07; //create a variable called prescalerVal and set it equal to the binary number "00000111"
  TCCR1B &= ~prescalerVal; //AND the value in TCCR0B with binary number "11111000"

  //Now set the appropriate prescaler bits:
  int prescalerVal2 = 0b001; //set prescalerVal equal to binary number "00000001"
  TCCR1B |= prescalerVal2; //OR the value in TCCR0B with binary number "00000001"
  
  // Set PWM for pins 3,11 to 32 kHz (Only pin 11 is used in this program)
  //First clear all three prescaler bits:
  TCCR2B &= ~prescalerVal; //AND the value in TCCR0B with binary number "11111000"

  //Now set the appropriate prescaler bits:
  TCCR2B |= prescalerVal2; //OR the value in TCCR0B with binary number "00000001"//First clear all three prescaler bits:
  
  
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH); 
  
  digitalWrite(INLA, HIGH);
  digitalWrite(INLB, HIGH);
  digitalWrite(INLC, HIGH);
  delay(15);

  digitalWrite(INLA, LOW);
  digitalWrite(INLB, LOW);
  digitalWrite(INLC, LOW);
  delay(15);
  
  Serial.begin(9600); //uncomment this line if you will use the serial connection
}

void loop() {
  // Ramp-up loop
  if(Loop_tim == 1000){
    if (pwmOutput < 250)
      pwmOutput++;
    else
      pwmOutput = 250;
    Serial.println(Loop_tim);
    Serial.println(pwmOutput);
    Loop_tim =0;
  }
  //pwmOutput = 50;
  
  //HALLS
  hallValA = digitalRead(HallA);  // read input value from Hall 1
  hallValB = digitalRead(HallB);  // read input value from Hall 2
  hallValC = digitalRead(HallC);  // read input value from Hall 3  

if (hallValA == 0 && hallValB == 0 && hallValC == 0) {
    // stop: should not get here
    /*
    digitalWrite(INHA, LOW);
    digitalWrite(INLA, LOW);
    digitalWrite(INHB, LOW);
    digitalWrite(INLB, LOW);
    digitalWrite(INHC, LOW);
    digitalWrite(INLC, LOW);
    
    analogWrite(INLA, LOW);
    digitalWrite(INHB, LOW);
    digitalWrite(INLB, HIGH);
    digitalWrite(INHC, LOW);
    digitalWrite(INLC, HIGH);
    */
    Serial.println("Read STOP from hall sensors");
  } 
  else if (hallValA == 1 && hallValB == 1 && hallValC == 1) {
    // align: should not get here
   /* analogWrite(INHA, pwmOutput);
    analogWrite(INLA, LOW);
    digitalWrite(INHB, LOW);
    digitalWrite(INLB, HIGH);
    digitalWrite(INHC, LOW);
    digitalWrite(INLC, HIGH);
    */
    
    Serial.println("Read ALIGN from hall sensors");
  }
  else if (hallValA == 1 && hallValB == 1 && hallValC == 0) {
    // STATE 1:
    // write the pwm to the appropriate gate
    Serial.println("STATE 1");
    digitalWrite(INLA, LOW);
    digitalWrite(INHA, LOW);
    
    digitalWrite(INLB, HIGH);
    analogWrite(INHB, pwmOutput);
    
    digitalWrite(INLC, HIGH);
    digitalWrite(INHC, LOW);
  
  } 
  else if (hallValA == 1 && hallValB == 0 && hallValC == 0) {
    // STATE 2:
    // write the pwm to the appropriate gate
    Serial.println("STATE 2");
    digitalWrite(INLA, HIGH);
    analogWrite(INHA, pwmOutput);
 
    
    digitalWrite(INLB, LOW);
    digitalWrite(INHB, LOW);
    
    digitalWrite(INLC, HIGH);
    digitalWrite(INHC, LOW);

  } 
  else if (hallValA == 1 && hallValB == 0 && hallValC == 1) {
    // STATE 3:
    // write the pwm to the appropriate gate
    Serial.println("STATE 3");
    digitalWrite(INLA, HIGH);
    analogWrite(INHA, pwmOutput);
    
    digitalWrite(INLB, HIGH);
    digitalWrite(INHB, LOW);

    digitalWrite(INLC, LOW);
    digitalWrite(INHC, LOW);

  } 
  else if (hallValA == 0 && hallValB == 0 && hallValC == 1) {
    // STATE 4:
    // write the pwm to the appropriate gate
   Serial.println("STATE 4");
    digitalWrite(INLA, LOW);
    digitalWrite(INHA, LOW);
    
    digitalWrite(INLB, HIGH);
    digitalWrite(INHB, LOW);

    digitalWrite(INLC, HIGH);
    analogWrite(INHC, pwmOutput);
  } 
  else if (hallValA == 0 && hallValB == 1 && hallValC == 1) {
    // STATE 5:
    // write the pwm to the appropriate gate
    Serial.println("STATE 5");
    digitalWrite(INLA, HIGH);
    digitalWrite(INHA, LOW);

    digitalWrite(INLB, LOW);
    digitalWrite(INHB, LOW);

    digitalWrite(INLC, HIGH);
    analogWrite(INHC, pwmOutput);
  } 
  else if (hallValA == 0 && hallValB == 1 && hallValC == 0) {
    // STATE 6:
    // write the pwm to the appropriate gate
    Serial.println("STATE 6");
    digitalWrite(INLA, HIGH);
    digitalWrite(INHA, LOW);

    digitalWrite(INLB, HIGH);
    analogWrite(INHB, pwmOutput);
        
    digitalWrite(INLC, LOW);
    digitalWrite(INHC, LOW);

  }
  Loop_tim++;
  delayMicroseconds(50); 
} 


//Broken sequence//
 /*
  Serial.print("HALLA: ");
  Serial.println(hallValA);
  Serial.print("HALLB: ");
  Serial.println(hallValB);
  Serial.print("HALLC: ");
  Serial.println(hallValC);
  //HallSTATE = (HallState1) + (2*HallState2) + (4*HallState3);  
  */

  /*
  if (hallValA == 1 && hallValB == 0 && hallValC == 0) {
    // STATE 1:
    // write the pwm to the appropriate gate
    Serial.println("STATE 1");
    digitalWrite(INLA, HIGH);
    analogWrite(INHA, pwmOutput);
    
    analogWrite(INLB, HIGH);
    analogWrite(INHB, LOW);
    
    digitalWrite(INLC, LOW);
    digitalWrite(INHC, LOW);
  
  } 
  else if (hallValA == 1 && hallValB == 1 && hallValC == 0) {
    // STATE 2:
    // write the pwm to the appropriate gate
    Serial.println("STATE 2");
    digitalWrite(INLA, HIGH);
    analogWrite(INHA, pwmOutput);
 
    
    digitalWrite(INLB, LOW);
    digitalWrite(INHB, LOW);
    
    digitalWrite(INLC, HIGH);
    digitalWrite(INHC, LOW);

  } 
  else if (hallValA == 1 && hallValB == 1 && hallValC == 1) {
    // STATE 3:
    // write the pwm to the appropriate gate
    Serial.println("STATE 3");
    digitalWrite(INLA, LOW);
    digitalWrite(INHA, LOW);
    
    digitalWrite(INLB, HIGH);
    analogWrite(INHB, pwmOutput);

    digitalWrite(INLC, HIGH);
    digitalWrite(INHC, LOW);

  } 
  else if (hallValA == 0 && hallValB == 1 && hallValC == 1) {
    // STATE 4:
    // write the pwm to the appropriate gate
    Serial.println("STATE 4");
    digitalWrite(INLA, HIGH);
    digitalWrite(INHA, LOW);
    
    digitalWrite(INLB, HIGH);
    analogWrite(INHB, pwmOutput);

    digitalWrite(INLC, LOW);
    digitalWrite(INHC, LOW);
  } 
  else if (hallValA == 0 && hallValB == 0 && hallValC == 1) {
    // STATE 5:
    // write the pwm to the appropriate gate
    Serial.println("STATE 5");
    digitalWrite(INLA, HIGH);
    digitalWrite(INHA, LOW);

    digitalWrite(INLB, LOW);
    digitalWrite(INHB, LOW);

    digitalWrite(INLC, HIGH);
    analogWrite(INHC, pwmOutput);
  } 
  else if (hallValA == 0 && hallValB == 0 && hallValC == 0) {
    // STATE 6:
    // write the pwm to the appropriate gate
    Serial.println("STATE 6");
    digitalWrite(INLA, LOW);
    digitalWrite(INHA, LOW);

    digitalWrite(INLB, HIGH);
    digitalWrite(INHB, LOW);
        
    digitalWrite(INLC, HIGH);
    analogWrite(INHC, pwmOutput);

  }
  */ 


