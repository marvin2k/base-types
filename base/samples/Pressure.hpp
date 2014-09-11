#ifndef __BASE_SAMPLES_PRESSURE_HPP
#define __BASE_SAMPLES_PRESSURE_HPP

namespace base
{
    namespace samples
    {
        /** Timestamped pressure */
        struct Pressure : public base::Pressure
        {
            /** The sample timestamp */
            base::Time time;

            Pressure(base::Time const& time, base::Pressure pressure)
                : base::Pressure(pressure)
                , time(time) {}

            Pressure fromPa(base::Time const& time, float pressure)
            {
                return Pressure(time, base::Pressure::fromPa(pressure));
            }

            Pressure fromBar(base::Time const& time, float pressure)
            {
                return Pressure(time, base::Pressure::fromBar(pressure));
            }

            Pressure fromPSI(base::Time const& time, float pressure)
            {
                return Pressure(time, base::Pressure::fromPSI(pressure));
            }
        };
    }
}

#endif

