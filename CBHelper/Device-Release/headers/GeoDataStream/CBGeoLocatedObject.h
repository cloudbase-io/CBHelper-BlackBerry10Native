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
#include <QtLocationSubset/QGeoCoordinate>

#include "../yajl/YAJLDom.h"

namespace Cloudbase {

#ifndef CBGEOLOCATEDOBJECT_H_
#define CBGEOLOCATEDOBJECT_H_

/**
 * Represents an object returned by a CBGeoDataStream with its
 * coordinates, altitude and additional information stored in the
 * cloud database collection
 */
class CBGeoLocatedObject : public QObject {
public:

	CBGeoLocatedObject();

	/**
	 * The coordinate position of the object
	 */
	QtMobilitySubset::QGeoCoordinate coordinate;
	/**
	 * The altitude of the object if the cb_location_altitude field
	 * exists in the document
	 */
	double altitude;
	/**
	 * All of the other data stored in the cloud database for the
	 * document
	 */
	YAJLDom::Value* objectData;

	int hash();
};

#endif

}
