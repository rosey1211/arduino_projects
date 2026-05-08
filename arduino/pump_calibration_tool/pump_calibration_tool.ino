
#include <Arduino.h>

#define enA 9
#define in1 6
#define in2 7



int minimum_pwm_output = 10;


void setup() {
  // put your setup code here, to run once:

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

int growth_increment = 10;
int decay_decrement = 1;
int level_start_time_ms=0;

int time_at_level_ms = 10000;
bool performing_level=false;

// do each level for 10 seconds and do 
void loop() {
  // put your main code here, to run repeatedly:
  // put your main code here, to run repeatedly:
  delay(100);
  while(current_pwm_output<=255)
  {
    if(!performing_level)
    {
      level_start_time_ms = millis();
      performing_level=true;
    }
    int current_time_ms=millis();
    int elapsed_time = current_time_ms - level_start_time_ms;

    //char line[100];
    //sprintf(line,"current_time_ms = %u : level_start_time_s = %u : elapsed_time = %u\n",current_time_ms,level_start_time_ms,elapsed_time);
    //Serial.println(line);


    if(elapsed_time>time_at_level_ms)
    {
      // send the last and then increment
      char line[100];
      sprintf(line,"current_pwm_output = %d : time_at_level = %u\n",current_pwm_output,elapsed_time);
      Serial.println(line);

      // send the current level
      analogWrite(enA, current_pwm_output); // Send PWM signal to L298N Enable pin
      performing_level=false;
      current_pwm_output+=growth_increment;
    }
    else
    {
      char line[100];
      sprintf(line,"current_pwm_output = %d : time_at_level = %u\n",current_pwm_output,elapsed_time);
      Serial.println(line);

      // send the current level
      analogWrite(enA, current_pwm_output); // Send PWM signal to L298N Enable pin
    }
  }
  char line[100];
  sprintf(line,"completed pump calibration\n");
  Serial.println(line);
}

  