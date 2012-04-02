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

/* useful constants
  F_CPU - CPU frequency
 */

class Task {
  public:
    Task( uint32_t millisInterval );
    virtual Task& operator()() = 0;
  private:
    Task(const Task& task);               // Disable copy constructor
    int _interval;                        // How often it runs in milliseconds
};


class TaskManager {
  public:
    TaskManager( uint8_t numTasks, uint16_t window );
    ~TaskManager();
    bool add( Task& task );
    bool resize( uint8_t newNumTasks );
    bool start();
    bool stop();

  private:
    uint8_t            _numTasks;      //< Number of tasks to manage
    uint8_t            _numTasksAdded; //< Number of tasks that have been added
    Task*              _task;
    long unsigned int* _timeLastRan;
    const uint16_t     _window;        //< This is the time required for the 
                                       //  TaskManager to complete a loop (i.e. check
                                       //  if all tasks are scheduled to run within 
                                       //  the current time plus the window)
};

#endif
