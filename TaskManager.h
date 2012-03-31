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
#ifndef TASKMANAGER_h
#define TASKMANAGER_h

#include <stdint.h>
#include <arduino.h>

/* useful constants
  F_CPU - CPU frequency
 */

class Task {
  public:
    Task( uint32_t millisInterval );
    virtual Task& operator()() = 0;
  private:
    int _interval; // How often it runs in milliseconds
};


class TaskManager {
  public:
    TaskManager(uint8_t nTasks);  // nTasks is the number of task you will have
    bool add( Task& task );
    bool start();
    bool stop();

  private:
    uint8_t nTasks;
    Task* task;
    long int* lastRan;
    static const _window = 100; //< This is the time required for the 
                                //  TaskManager to complete a loop (i.e. check
                                //  if all tasks are scheduled to run within 
                                //  the current time plus the window)
};

#endif
