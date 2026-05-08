/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 9
#define in1 6
#define in2 7



int minimum_pwm_output = 30;

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


  // initialize the water column at half height
  analogWrite(enA, minimum_pwm_output); // Send PWM signal to L298N Enable pin
}


int current_pwm_output = minimum_pwm_output;
int growing_water_column = true;
bool in_startup = true;
int iterations_in_startup = 300;
int startup_iteration=0;

int growth_delay = 10;
int decay_delay=50;

int growth_increment = 2;
int decay_decrement = 1;

void loop() 
{
  // put your main code here, to run repeatedly:
  char line[100];
  sprintf(line,"current_pwm_output = %d",current_pwm_output);
  Serial.println(line);
  
  analogWrite(enA, current_pwm_output); // Send PWM signal to L298N Enable pin
  if(in_startup)
  {
    delay(growth_delay);

    startup_iteration++;
    if(startup_iteration>iterations_in_startup)
    {
      in_startup=false;
    }

  }
  else if(growing_water_column)
  {
    delay(growth_delay);
    current_pwm_output+=growth_increment;
    if(current_pwm_output>255)
    { 
      current_pwm_output=255;
      growing_water_column = false;
      delay(1000);
    }
  }
  else
  {
    delay(decay_delay);
    current_pwm_output-=decay_decrement;
    if(current_pwm_output<minimum_pwm_output)
    {
      current_pwm_output = minimum_pwm_output;
      growing_water_column = true;
      delay(1000);
    }
  }
}
