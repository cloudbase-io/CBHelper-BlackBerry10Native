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
#include <QObject>
#include <QTimer>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoCoordinate>

#include "../CBHelper.h"
#include "../DataCommands/CBHelperSearchCondition.h"

#include "CBGeoLocatedObject.h"

namespace Cloudbase {

#ifndef CBGEODATASTREAM_H_
#define CBGEODATASTREAM_H_

#define CBGEODATASTREAM_UPDATE_SPEED    1000
#define CBGEODATASTREAM_RADIUS_RATIO    4.0

class CBGeoDataStream : public QObject, CBHelperResponder {
	Q_OBJECT
public:
	/**
	 * The collection on which to run the search
	 */
	std::string collection;
	/**
	 * The radius for the next search in meters from the point returned by the
	 * getLatestPosition method
	 */
	double searchRadius;
	/**
	 * The name given in its constructor to this CBGeoDataStream
	 */
	std::string streamName;

	/**
	 * Creates a new CBGeoDataStream object using the given helper class to query the
	 * cloudbase.io APIs.
	 *
	 * @param helper An initialized CBHelper object
	 * @param collection The name of the collection containing the geospatial data in the cloud database
	 * @param searchRadius The initial search radius in meters.
	 */
	CBGeoDataStream(std::string streamName, CBHelper* helper, std::string collection, double searchRadius);

	/**
	 * Begins querying the cloudbase.io APIs and returning data periodically.
	 */
	void startStream();

	/**
	 * Stops the data stream
	 */
	void stopStream();

	virtual void parseResponse(Cloudbase::CBHelperResponseInfo resp);
private:
	CBHelper* helper;
	QtMobilitySubset::QGeoPositionInfo* previousPosition;
	QTimer *timer;
	double previousSpeed;
	double queryRadius;
	std::map<int, CBGeoLocatedObject*> foundObjects;
	bool isRunning;

Q_SIGNALS:
	/**
	 * Returns the latest known position to the CBGeoDataStream object.
	 * This is used to retrieve the data and compute the movement speed to
	 * increase or decrease the speed of refresh
	 *
	 * @return A valid QtMobilitySubset::QGeoPositionInfo* pointer
	 */
	QtMobilitySubset::QGeoPositionInfo* getLatestPosition(std::string streamName);
	/**
	 * receives a new point to be visualized
	 *
	 * @param CBGeoLocatedObject An object representing a new point on the map
	 */
	void receivedPoint(CBGeoLocatedObject* newPoint, std::string streamName);
	/**
	 * Informs the application that the CBGeoDataStream is removing a point from its cache
	 *
	 * @param CBGeoLocatedObject The point being removed
	 */
	void removingPoint(CBGeoLocatedObject* point, std::string streamName);

public Q_SLOTS:
	void retrievePoints();
};

#endif /* CBGEODATASTREAM_H_ */

}
