/*
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See GitHub project https://github.com/balarsen/TaskManager for latest 
 *      version, please feel free to fork and/or post patches
 */
#ifndef TASKMANAGER_cpp
#define TASKMANAGER_cpp

#include <TimerOne.h>

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define SERIAL_DEBUG

/* useful constants
  F_CPU - CPU frequency
 */

////////////////////////////////////////////
// TASK definition, to header file
////////////////////////////////////////////
class Task {
  public:
    // uint8_t taskNum;   // should a task know its own number?
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

////////////////////////////////////////////
// TaskManager definition, to header file
////////////////////////////////////////////
class TaskManager {
  public:
    TaskManager(uint8_t nTasks, uint32_t useconds);  // nTasks is the number of task you will have
    void attach( Task *task, uint8_t taskNum );
    void start();
    void stop();
//    void overflow();
    Task* tasks;
    uint8_t nTasks;
  private:
    uint32_t useconds;
};

TaskManager::TaskManager(uint8_t nTasks, uint32_t useconds) {
  nTasks = nTasks; // set to class variable
  useconds = useconds;
  tasks = (Task *) calloc(nTasks, sizeof(Task));
}

void TaskManager::attach(  Task *task, uint8_t taskNum ) {
  tasks[taskNum] = *task;
}

void TaskManager::start() {
   Timer1.initialize(useconds);
   Timer1.start();
   // Timer1.attachInterrupt( overflow );   // need to do something like this here...
}

void TaskManager::stop() {
  Timer1.stop();
}



////////////////////////////////////////////
// Testing demo functions
////////////////////////////////////////////
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



TaskManager TM = TaskManager(1, 10000); // 10000 useconds

Task tsk = Task(isr, 100);

void overflow() {
  uint8_t i;
  for (i=0; i<TM.nTasks; i++){
    TM.tasks[i]++; 
  }
}



void setup()
{
  TM.attach(&tsk, 0); // start at zero
  Timer1.attachInterrupt( overflow );  // don't want to have to manually do this

  pinMode(greenLEDpin, OUTPUT);   
  #ifdef SERIAL_DEBUG
    Serial.begin(19200);
  #endif
}

void loop()
{
}



#endif
