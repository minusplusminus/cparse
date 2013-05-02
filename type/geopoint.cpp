#include <cparse/type/geopoint.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        GeoPoint::GeoPoint() {}

        GeoPoint::GeoPoint(double lat, double lon) : latitude_(lat), longitude_(lon)
        {}

        GeoPoint::GeoPoint(const Value &obj)
        {
            fromValue(obj);
        }

        void GeoPoint::fromValue(const Value &obj)
        {
            if(obj.contains("latitude"))
                latitude_ = obj.getDouble("latitude");

            if(obj.contains("longitude"))
                longitude_ = obj.getDouble("longitude");
        }

        double GeoPoint::getLatitude() const
        {
            return latitude_;
        }

        double GeoPoint::getLongitude() const
        {
            return longitude_;
        }

        void GeoPoint::setLatitude(double value)
        {
            latitude_ = value;
        }

        void GeoPoint::setLongitude(double value)
        {
            longitude_ = value;
        }

        Value GeoPoint::toValue() const
        {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_GEOPOINT);

            value.setDouble("latitude", latitude_);

            value.setDouble("longitude", longitude_);

            return value;
        }
    }
}