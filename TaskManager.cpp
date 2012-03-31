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

#include "TaskManager.h"

#define SERIAL_DEBUG

////////////////////////////////////////////
// TASK definition, to header file
////////////////////////////////////////////
Task::Task( uint32_t millisInterval ) {
  #ifdef SERIAL_DEBUG
    Serial.println("in constructor");
  #endif
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
