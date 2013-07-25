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
#include "CBHelperDataCommandList.h"

namespace Cloudbase {

CBHelperDataCommandList::CBHelperDataCommandList() {

}

void CBHelperDataCommandList::addCommand(CBDataAggregationCommand* command) {
	this->commands.push_back(command);
}

std::string CBHelperDataCommandList::serialize() {
	std::string output = " { ";

	output += "\"cb_aggregate_key\" : [ ";

	for ( int i = 0; i < (int)this->commands.size(); i++ ) {
		CBDataAggregationCommand *curCommand = this->commands[i];
		output += " { \"" + curCommand->getCommandTypeString() + "\" : ";
		output += curCommand->serializeAggregateConditions();
		output += " } ";

		if ( i < (int)this->commands.size() - 1 )
			output += ", ";
	}

	output += " ] } ";

	//printf("Prepared aggregate request: %s", output.c_str());

	return output;
}

}
