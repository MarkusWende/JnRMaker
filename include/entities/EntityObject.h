/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

 /**
  * @file EntityObject.h
  * @brief This file contains the object entity class.
  *
  * @author Markus Wende
  * https://github.com/MarkusWende
  */


#ifndef OBJECT_H
#define OBJECT_H

  /**
   * @brief Object class represents an general object.
   */
class Object
{
public:
    Object() { init(); }                                            //!< constructor
    ~Object() { };                                                  //!< destructor

private:
    GLvoid init() { };
	
};

#endif  // OBJECT_H
