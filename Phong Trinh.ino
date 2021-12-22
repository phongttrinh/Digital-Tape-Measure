//Phong Thanh Trinh 
int analogValue; //creates a variable integer called 'analogValue' 
int analogPin = 10; //creates a variable integer called 'analogPin' 
float voltage[9]; //creates a array variable float called 'voltage[9]'
float int_array[9]; //creates a array variable float called 'int_array[9]'
int i; //creates a variable integer called 'i' 
int int_flag = 0; //creates a variable integer called 'int_flag' 
int timer1_counter; //creates a variable integer called 'timer1_counter' 
float total; //creates a variable float called 'total' 
float average; //creates a variable float called 'average' 
float Maxvoltage; //creates a variable float called 'Maxvoltage' 
float median; //creates a variable float called 'meidan'
float DistanceMedian; //creates a variable float called 'DistanceMedian'
float DistanceAverage; //creates a variable float called 'DistanceAverage'
int rep; //creates a variable integer called 'rep' 
int NumberMaxvolt; //creates a variable integer called 'NumberMaxvolt' 

void setup() {
  delay(2000);
  Serial.begin(9600);  //Initialize serial commmunications
  pinMode(analogPin, OUTPUT); // set analogPin to output
  pinMode(13, OUTPUT); // sets the digital pin 13 as output
  digitalWrite(13, LOW); // sets the digital pin 13 off


  noInterrupts();           // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  timer1_counter = 59286;   // preload timer 65536-16MHz/256/10Hz
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

  interrupts();             // enable all interrupts
}
ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  TCNT1 = timer1_counter;   // preload timer
  int_flag = 1;             // interrupt has ocurred
}

void loop() {

  if (int_flag == 1) // if the interrupt occurs
  {
    int_flag = 0; // int_flag back to zero
    for (i = 0; i <= 8; i++) // set the loop to get 9 sample with the rate of 10Hz
    {
      analogValue = analogRead(A0); // set analogValue get the value from A0
      voltage[i] = analogValue * (5.0 / 1023.0); //Set equation to get array of voltage
      Serial.println(voltage[i]); //print out the array of voltage including 9 samples
    }


 //****** FIDINDING AVERAGE****//
    total = 0; //Set total equal zero
    average = 0; // set average equal zero
    for (i = 0; i <= 8; i++) //set a loop for 9 samples
    {
      total = total + voltage[i]; // set equation to get total of 9 samples
    }
    average = total / 9; // set equation to get the average after loop
    DistanceAverage = ((1 / average) - 0.1727) / 0.031; //convert to distance (cm)

    
    //****FINDING MEDIAN****//
    /* The idea is that in first loop, finding the maximun of the array and then set it zero.
    Then do that five times. In the fifth times, the maximun will be median*/

    median = 0; //Firstly, set the median equal zero
    Maxvoltage = 0; // set the maxvoltage equal zero
    for (i = 0; i <= 8; i++) {          // set the first loop to find max of the array
      if (Maxvoltage < voltage[i]) {   // the sample is greater than maxvoltage
        Maxvoltage = voltage[i];      //---> it will be the max
        NumberMaxvolt = i;           // the order of this sample in the array

      }
    }
    Maxvoltage = 0;              //set the voltage equal to zero to start next loop 
    voltage[NumberMaxvolt] = 0; // set this max sample equal zero
    for (i = 0; i <= 8; i++) {  // do this five time
      if (Maxvoltage < voltage[i]) {
        Maxvoltage = voltage[i];
        NumberMaxvolt = i;

      }
    }
    Maxvoltage = 0;
    voltage[NumberMaxvolt] = 0;
    for (i = 0; i <= 8; i++) {
      if (Maxvoltage < voltage[i]) {
        Maxvoltage = voltage[i];
        NumberMaxvolt = i;

      }
    }

    Maxvoltage = 0;
    voltage[NumberMaxvolt] = 0;
    for (i = 0; i <= 8; i++) {
      if (Maxvoltage < voltage[i]) {
        Maxvoltage = voltage[i];
        NumberMaxvolt = i;

      }
    }

    Maxvoltage = 0;
    voltage[NumberMaxvolt] = 0;
    for (i = 0; i <= 8; i++) {
      if (Maxvoltage < voltage[i]) {
        Maxvoltage = voltage[i];
        NumberMaxvolt = i;

      }
    }

    Maxvoltage = 0;
    voltage[NumberMaxvolt] = 0;
    for (i = 0; i <= 8; i++) {
      if (Maxvoltage < voltage[i]) {
        Maxvoltage = voltage[i];
        median = Maxvoltage;       // At the fifth time, the media euqal to the max sample
      }

    }
    DistanceMedian = ((1 / median) - 0.1727) / 0.031; //Convert to distance
   
    //Print out: Voltage average, Distance average, Voltage median, distance Median
    Serial.print("Voltage Average = "); Serial.print(average);
    Serial.print(" \tDistance Average = "); Serial.print(DistanceAverage); Serial.print("\n");
    Serial.print("Voltage Median = "); Serial.print(median);
    Serial.print("\tDistance Median = "); Serial.print(DistanceMedian);
    Serial.print("\n");
  }


  
  //**** Set the Code to Alert the Operator***//
  if (DistanceAverage > 80 || DistanceAverage < 10) {     //if the Distanceaverage > 80 or <10
    for (rep = 0; rep <= 2; rep++) {   //set the loop to turn on and of the Led 
      digitalWrite(13, HIGH); // turn on the led
      delay(150);
      digitalWrite(13, LOW); // turn off the Led
      delay(150);

    }
  }
}
