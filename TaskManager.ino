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
  delay(500);
  ++tsk;
  delay(500);
  tsk++;
}



#endif
