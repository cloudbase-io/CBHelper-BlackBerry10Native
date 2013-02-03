/* Copyright (C) 2011 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/*
 * YAJLDom.cpp
 *
 *  Created on: Jan 3, 2011
 *  Author: Niklas Nummelin
 *  History not in version control system:
 *   Updated by Mikael Kindborg, Dec 5, 2011
 */
#include "YAJLDom.h"
#include "api/yajl_parse.h"
#include "api/yajl_gen.h"


// Uncomment to disable memory tracker.
/*
#undef deleteobject
#undef newobject
#define deleteobject(ptr) delete ptr
#define newobject(type, code) code
*/

namespace YAJLDom {

static NullValue sNullValue;

Value::Value(Type type) :
	mType(type) {
}

Value::~Value() {
}

bool Value::isNull() const {
	return mType == Value::NUL;
}

Value::Type Value::getType() const {
	return mType;
}

bool Value::toBoolean() const {
	std::string ret = toString();
	if (ret == "true")
		return true;
	if (ret == "false")
		return false;
	else
		return false; //maPanic(1, "Not a boolean value!");
}

int Value::toInt() const {
	return atoi(toString().c_str());
}

double Value::toDouble() const {
	return atof(toString().c_str());
}

Value* Value::getValueForKey(const std::string& key) {
	return &sNullValue;
}

Value* Value::getValueByIndex(int i) {
	return &sNullValue;
}

const Value* Value::getValueForKey(const std::string& key) const {
	return &sNullValue;
}

const Value* Value::getValueByIndex(int i) const {
	return &sNullValue;
}

int Value::getNumChildValues() const {
	return 0;
}

NullValue::NullValue() :
	Value(NUL) {
}

std::string NullValue::toString() const {
	return "";
}

BooleanValue::BooleanValue(bool value) :
	Value(BOOLEAN), mValue(value) {

}

std::string BooleanValue::toString() const {
	if (mValue == true)
		return "true";
	else
		return "false";
}

bool BooleanValue::toBoolean() const {
	return mValue;
}

void BooleanValue::setBoolean(bool value) {
	mValue = value;
}

NumberValue::NumberValue(double num) :
	Value(NUMBER), mValue(num) {
}

std::string NumberValue::toString() const {
	std::string retValue = "";
	retValue += mValue;
	return retValue;
	//return doubleToString(mValue);
}

int NumberValue::toInt() const {
	return (int) mValue;
}

double NumberValue::toDouble() const {
	return mValue;
}

StringValue::StringValue(const char* str, size_t length) :
	Value(STRING), mValue(str, length) {
}

StringValue::StringValue(const std::string& str) :
	Value(STRING), mValue(str) {
}

std::string StringValue::toString() const {
	return mValue;
}

MapValue::MapValue() :
	Value(MAP) {
}

MapValue::~MapValue() {
	std::map<std::string, Value*>::iterator iter = mMap.begin();
	while (iter != mMap.end()) {
		deleteobject(iter->second);
		iter++;
	}

	mMap.clear();
}

void MapValue::setValueForKey(const std::string& key, Value* value) {
	mMap.insert(std::make_pair<std::string, Value*>(key, value));//[key] = value;
}

Value* MapValue::getValueForKey(const std::string& key) {
	std::map<std::string, Value*>::const_iterator iter = mMap.find(key);
	if (iter != mMap.end())
		return iter->second;
	else
		return &sNullValue;
}

const Value* MapValue::getValueForKey(const std::string& key) const {
	std::map<std::string, Value*>::const_iterator iter = mMap.find(key);
	if (iter != mMap.end())
		return iter->second;
	else
		return &sNullValue;
}

std::string MapValue::toString() const {
	std::string ret = "{";

	std::map<std::string, Value*>::const_iterator iter = mMap.begin();

	while (iter != mMap.end()) {
		ret += "\"" + iter->first + "\": ";

		Value *value = iter->second;
		bool isString = value->getType() == Value::STRING;
		if (isString)
			ret += "\"";
		if (value->getType() == Value::NUL)
			ret += "null";
		else
			ret += value->toString();
		if (isString)
			ret += "\"";

		iter++;

		if (iter != mMap.end())
			ret += ", ";
	}
	ret += "}";
	return ret;
}

ArrayValue::ArrayValue() :
	Value(ARRAY) {
}

ArrayValue::~ArrayValue() {
	for (int i = 0; i < (int)mValues.size(); i++) {
		deleteobject(mValues[i]);
	}
}

void ArrayValue::addValue(Value* value) {
	//mValues.add(value);
	mValues.push_back(value);
}

const std::vector<Value*>& ArrayValue::getValues() const {
	return mValues;
}

int ArrayValue::getNumChildValues() const {
	return mValues.size();
}


std::string ArrayValue::toString() const {
	std::string ret = "[";
	for (int i = 0; i < (int)mValues.size(); i++) {
		Value *value = mValues[i];
		bool isString = value->getType() == Value::STRING;
		if (isString)
			ret += "\"";
		ret += value->toString();
		if (isString)
			ret += "\"";

		if (i != (int)mValues.size() - 1)
			ret += ", ";
	}
	ret += "]";
	return ret;
}

Value* ArrayValue::getValueByIndex(int i) {
	if (i < 0 || i >= (int)mValues.size())
		return &sNullValue;
	return mValues[i];
}

const Value* ArrayValue::getValueByIndex(int i) const {
	if (i < 0 || i >= (int)mValues.size())
		return &sNullValue;
	return mValues[i];
}

Value *sRoot = NULL;
Stack<Value*> sValueStack;

struct KeyString {
	KeyString() {}
	KeyString(const char* s, int len) : str(s), length(len) {
	}
	const char* str;
	int length;
};

Stack<KeyString> sKeyStack;

Value* validateValue(Value* value, Value::Type type) {
	if (value->getType() != type)
		return &sNullValue;
	else
		return value;
}

void printValue(Value* value) {

	if(!value) {
		qDebug("NULL pointer!!\n");
		return;
	}

	switch(value->getType()) {
		case Value::NUL: qDebug("NUL\n"); break;
		case Value::BOOLEAN: qDebug("BOOLEAN"); break;
		case Value::NUMBER: qDebug("NUMBER\n"); break;
		case Value::ARRAY: qDebug("ARRAY\n"); break;
		case Value::MAP: qDebug("MAP\n"); break;
		case Value::STRING: qDebug("STRING\n"); break;
	}
}

void pushValue(Value *value) {
	Value* parent;

	if (value == NULL) {
		qDebug("YAJLDom::pushValue, value is null.");
		return;
	}


	bool isContainer = (value->getType() == Value::MAP || value->getType()
			== Value::ARRAY);

	// this must be the first item (i.e. sRoot is NULL).
	if(sValueStack.size()==0)
	{
		// if that is case we set it as the root and if it's a container, we push it to the stack.
		if (sRoot == NULL)
		{
			sRoot = value;
			if (isContainer)
				sValueStack.push(value);
			return;
		}
		else
		{
			qDebug("YAJLDom::pushValue, sValueStack.size() is 0.");
		}
	}
	parent = sValueStack.peek();

	if (parent == NULL)
		qDebug("YAJLDom::pushValue, parent is null.");

	switch (parent->getType())
	{
		case Value::MAP:
		{
			MapValue* map = (MapValue*) validateValue(parent, Value::MAP);
			const KeyString& key = sKeyStack.peek();
			map->setValueForKey(std::string(key.str, key.length), value);
		}
		break;

		case Value::ARRAY:
		{
			ArrayValue* array = (ArrayValue*) validateValue(parent, Value::ARRAY);
			array->addValue(value);
		}
		break;

		default:
		{
			if (sRoot == NULL) {
				sRoot = value;
				break;
			}
			qDebug("Invalid state!");
		}
		break;
	}

	if (isContainer)
		sValueStack.push(value);
}

void popValue() {
	sValueStack.pop();
}

static int parse_null(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_null(g);
	pushValue(newobject(NullValue, new NullValue()));
	return 1;
}

static int parse_boolean(void * ctx, int boolean) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_bool(g, boolean);
	pushValue(newobject(BooleanValue, new BooleanValue((bool) boolean)));
	return 1;
}

static int parse_number(void * ctx, const char * s, unsigned int l) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_number(g, s, l);
	pushValue(newobject(NumberValue, new NumberValue(atof(std::string(s, l).c_str()))));
	return 1;
}

static int parse_string(void * ctx, const unsigned char * stringVal,
		unsigned int stringLen) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_string(g, stringVal, stringLen);
	pushValue(newobject(StringValue, new StringValue(std::string((const char*) stringVal, stringLen))));
	return 1;
}

static int parse_map_key(void * ctx, const unsigned char * stringVal,
		unsigned int stringLen) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_string(g, stringVal, stringLen);
	sKeyStack.push(KeyString((const char*) stringVal, stringLen));
	return 1;
}

static int parse_start_map(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_map_open(g);
	pushValue(newobject(MapValue, new MapValue()));
	return 1;
}

static int parse_end_map(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_map_close(g);
	popValue();
	return 1;
}

static int parse_start_array(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_array_open(g);
	pushValue(newobject(ArrayValue, new ArrayValue()));
	return 1;
}

static int parse_end_array(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_array_close(g);
	popValue();
	return 1;
}

static yajl_callbacks callbacks = { parse_null, parse_boolean, NULL, NULL,
		parse_number, parse_string, parse_start_map, parse_map_key,
		parse_end_map, parse_start_array, parse_end_array };

void parseError(yajl_handle hand, int verbose, const unsigned char* jsonText,
		size_t jsonTextLength) {
	unsigned char * str = yajl_get_error(hand, 1, jsonText, jsonTextLength);
	yajl_free_error(hand, str);
}

void gen_print(void *ctx, const char *str, unsigned int len) {
	printf("%.*s", len, str);
}

Value* parse(const unsigned char* jsonText, size_t jsonTextLength) {
	//qDebug("parsing %s", jsonText);
	yajl_handle hand;
	yajl_gen_config conf = { 1, "  " };
	yajl_gen g;
	yajl_status stat;
	yajl_parser_config cfg = { 1, 1 };

	//conf.beautify = 0;

	// enable this if it should parse utf-8?
	cfg.checkUTF8 = 1;

	g = yajl_gen_alloc(&conf, NULL);
	//g = yajl_gen_alloc2(gen_print, &conf, NULL, NULL);

	hand = yajl_alloc(&callbacks, &cfg, NULL, (void *) g);

	sValueStack.clear();
	sKeyStack.clear();
	sRoot = NULL;

	/* read file data, pass to parser */
	stat = yajl_parse(hand, jsonText, jsonTextLength);

	if (stat != yajl_status_ok && stat != yajl_status_insufficient_data) {
		parseError(hand, 1, jsonText, jsonTextLength);
		yajl_gen_free(g);
		yajl_free(hand);
		return NULL;
	}

	stat = yajl_parse_complete(hand);

	yajl_gen_free(g);

	if (stat != yajl_status_ok && stat != yajl_status_insufficient_data) {
		parseError(hand, 1, jsonText, jsonTextLength);
		yajl_free(hand);
		return NULL;
	}

	yajl_free(hand);

	return sRoot;
}

void deleteValue(Value* value) {
	if(!value || value == &sNullValue) return;
	deleteobject(value);
	//MAPUtil::MemoryMgr::dump();
}

} // namespace YAJLDom

