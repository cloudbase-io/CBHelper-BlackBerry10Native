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
#include "CBHelperAttachment.h"
#include "yajl/YAJLDom.h"


namespace Cloudbase {

#ifndef CBRESPONSEINFO_H_
#define CBRESPONSEINFO_H_

/**
 * A container for the data returned from the cloudbase.io server. This is handed over as a representation
 * of a response to objects implementing the CBHelperResponder interface.
 */
class CBHelperResponseInfo {
public:
	std::string function; /// The function from cloudbase.io (data/notifications/CloudFunction/Applet etc)
	std::string errorMessage; /// An error message, if any, returned by cloudbase.io
	std::string outputString; /// The full String representation of the response from the cloudbase.io servers
	std::string downloadedFileName; /// The full path to the temporary file downloaded by the file download cloudbase.io APIs
	YAJLDom::Value* parsedMessage; /// The parsed JSON message received in the response
	int httpStatusCode; /// The status code for the call
	bool postSuccess; /// Whether the request was successfull
};

#endif

#ifndef CBRESPONDER_H_
#define CBRESPONDER_H_

class CBHelperResponder {
public:
	/**
	* Called when a connection to the cloudbase.io APIs is completed and the data is ready.
	* @param respData A CBHelperResponseInfo object populated with the data from the API request
	* @param result \> 0 on success,
	* or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	*/
	virtual void parseResponse(CBHelperResponseInfo resp) { return; };

	virtual ~CBHelperResponder(void) {}
};

#endif

}
