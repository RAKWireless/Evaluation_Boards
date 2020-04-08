
//4600 supply 2 gpio for wisblock
int gpio_1 = 14;
int gpio_2 = 17;

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);    
  pinMode(gpio_1, OUTPUT);
  pinMode(gpio_2, OUTPUT);
}



void loop()
{
    Serial.println("gpio_1 test with a led!");
    digitalWrite(gpio_1, 1);
    delay(1000);
    Serial.println("gpio_1 test with a led!");
    digitalWrite(gpio_1, 0);    
    delay(1000);
}
