/* Copyright (C) 2012 cloudbase.io

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
#include "CBDataAggregationCommandGroup.h"

namespace Cloudbase {

const std::string CBDataAggregationCommandGroup::CBDataAggregationGroupOperator_ToString[] = {
	"$sum",
	"$max",
	"$min",
	"$avg"
};

CBDataAggregationCommandGroup::~CBDataAggregationCommandGroup() {
	this->idFields.clear();
	this->groupFields.clear();
}

CBDataAggregationCommandGroup::CBDataAggregationCommandGroup() {
	this->commandType = CBDataAggregationGroup;
}

void CBDataAggregationCommandGroup::addOutputField(std::string fieldName) {
	//String finalName = "$" + fieldName;
	this->idFields.push_back(fieldName);
}

void CBDataAggregationCommandGroup::addGroupFormulaForField(std::string outputField, CBDataAggregationGroupOperator op, std::string fieldName) {
	std::string finalFieldName = "$" + fieldName;
	this->addGroupFormulaForValue(outputField, op, finalFieldName);
}

void CBDataAggregationCommandGroup::addGroupFormulaForValue(std::string outputField, CBDataAggregationGroupOperator op, std::string groupValue) {
	std::string output = "";

	output += "\"" + outputField + "\" : { ";
	output += "\"" + this->CBDataAggregationGroupOperator_ToString[op] + "\" : \"";
	output += groupValue + "\" } ";

	this->groupFields.push_back(output);
}

std::string CBDataAggregationCommandGroup::serializeAggregateConditions() {
	std::string output = " { ";

	output += " \"_id\" : ";
	if ( this->idFields.size() > 1 ) {
		output += " { ";
		for ( int i = 0; i < (int)this->idFields.size(); i++ ) {
			output += " \"" + this->idFields[i] + "\" : \"$";
			output += this->idFields[i] + "\"";

			if ( i < (int)this->idFields.size() - 1 )
				output += ",";
		}
		output += " } ";
	} else {
		if ( this->idFields.size() > 0 )
			output += "\"$" + this->idFields[0] + "\"";
	}

	output += ",";

	for ( int i = 0; i < (int)this->groupFields.size(); i++ ) {
		output += this->groupFields[i];

		if ( i < (int)this->groupFields.size() - 1 )
			output += ",";
	}

	output += " } ";

	return output;
}

}
