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
  * @file entity_object.h
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

	friend class cereal::access;
	template <class Archive>
	void save(Archive& ar, std::uint32_t const version)
	{
		
	}

	template <class Archive>
	void load(Archive& ar, std::uint32_t const version)
	{

	}
	
};

CEREAL_CLASS_VERSION(Object, 1)

#endif  // OBJECT_H
