

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define SERIAL_DEBUG


class Task {
  public:
    Task( void (*fn)(), uint32_t ticks);
    Task operator++();    //Prefix decrement operator  (++x)
    Task operator++(int); //Postfix decrement operator (x++)
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

Task Task::operator++(int val) {  
  #ifdef SERIAL_DEBUG
    Serial.print("in Task++: ");
  #endif
  Task::operator++();
}

Task Task::operator++() {
  #ifdef SERIAL_DEBUG
    Serial.print("in ++Task: ");
    Serial.print("counter: ");
    Serial.print(counter, DEC);
  #endif
  ++counter;
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

class TaskManager {
  public:
    TaskManager(uint8_t nTasks, uint32_t useconds);  // nTasks is the number of task you will have
    void attach( void (*fn)() );
    void start();
    void stop();
  private:
    Task* tasks;
    uint8_t nTasks;
    void period();
    uint32_t useconds;
    Task overflow();
};

TaskManager::TaskManager(uint8_t nTasks, uint32_t useconds) {
  TCCR1A = 0;                 // clear control register A 
  TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer
  nTasks = nTasks;
  useconds = useconds;
  tasks = (Task*) calloc (nTasks, sizeof(Task));
  if (tasks==NULL) error();
// setup the timer in here
}

void TaskManager::overflow() {
  // as the timer ticks this is the interrupt that is called 
  
}

void TaskManager::attach( void (*fn)() ) {
// add into the Manager 
}

void TaskManager::start() {
  // this function needs a rewrite as I understand it better
  uint16_t tcnt1;
  
  TIMSK1 &= ~_BV(TOIE1);       
  GTCCR |= _BV(PSRSYNC);               

  oldSREG = SREG;                               
  noInterrupts();                                            
  TCNT1 = 0;                    
  SREG = oldSREG;                       

  do {  // Nothing -- wait until timer moved on from zero - otherwise get a phantom interrupt
        oldSREG = SREG;
        noInterrupts();
        tcnt1 = TCNT1;
        SREG = oldSREG;
  } while (tcnt1==0); 
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit

  TCCR1B |= clockSelectBits;

  interrupts();
}

void TaskManager::stop() {
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}

#define RESOLUTION 65536 // Timer1 is a 16 bit timer

void TaskManager::period()
{
  // this function needs a rewrite as I understand it better
  uint32_t cycles = (F_CPU / 2000000) * useconds;                              
  if(cycles < RESOLUTION)              
    clockSelectBits = _BV(CS10);            
  else if((cycles >>= 3) < RESOLUTION) 
    clockSelectBits = _BV(CS11);             
  else if((cycles >>= 3) < RESOLUTION) 
    clockSelectBits = _BV(CS11) | _BV(CS10); 
  else if((cycles >>= 2) < RESOLUTION) 
    clockSelectBits = _BV(CS12);           
  else if((cycles >>= 2) < RESOLUTION) 
    clockSelectBits = _BV(CS12) | _BV(CS10);  
  else        
    cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  
  
  oldSREG = SREG;                               
  noInterrupts();                                                    
  ICR1 = pwmPeriod = cycles;                                         
  SREG = oldSREG;
  
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                        
}





void error(void) {
  #ifdef SERIAL_DEBUG
    Serial.println("ERROR STATE");  
  #endif
  digitalWrite(greenLEDpin, HIGH);
  digitalWrite(redLEDpin, HIGH);
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
  Serial.println(F_CPU, DEC);
  delay(500);
  ++tsk;
  delay(500);
  tsk++;
}
