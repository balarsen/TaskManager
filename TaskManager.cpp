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

#include <stdint.h>

#include "TaskManager.h"
#include <TimerOne.h>

#define SERIAL_DEBUG

////////////////////////////////////////////
// TASK definition, to header file
////////////////////////////////////////////
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


#endif
