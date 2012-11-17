// Arduino DSLR Flash Trigger
// Craig Bonsignore
// craig.bonsignore.com
// 17-November-2012

// Laser shines on a light sensor. And interruption of the beam is detected
// by a decrease in intensity, which triggers a flash after a specified delay.
// schematic: http://www.flickr.com/photos/cbonsig/5478376580/
// photos: http://www.flickr.com/photos/cbonsig/tags/trigger/

// Based on code from:
// Maurice Ribble 
// 4-12-2008
// http://www.glacialwanderer.com/hobbyrobotics


#define LASER_THRESHHOLD 100 // initial intensity threshold
#define TRIGGER_FRACTION 0.998 // trigger flash if average intensity drops below this fraction

// Falling object: 3.5in ~= 100ms | 6.0in ~=120ms  | 8.0in ~= 140ms
#define DELAY_MS 60 // milliseconds delay for flash trigger

// The digital pins being used
#define CAMERA_FLASH_PIN 4
#define LASER_PIN 5

// The analog pins being used
#define LASER_TRIGGER_ANALOG_PIN 0

int threshold = LASER_THRESHHOLD;
int count=0;
int total=0;

void setup()
{
  pinMode(CAMERA_FLASH_PIN, OUTPUT);
  digitalWrite(CAMERA_FLASH_PIN, LOW);
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);
  
  threshold = LASER_THRESHHOLD; // set the initial threshold value

  digitalWrite(LASER_PIN, HIGH);  // Turn on the Laser

  Serial.begin(9600); // open serial
}

void loop()
{
  int laserVal;

  ////////////////////////////////////////////////////////////
  // LASER TRIGGER
  ////////////////////////////////////////////////////////////
  laserVal = analogRead(LASER_TRIGGER_ANALOG_PIN);
  
  total = total + analogRead(LASER_TRIGGER_ANALOG_PIN);
  count = count + 1;
  
  // calculate an average of 10 intensity readings
  // set the new threshold according of (average)*TRIGGER_FRACTION
  
  if (count == 10)
  {
    threshold = (total/10) * TRIGGER_FRACTION;
    count = 0;
    total = 0;
    Serial.print("New threshold of ");
    Serial.println(threshold, DEC);
  }
   
  if (laserVal < threshold)
  {
    
    // delay the flash for a few milliseconds 
    // comments relate distance traveled by a falling object
    
    delay(60); // 100ms = 3.5in . 120ms = 6.0in . 140ms = 8.0in 175
    
    digitalWrite(CAMERA_FLASH_PIN, HIGH);
    digitalWrite(LASER_PIN, LOW);  // Turn off laser during picture
    Serial.print("laserVal of ");
    Serial.print(laserVal,DEC);
    Serial.print(" < threshold of ");
    Serial.print(threshold,DEC);
    Serial.println(". Flash triggered by laser.");
    delay(1000);
    digitalWrite(CAMERA_FLASH_PIN, LOW);
    digitalWrite(LASER_PIN, HIGH);  // Turn laser back on after picture
    count = 0;
    total = 0;
    threshold = LASER_THRESHHOLD; // revert to original LASER_THRESHOLD
  }
  Serial.print("Laser: ");
  Serial.print(laserVal, DEC);
  Serial.print(", threshold of ");
  Serial.println(threshold, DEC);
}

