/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 9
#define in1 6
#define in2 7



void setup() 
{

  Serial.begin(9600);

  delay(3000); // 3 second delay for recovery
  // put your setup code here, to run once:
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop() 
{
  delay(1000); // 3 second delay for recovery

  // put your main code here, to run repeatedly:
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  char line[100];
  sprintf(line,"pwmOutput = %d",pwmOutput);
  Serial.println(line);
  
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
}
