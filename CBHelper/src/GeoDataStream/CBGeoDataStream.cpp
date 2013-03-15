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
#include "CBGeoDataStream.h"

namespace Cloudbase {

CBGeoDataStream::CBGeoDataStream(std::string streamName, CBHelper* helper, std::string collection, double searchRadius) {
	this->streamName = streamName;
	this->helper = helper;
	this->collection = collection;
	this->searchRadius = searchRadius;
	this->queryRadius = searchRadius;
	this->isRunning = false;
	this->previousSpeed = 0.0;
}

void CBGeoDataStream::startStream() {
	this->isRunning = true;
	this->timer = new QTimer(this);

	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(retrievePoints()));
	this->timer->setInterval(CBGEODATASTREAM_UPDATE_SPEED);
	this->timer->start();
}

void CBGeoDataStream::stopStream() {
	this->isRunning = false;
	this->timer->stop();
}

void CBGeoDataStream::retrievePoints() {
	QtMobilitySubset::QGeoPositionInfo* currentLocation = emit getLatestPosition(this->streamName);

	if (currentLocation == NULL) {
		if (this->helper->debugMode) {
			qDebug("No location returned by getLatestPosition SIGNAL");
		}
		return;
	}

	if (this->previousPosition != NULL) {
		double distance = currentLocation->coordinate().distanceTo(this->previousPosition->coordinate());

		if (distance < this->queryRadius / CBGEODATASTREAM_RADIUS_RATIO) {
			if (this->helper->debugMode) {
				qDebug("Not enough distance between the two points. returning without fetching data");
			}
			return;
		}

		double speed = distance / (CBGEODATASTREAM_UPDATE_SPEED / 1000);
		double ratio = 1.0;

		if (this->helper->debugMode) {
			qDebug("Computed speed %f meters per second", speed);
		}
		if (this->previousSpeed != 0.0) {
			ratio = speed / this->previousSpeed;
		}
		if (this->queryRadius * ratio < this->searchRadius) {
			this->queryRadius = this->searchRadius;
		} else {
			this->queryRadius = this->queryRadius * ratio;
		}

		this->previousSpeed = speed;
	}
	this->previousPosition = currentLocation;

	CBHelperSearchCondition* search = new CBHelperSearchCondition(currentLocation->coordinate().latitude(), currentLocation->coordinate().longitude(), this->queryRadius);

	this->helper->searchDocument(this->collection, search, this);

	std::vector<int> itemsToRemove;
	for(std::map<int, CBGeoLocatedObject*>::iterator iter = this->foundObjects.begin();
					iter != this->foundObjects.end();
					iter++) {
		if (iter->second->coordinate.distanceTo(currentLocation->coordinate()) > this->searchRadius) {
			itemsToRemove.push_back(iter->first);
			emit removingPoint(iter->second, this->streamName);
		}
	}

	for (std::vector<int>::iterator iter = itemsToRemove.begin();
					iter != itemsToRemove.end();
					iter++) {
		this->foundObjects.erase(itemsToRemove.at(*iter));
	}
}

void CBGeoDataStream::parseResponse(Cloudbase::CBHelperResponseInfo resp) {
	if (resp.postSuccess) {
		if (resp.parsedMessage->getType() == YAJLDom::Value::ARRAY) {
			for (int i = 0; i < resp.parsedMessage->getNumChildValues(); i++) {
				YAJLDom::Value* newObj = resp.parsedMessage->getValueByIndex(i);
				CBGeoLocatedObject* outputObj = new CBGeoLocatedObject();
				YAJLDom::Value* location = newObj->getValueForKey("cb_location");
				double lat = location->getValueForKey("lat")->toDouble();
				double lng = location->getValueForKey("lng")->toDouble();
				if (!newObj->getValueForKey("cb_location_altitude")->isNull()) {
					double altitude = newObj->getValueForKey("cb_location_altitude")->toDouble();
					outputObj->coordinate = QtMobilitySubset::QGeoCoordinate(lat, lng, altitude);
					outputObj->altitude = altitude;
				} else {
					outputObj->coordinate = QtMobilitySubset::QGeoCoordinate(lat, lng);
				}

				outputObj->objectData = newObj;

				this->foundObjects.insert(std::make_pair(outputObj->hash(), outputObj));

				emit receivedPoint(outputObj, this->streamName);
			}
		}
	} else {
		qDebug("Error while calling the cloudbase.io APIs: %s", resp.errorMessage.c_str());
	}

}

}

#include "CBGeoDataStream.moc"
