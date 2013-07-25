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
#include "CBDataAggregationCommandProject.h"

namespace Cloudbase {

CBDataAggregationCommandProject::~CBDataAggregationCommandProject() {
	this->includeFields.clear();
	this->excludeFields.clear();
}

CBDataAggregationCommandProject::CBDataAggregationCommandProject() {
	this->commandType = CBDataAggregationProject;
}

std::string CBDataAggregationCommandProject::serializeAggregateConditions() {
	std::string output = " { ";

	for ( int i = 0; i < (int)this->includeFields.size(); i++ ) {
		output += "\"" + this->includeFields[i] + "\" : 1";
		if ( i < (int)this->includeFields.size() - 1)
			output += ",";
	}

	for ( int i = 0; i < (int)this->excludeFields.size(); i++ ) {
		output += "\"" + this->excludeFields[0] + "\" : 0";
		if ( i < (int)this->excludeFields.size() - 1)
			output += ",";
	}

	output += " } ";

	return output;
}

}


