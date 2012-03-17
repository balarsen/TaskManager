

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define SERIAL_DEBUG

class Task {
  public:
    Task( void (*fn)(), uint32_t ticks);
    Task operator++();    //Prefix decrement operator  (++x)
  private:
    void (*function)();
    uint32_t counter;
    uint32_t max_cnt;
};

Task::Task(void (*fn)(), uint32_t ticks ) {
  #ifdef SERIAL_DEBUG
    Serial.println("in constructor");
  #endif
  function = fn;
  counter = 0;  
  max_cnt = ticks;
}

Task Task::operator++() {
  #ifdef SERIAL_DEBUG
    Serial.print("in ++: ");
    Serial.print("counter: ");
    Serial.print(counter, DEC);
  #endif
  counter++;
  #ifdef SERIAL_DEBUG
    Serial.print(" counter: ");
    Serial.print(counter, DEC);
    Serial.print(" max_cnt: ");
    Serial.println(max_cnt, DEC);
  #endif
  if (counter >= max_cnt) {
    counter = 0;  
    function();
  }
    
}

void isr(void)
{
  digitalWrite(greenLEDpin, digitalRead(greenLEDpin) ^ 1);
  #ifdef SERIAL_DEBUG
    Serial.println("In the ISR");  
  #endif
}



Task tsk = Task(isr, 10);

void setup()
{
  pinMode(greenLEDpin, OUTPUT);   
  #ifdef SERIAL_DEBUG
    Serial.begin(19200);
  #endif
}

void loop()
{
  delay(1000);
  ++tsk;
}
