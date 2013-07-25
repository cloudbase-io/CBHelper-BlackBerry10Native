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
#include "CBPayPal.h"
#include <sstream>

namespace Cloudbase {

CBPayPalBill::CBPayPalBill() {
	this->currency = "USD";
	this->paymentCompletedFunction = "";
	this->paymentCancelledFunction = "";
	this->paymentCompletedUrl = "";
	this->paymentCancelledUrl = "";
}

void CBPayPalBill::addItem(CBPayPalBillItem newItem) {
	this->items.push_back(newItem);
}

std::string CBPayPalBill::serialize() {
	std::string output = "";
	std::string itemsOutput = "";
	double fullAmount = 0.0f;

	for ( int i = 0; i < (int)this->items.size(); i++ ) {
		CBPayPalBillItem curItem = this->items[i];

		itemsOutput += curItem.serialize();

		if ( i < (int)this->items.size() - 1 ) {
			itemsOutput += ", ";
		}

		fullAmount = fullAmount + curItem.amount + (curItem.tax < 0 ? 0 : curItem.tax );
	}

	output += " { ";
	output += " \"name\" : \"" + this->name + "\", ";
	output += " \"description\" : \"" + this->description + "\", ";
	output += " \"amount\" : ";
	std::stringstream ssFullAmount;
	ssFullAmount << fullAmount;
	output += ssFullAmount.str();
	output += ", ";
	output += " \"invoice_number\" : \"" + this->invoiceNumber + "\", ";
	output += " \"items\" : [ " + itemsOutput + " ] } ";

	return output;
}

CBPayPalBillItem::CBPayPalBillItem() {
	this->tax = -1;
}

std::string CBPayPalBillItem::serialize() {
	std::string output = "";

	output += " { \"item_name\" : \"" + this->name + "\", ";
	output += " \"item_description\" : \"" + this->description + "\", ";
	output += " \"item_amount\" : ";
	std::stringstream ssAmount;
	ssAmount << this->amount;
	output += ssAmount.str();
	output += ", ";
	if ( this->tax > 0 ) {
		output += " \"item_tax\" : ";
		std::stringstream ssTax;
		ssTax << this->tax;
		output += ssTax.str();
		output += ", ";
	}
	output += " \"item_quantity\" : ";
	std::stringstream ssQuantity;
	ssQuantity << this->quantity;
	output += ssQuantity.str();
	output += " } ";

	return output;
}

}
