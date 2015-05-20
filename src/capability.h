/* 3DInfo - Print information about your graphics hardware.
 *
 * 3DInfo is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * 3DInfo is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * 3DInfo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 3DInfo. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file capability.h
 *  Graphics hardware capability.
 */

#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <string>

enum Type {
	kTypeNone,
	kTypeBool,
	kTypeInt,
	kTypeString
};

struct Capability {
public:
	Capability();
	Capability(const Capability &cap);
	Capability(const std::string &label, bool vBool);
	Capability(const std::string &label, int vInt);
	Capability(const std::string &label, const std::string &vString);
	~Capability();

	Capability &operator=(const Capability &cap);

	void setLabel(const std::string &label);
	const std::string &getLabel() const;

	Type getType() const;

	void clear();

	void setValue(bool vBool);
	void setValue(int vInt);
	void setValue(const std::string &vString);

	bool getBool() const;
	int getInt() const;
	std::string getString() const;

private:
	std::string _label;
	Type _type;
	union {
		bool _valueBool;
		int _valueInt;
		char *_valueString;
	};
};

#endif // CAPABILITY_H
