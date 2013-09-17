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
#include "CBHelperSearchCondition.h"

namespace Cloudbase {

const std::string CBHelperSearchCondition::CBConditionOperator_ToString[] = {
		"",
		"$lt",
		"$lte",
		"$gt",
		"$gte",
		"$all",
		"$exists",
		"$mod",
		"$ne",
		"$in",
		"$nin",
		"$size",
		"$type",
		"$within",
		"$near"
	};

const std::string CBHelperSearchCondition::CBConditionLink_ToString[] = {
		"$and",
		"$or",
		"$nor"
	};

const std::string CBHelperSearchCondition::CBSortDirection_ToString[] = {
		"1",
		"-1"
	};


CBHelperSearchCondition::CBHelperSearchCondition() {
	this->baseInit();
}
CBHelperSearchCondition::CBHelperSearchCondition(std::string field, std::string value, CBConditionOperator op) {
	this->baseInit();

	this->field_ = field;
	this->value_ = value;
	this->conditionOperator_ = op;
}

CBHelperSearchCondition::CBHelperSearchCondition(double SWLat, double SWLng, double NELat, double NELng) {
	this->baseInit();

	this->field_ = "cb_location";
	this->value_ = "[ [ ";
	this->value_ += SWLat;
	this->value_ += ", ";
	this->value_ += SWLng;
	this->value_ += "], [ ";
	this->value_ += NELat;
	this->value_ += ", ";
	this->value_ += NELng;
	this->value_ += "] ] ";
	this->conditionOperator_ = CBOperatorWithin;
}
CBHelperSearchCondition::CBHelperSearchCondition(double lat, double lng, double maxDistance) {
	this->baseInit();

	this->field_ = "cb_location";
	this->value_ = "{ \"$near\" : [ ";
	this->value_ +=  lat;
	this->value_ +=  ", ";
	this->value_ += lng;
	this->value_ += "], \"$maxDistance\" : ";
	this->value_ += maxDistance;
	this->value_ += " }";
	this->conditionOperator_ = CBOperatorNear;

}

void CBHelperSearchCondition::setConditionLink(CBConditionLink link) {
	this->contidionLink_ = link;
}

void CBHelperSearchCondition::addCondition(CBHelperSearchCondition* newCond) {
	this->conditions_.insert(this->conditions_.begin(), newCond);
}

void CBHelperSearchCondition::baseInit() {
	this->field_ = "";
	this->value_ = "";
	this->limit = -1;
	this->offset = -1;
	this->commandType = CBDataAggregationMatch;
	this->contidionLink_ = CBConditionLinkAnd;
}

void CBHelperSearchCondition::addSortField(std::string fieldName, CBSortDirection dir) {
	std::string sortField = "{ \"";
	sortField += fieldName + "\" : ";
	sortField += CBSortDirection_ToString[dir] + " }";
	this->sortFields_.insert(this->sortFields_.begin(), sortField);
}

std::string CBHelperSearchCondition::serializeAggregateConditions() {
	return this->serialize(this, true);
}

std::string CBHelperSearchCondition::serialize() {
	std::string output = " { ";
	if (!this->sortFields_.empty()) {
		output += "\"cb_sort_key\" : [";
		for (int i = 0; i < (int)this->sortFields_.size(); i++) {
			output += this->sortFields_[i];
			if (i < (int)this->sortFields_.size() - 1)
				output += ", ";
		}
		output += " ], ";
	}

	if (this->limit > 0) {
		output += "\"cb_limit\" : " + this->limit;
		output += ", ";
	}

	if (this->offset > 0) {
		output += "\"cb_offset\" : " + this->offset;
		output += ", ";
	}
	output += "\"cb_search_key\" : ";

	output += this->serialize(this, true);

	output += " } ";

	return output;
}

std::string CBHelperSearchCondition::serialize(CBHelperSearchCondition* cond, bool isTop) {
	std::string output = "{ ";

	if (cond->field_ == "") {
		if (!cond->conditions_.empty()) {
			int prevLink = -1;
			std::string curCond = "";

			for (int i = 0; i < (int)cond->conditions_.size(); i++) {
				CBHelperSearchCondition* curGroup = cond->conditions_[i];
				if (prevLink != -1 && prevLink != curGroup->contidionLink_) {
					std::string linkStr = cond->CBConditionLink_ToString[prevLink];
					output += "\"" + linkStr + "\" :  " + curCond + ", ";
					curCond = "";
				}
				std::string tmpCond = curGroup->serialize(curGroup, false);
				curCond += tmpCond;
				if ( i < cond->conditions_.size() - 1 ) {
					curCond += ", ";
				}
				prevLink = curGroup->contidionLink_;
				if ( i == (cond->conditions_.size() -1 ) ) {
				    //[output setValue:curObject forKey:CBConditionLink_Tostd::string[prevLink]];
					std::string linkStr = cond->CBConditionLink_ToString[prevLink];
					output += "\"" + linkStr + "\" : [ " + curCond + " ]";
				}
			}
		}
	} else {
		std::string op = cond->CBConditionOperator_ToString[cond->conditionOperator_];
		switch (cond->conditionOperator_) {
		case CBOperatorEqual:
			output += "\"" + cond->field_ + "\"";
			output += " : \"" + cond->value_ + "\"";
			break;
		case CBOperatorAll:
		case CBOperatorExists:
		case CBOperatorNe:
		case CBOperatorIn:
		case CBOperatorBigger:
		case CBOperatorBiggerOrEqual:
		case CBOperatorLess:
		case CBOperatorLessOrEqual:
		case CBOperatorNin:
		case CBOperatorSize:
		case CBOperatorType:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" : \"" + cond->value_ + "\" } ";
		                //[cond setValue:conditionsGroup.value forKey:CBConditionOperator_Tostd::string[conditionsGroup.CBOperator]];
		                //[output setValue:cond forKey:conditionsGroup.field];
			break;
		case CBOperatorMod:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" : [ " + cond->value_ + ", 1 ] } ";
			break;
		case CBOperatorWithin:
			output += "\"" + cond->field_ + "\"";
			output += " : { \"" + op + "\" :  { \"$box\" : " + cond->value_ + " } } ";
			break;
		case CBOperatorNear:
			output += "\"" + cond->field_ + "\"";
			output += " : " + cond->value_;
			break;
		default:
			break;
		}
	}

	output += " }";

	return output;
}

}
