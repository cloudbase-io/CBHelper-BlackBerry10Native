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
#include "CBHttpConnection.h"

namespace Cloudbase {


CBHttpConnection::CBHttpConnection(std::string appCode, std::string appUniq, std::string password, std::string deviceUniq, std::string function) :
		isDownload(false) {
	this->appCode = appCode;
	this->appUniq = appUniq;
	this->password = password;
	this->deviceUniqueIdentifier = deviceUniq;
	this->function = function;
	this->currentLocation = NULL;
	this->CBResponder = NULL;
	this->parameters = NULL;

	this->authPassword = "";
	this->authUsername = "";
}

void CBHttpConnection::run() {
	this->sendRequest();
}

void CBHttpConnection::sendRequest() {
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);

	curl_formadd(&formpost,
				&lastptr,
				CURLFORM_COPYNAME, "app_uniq",
				CURLFORM_COPYCONTENTS, this->appUniq.c_str(),
				CURLFORM_END);
	curl_formadd(&formpost,
				&lastptr,
				CURLFORM_COPYNAME, "app_pwd",
				CURLFORM_COPYCONTENTS, this->password.c_str(),
				CURLFORM_END);
	curl_formadd(&formpost,
				&lastptr,
				CURLFORM_COPYNAME, "device_uniq",
				CURLFORM_COPYCONTENTS, this->deviceUniqueIdentifier.c_str(),
				CURLFORM_END);

	if ( this->authUsername != "" ) {
		curl_formadd(&formpost,
					&lastptr,
					CURLFORM_COPYNAME, "cb_auth_user",
					CURLFORM_COPYCONTENTS, this->authUsername.c_str(),
					CURLFORM_END);
		curl_formadd(&formpost,
					&lastptr,
					CURLFORM_COPYNAME, "cb_auth_password",
					CURLFORM_COPYCONTENTS, this->authPassword.c_str(),
					CURLFORM_END);
	}
	if ( this->currentLocation != NULL ) {
		std::string location = "";

		char latBuffer[32];
		snprintf(latBuffer, 32, "%g", this->currentLocation->coordinate().latitude());
		std::string stringValLat(latBuffer);
		location += "{ \"lat\" : \"" + stringValLat;

		char lngBuffer[32];
		snprintf(lngBuffer, 32, "%g", this->currentLocation->coordinate().longitude());
		std::string stringValLng(lngBuffer);

		location += "\", \"lng\" : \"" + stringValLng;
		location += "\" }";

		if (this->debugMode)
			qDebug("Sending current location: %s", location.c_str());

		curl_formadd(&formpost,
					&lastptr,
					CURLFORM_COPYNAME, "location_data",
					CURLFORM_COPYCONTENTS, location.c_str(),
					CURLFORM_END);
	}

	if ( parameters != NULL ) {
		if (this->debugMode)
			qDebug("Sending parameters: %s", parameters->serialize().c_str());

		curl_formadd(&formpost,
					&lastptr,
					CURLFORM_COPYNAME, "post_data",
					CURLFORM_COPYCONTENTS, parameters->serialize().c_str(),
					CURLFORM_END);
	}

	if ( !this->additionalPostParams.empty() ) {
		for(std::map<std::string, std::string>::iterator iter = this->additionalPostParams.begin();
				iter != this->additionalPostParams.end();
				iter++) {
			curl_formadd(&formpost,
						&lastptr,
						CURLFORM_COPYNAME, iter->first.c_str(),
						CURLFORM_COPYCONTENTS, iter->second.c_str(),
						CURLFORM_END);
		}
	}

	if (this->atts.size() > 0) {
		if (this->debugMode)
			qDebug("The request has %i attachments", (int)this->atts.size());
		for (int i = 0; i < (int)this->atts.size(); i++) {
			curl_formadd(&formpost,
						&lastptr,
						CURLFORM_COPYNAME, this->atts[i].fileName.c_str(),
						CURLFORM_FILE, this->atts[i].filePath.c_str(),
						CURLFORM_END);

		}
	}
	curl = curl_easy_init();
	headerlist = curl_slist_append(headerlist, buf);

	if ( curl ) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		if (this->isDownload) {
			const QDir home = QDir::home();
			QString fileIdString = QString::fromStdString(this->fileId);
			std::string filePath = home.absoluteFilePath(fileIdString).toStdString();
			this->downloadedFilePath = filePath;
			outfile = fopen(filePath.c_str(), "w");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CBHttpConnection::WriteFileDataCallback);
		} else {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CBHttpConnection::WriteDataCallback);
		}

		res = curl_easy_perform(curl);

		if ( res == CURLE_OK ) {
			this->parseResponseOutput(res, this->function);
		} else {
			qDebug("Error in HTTPS call %i", res);
			//qDebug(returnValue.c_str());
		}
		fclose(outfile);
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all (headerlist);
	}
}

size_t CBHttpConnection::writeData(void* ptr, size_t size, size_t nmemb) {
	size_t numOfBytes = size * nmemb;

	char *iter = (char*)ptr;
	char *iterEnd = iter + numOfBytes;

	responseOutputString += std::string(iter, iterEnd);

	return numOfBytes;
}

size_t CBHttpConnection::writeFileData(void *ptr, size_t size, size_t nmemb)
{
	return fwrite(ptr, size, nmemb, this->outfile);
}

void CBHttpConnection::parseResponseOutput(int statusCode, std::string function) {
	CBHelperResponseInfo resp;

	resp.httpStatusCode = statusCode;
	resp.function = function;

	if (this->isDownload) {
		resp.downloadedFileName = this->downloadedFilePath;
	} else {
		resp.outputString = this->responseOutputString;

		if (this->debugMode)
			qDebug("received: %s", this->responseOutputString.c_str());

		YAJLDom::Value* root = YAJLDom::parse(
				(const unsigned char*)responseOutputString.c_str(),
				responseOutputString.length());

		if (NULL == root || YAJLDom::Value::NUL == root->getType()) {
			resp.errorMessage = "Error while parsing JSON response";
		} else {
			YAJLDom::Value* functionData = root->getValueForKey(function);
			if (YAJLDom::Value::MAP != functionData->getType()) {
				resp.errorMessage = "Could not find function data in response";
			} else {
				resp.postSuccess = (functionData->getValueForKey("status")->toString() == "OK");
				resp.errorMessage = functionData->getValueForKey("error")->toString();
				resp.parsedMessage = functionData->getValueForKey("message");
			}
		}
	}

	emit requestCompleted(resp, this->CBResponder);
}

}

#include "CBHttpConnection.moc"
