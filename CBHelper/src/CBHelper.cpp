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
#include "CBHelper.h"

namespace Cloudbase {

CBHelper::CBHelper(std::string appCode, std::string appUniq) {
	this->baseInit();
	this->appCode = appCode;
	this->appUniq = appUniq;
}

void CBHelper::baseInit() {
	this->isHttps				= true;
	this->deviceRegistered		= false;
	this->debugMode				= false;
	this->domain				= "api.cloudbase.io";
	this->defaultLogCategory	= "DEFAULT";
	this->language				= "";
	this->deviceUniqueIdentifier= "";

	this->authUsername			= "";
	this->authPassword			= "";

	this->sessionId				= "";

	char *language;
	char *country;
	int getLocale = locale_get(&language, &country);
	this->language = language;
	this->country = country;

	bb::device::HardwareInfo *bbInfo = new bb::device::HardwareInfo(0);
	this->deviceUniqueIdentifier = bbInfo->serialNumber().toStdString();
	this->deviceModel = bbInfo->deviceName().toStdString() + " - " + bbInfo->modelName().toStdString();
	this->deviceVersion = bbInfo->modelNumber().toStdString();

	this->notificationNetwork = "bb";
	this->currentLocation = NULL;
}

void CBHelper::registerDevice() {
	if (debugMode)
		qDebug("called register device");

	if (this->deviceRegistered)
		return;
	CBHttpConnection* con = this->createConnection("register-device");

	CBRegisterDevice* params = new CBRegisterDevice(this->notificationNetwork, this->deviceModel, this->deviceVersion, this->language, this->country);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/register";

	CBRegisterDeviceResponder* resp = new CBRegisterDeviceResponder(this);
	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(params);
	con->CBResponder = resp;
	con->start();
	//con->sendRequest(url, dynamic_cast<CBSerializable*>(params), resp);
	this->deviceRegistered = true;
}

void CBHelper::writeLog(std::string line, std::string category, std::string level) {
	CBHttpConnection* con = this->createConnection("log");

	CBLogLine* params = new CBLogLine(category, level, line, this->deviceModel, this->deviceVersion);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/log";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(params);
	con->start();
}

void CBHelper::logNavigation(std::string screenName) {
	if (this->sessionId == "")
		return;

	CBHttpConnection* con = this->createConnection("log-navigation");

	CBNavigation* params = new CBNavigation(this->sessionId, screenName);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/lognavigation";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(params);
	con->start();
}

void CBHelper::insertDocument(std::string collectionName, CBSerializable* data, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/insert";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(data);
	con->CBResponder = responder;
	con->start();
}
void CBHelper::insertDocument(std::string collectionName, CBSerializable* data, std::vector<CBHelperAttachment> atts, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");
	con->atts = atts;

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/insert";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(data);
	con->CBResponder = responder;
	con->start();
}
void CBHelper::searchDocument(std::string collectionName, CBHelperSearchCondition* conditions, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/search";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(conditions);
	con->CBResponder = responder;
	con->start();
}
void CBHelper::searchDocumentAggregate(std::string collectionName, CBHelperDataCommandList* commands, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("data");

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/" + collectionName;
	url += "/aggregate";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(commands);
	con->CBResponder = responder;
	con->start();
}

void CBHelper::downloadFile(std::string fileId, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("download");
	con->isDownload = true;

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/file/" + fileId;

	con->url = url;
	con->fileId = fileId;
	con->CBResponder = responder;
	con->start();

}

void CBHelper::subscribeDeviceWithToken(std::string token, std::string channel, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications-register");
	std::string network = "bb";
	CBPushNotification* notif = new CBPushNotification("subscribe", token, channel, network);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications-register";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(notif);
	con->CBResponder = responder;
	con->start();
}
void CBHelper::unsubscribeDeviceWithToken(std::string token, std::string channel, bool fromAll, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications-register");
	std::string network = "bb";
	CBPushNotification* notif = new CBPushNotification("unsubscribe", token, channel, network);
	notif->setFromAll(fromAll);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications-register";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(notif);
	con->CBResponder = responder;
	con->start();
}
void CBHelper::sendNotification(std::string channel, std::string certType, std::string notification, std::string badge, std::string sound, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("notifications");

	CBPushNotificationMessage* notif = new CBPushNotificationMessage(channel, certType, notification, badge, sound);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/notifications";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(notif);
	con->CBResponder = responder;
	con->start();
}

void CBHelper::sendNotificationEmail(std::string templateCode, std::string recipient, std::string subject, CBSerializable* vars) {
	CBHttpConnection* con = this->createConnection("email");

	CBNotificationEmail* notif = new CBNotificationEmail(templateCode, recipient, subject, vars);

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/email";

	con->url = url;
	con->parameters = dynamic_cast<CBSerializable*>(notif);
	con->start();
}
void CBHelper::executeCloudFunction(std::string functionCode, std::map<std::string, std::string> params, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("cloudfunction");
	con->additionalPostParams = params;

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/cloudfunction/";
	url += functionCode;

	con->url = url;
	con->CBResponder = responder;
	con->start();
}
void CBHelper::executeApplet(std::string appletCode, std::map<std::string, std::string> params, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("applet");
	con->additionalPostParams = params;

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/applet/";
	url += appletCode;

	con->url = url;
	con->CBResponder = responder;
	con->start();
}
void CBHelper::preparePayPalPurchase(CBPayPalBill purchaseDetails, bool isLiveEnvironment, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("paypal");

	std::string url = this->generateURL();
	url += "/" + this->appCode;
	url += "/paypal/prepare";

	CBPayPalPurchase *purchase = new CBPayPalPurchase(purchaseDetails, isLiveEnvironment, "purchase");

	con->url = url;
	con->CBResponder = responder;
	con->parameters = dynamic_cast<CBSerializable*>(purchase);
	con->start();
}

void CBHelper::completePayPalPurchase(std::string url, CBHelperResponder* responder) {
	CBHttpConnection* con = this->createConnection("paypal");

	con->url = url;
	con->CBResponder = responder;
	con->start();
}

void CBHelper::setPassword(std::string pwd) {
	this->password = pwd;
	this->registerDevice();
}

void CBHelper::setAuthentication(std::string username, std::string password) {
	this->authUsername = username;
	this->authPassword = password;
}

CBHttpConnection* CBHelper::createConnection(std::string function) {
	emit requestStarted(function);

	CBHttpConnection* con = new CBHttpConnection(this->appCode, this->appUniq, this->password, this->deviceUniqueIdentifier, function);

	if (this->authUsername != "") {
		con->authUsername = this->authUsername;
		con->authPassword = this->authPassword;
	}
	con->debugMode = this->debugMode;

	qRegisterMetaType< CBHelperResponder* >("CBHelperResponder*");
	qRegisterMetaType< CBHelperResponseInfo >("CBHelperResponseInfo");
	QObject::connect(con, SIGNAL(requestCompleted(CBHelperResponseInfo, CBHelperResponder*)), this, SLOT(handleRequestCompleted(CBHelperResponseInfo, CBHelperResponder*)));

	return con;
}

std::string CBHelper::generateURL() {
	return (this->isHttps?"https":"http") + (std::string)"://" + this->domain;
}

void CBHelper::handleRequestCompleted(CBHelperResponseInfo resp, CBHelperResponder* responder) {
	emit requestCompleted(resp.httpStatusCode);

	if (responder != NULL) {
		responder->parseResponse(resp);
	} else {
		if (this->debugMode)
			qDebug("Received response but no responder to parse it");
	}
}

CBRegisterDeviceResponder::CBRegisterDeviceResponder(CBHelper* helperInstance) { this->helper = helperInstance; }
void CBRegisterDeviceResponder::parseResponse(CBHelperResponseInfo resp) {
	this->helper->sessionId = resp.parsedMessage->getValueForKey("sessionid")->toString();
}

}

#include "CBHelper.moc"
