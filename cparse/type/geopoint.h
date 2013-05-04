#ifndef ARG3_CPARSE_TYPE_GEOPOINT_H_
#define ARG3_CPARSE_TYPE_GEOPOINT_H_

#include "parsetype.h"

namespace cparse
{
    namespace type
    {
        class GeoPoint
        {
        public:
            GeoPoint(const Value &attributes);
            GeoPoint(double lat, double lon);
            GeoPoint();
            virtual ~GeoPoint();
            GeoPoint(const GeoPoint &value);
            GeoPoint(GeoPoint &&value);
            GeoPoint &operator=(const GeoPoint &value);
            GeoPoint &operator=(GeoPoint &&value);

            double getLatitude() const;
            double getLongitude() const;

            void setLatitude(double value);
            void setLongitude(double value);

            Value toValue() const;
            void fromValue(const Value &attributes);

        private:
            double latitude_;
            double longitude_;
        };
    }
}

#endif
