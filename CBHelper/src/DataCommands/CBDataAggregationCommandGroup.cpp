/* Copyright (c) 2013 Cloudbase.io Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
