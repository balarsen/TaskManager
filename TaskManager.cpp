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

// C includes

// C++ includes
#include <cstdlib>

// TPL includes
#include <arduino.h>

// Our includes
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

/**
 *  \fn         Constructor
 *  \brief      Grabs memory for the number of tasks requested
 *  \param[in]  Number of tasks to manage/alloc memory
 *  \param[in]  If a task is within this time window, run it
 *              This prevents the task from getting skipped
 *  \author     Brian Larsen, Nathan Barnett
 */
TaskManager::TaskManager( uint8_t numTasks, uint16_t window=100 ) : 
    _numTasks(numTasks), _numTasksAdded(0), _window(window) {
  _task         = (Task*) calloc( numTasks, sizeof(Task) );
  _timeLastRan = (long unsigned int*) calloc( numTasks, sizeof(long unsigned int) );
}

/**
 *  \fn         Destructor
 *  \brief      Frees some memory
 *  \author     Brian Larsen, Nathan Barnett
 */
TaskManager::~TaskManager() {
  free( _task );
  free( _timeLastRan );
}

/**
 *  \fn         add
 *  \brief      Adds a task to the list of managed tasks
 *  \param[in]  Reference to an existing task
 *  \returns    True if successful
 *  \author     Brian Larsen, Nathan Barnett
 */
bool TaskManager::add( Task& task ) {
  if ( _numTasksAdded == _numTasks-1 )
    return false;
  _task[_numTasksAdded] = task;
  ++_numTasksAdded;
  return true;
}

/**
 *  \fn         start
 *  \brief      Starts the clock and tasks
 *  \returns    True if successful
 *  \author     Brian Larsen, Nathan Barnett
 */
bool TaskManager::start() {
  Timer1.initialize(useconds);
  Timer1.start();
  // Timer1.attachInterrupt( overflow );   // need to do something like this here...
  return true;
}

/**
 *  \fn         stop
 *  \brief      Stops the timer and all of the tasks
 *  \returns    True if successful
 *  \author     Brian Larsen, Nathan Barnett
 */
bool TaskManager::stop() {
  Timer1.stop();
  return true;
}

/**
 *  \fn         resize
 *  \brief      Reallocates the memory.
 *
 *              Lookup realloc() for more info.
 *  \param[in]  The new number of tasks to manage
 *  \returns    True if successful
 *  \author     Brian Larsen, Nathan Barnett
 */
bool TaskManager::resize( uint8_t newNumTasks ) {
  Task* tmpTask                 = NULL;
  long unsigned int* tmpLastRan = NULL;
  tmpTask    = (Task*) realloc( _task, newNumTasks*sizeof(Task) );
  tmpLastRan = (long unsigned int*) realloc( _timeLastRan, newNumTasks*sizeof(long unsigned int) );
  
  _numTasks = newNumTasks;
  // If the memory allocation succeeded...
  if ( (tmpTask != NULL) && (tmpLastRan != NULL) ) {
    _task        = tmpTask;
    _timeLastRan = tmpLastRan;
    // If we are losing management of some tasks
    if ( newNumTasks <= _numTasksAdded )
      _numTasksAdded = newNumTasks-1;
    return true;
  // If reallocation failed, return false (but we still have original memory)
  } else {
    return false;
  }
}

#endif
