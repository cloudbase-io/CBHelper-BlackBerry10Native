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
