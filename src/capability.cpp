/* 3DInfo - Print information about your graphics hardware.
 *
 * 3DInfo is the legal property of its developers, whose labels can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** @file capability.cpp
 *  Graphics hardware capability.
 */

#include <cassert>
#include <cstring>

#include "capability.h"

Capability::Capability() : _type(kTypeNone) {
}

Capability::Capability(const Capability &cap) : _type(kTypeNone) {
	*this = cap;
}

Capability::Capability(const std::string &label, bool vBool) : _label(label), _type(kTypeNone) {
	setValue(vBool);
}

Capability::Capability(const std::string &label, int vInt) : _label(label), _type(kTypeNone) {
	setValue(vInt);
}

Capability::Capability(const std::string &label, const std::string &vString) : _label(label), _type(kTypeNone) {
	setValue(vString);
}

Capability::~Capability() {
	clear();
}

Capability &Capability::operator=(const Capability &cap) {
	_label = cap._label;

	if      (cap._type == kTypeBool)
		setValue(cap.getBool());
	else if (cap._type == kTypeInt)
		setValue(cap.getInt());
	else if (cap._type == kTypeString)
		setValue(cap.getString());

	return *this;
}

void Capability::setLabel(const std::string &label) {
	_label = label;
}

Type Capability::getType() const {
	return _type;
}

const std::string &Capability::getLabel() const {
	return _label;
}

void Capability::clear() {
	if (_type == kTypeString)
		delete[] _valueString;

	_type = kTypeNone;
}

void Capability::setValue(bool vBool) {
	clear();

	_valueBool = vBool;
	_type      = kTypeBool;
}

void Capability::setValue(int vInt) {
	clear();

	_valueInt = vInt;
	_type     = kTypeInt;
}

void Capability::setValue(const std::string &vString) {
	clear();

	_valueString = new char[vString.size() + 1];
	strcpy(_valueString, vString.c_str());

	_type = kTypeString;
}

bool Capability::getBool() const {
	assert(_type == kTypeBool);

	return _valueBool;
}

int Capability::getInt() const {
	assert(_type == kTypeInt);

	return _valueInt;
}

std::string Capability::getString() const {
	assert(_type == kTypeString);

	return _valueString;
}
