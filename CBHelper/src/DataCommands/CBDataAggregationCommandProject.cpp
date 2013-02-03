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


