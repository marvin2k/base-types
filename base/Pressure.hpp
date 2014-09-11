#ifndef __BASE_PRESSURE_HPP__
#define __BASE_PRESSURE_HPP__

namespace base
{
    /** Representation of a pressure value
     *
     * It is internally normalized to a pressure in pascals (the SI unit for
     * pressure), but provides accessors and initializers to convert to the most
     * common pressure units (bar and PSI)
     */
    class Pressure
    {
    public:
        /** Pressure in pascals
         *
         * This is made public so that it can be used directly in Rock
         * components. You usually should not access it, but instead use one of
         * the initializers / accessors
         */
        float pressure;

    public:
        /** Create a pressure object using a value in pascals */
        static Pressure fromPa(float pressure)
        {
            Pressure result;
            result.pressure = pressure;
            return result;
        }

        /** Create a pressure object using a value in bars */
        static Pressure fromBar(float pressure)
        {
            return fromPa(pressure * 100000);
        }

        /** Create a pressure object using a value in PSI */
        static Pressure fromPSI(float pressure)
        {
            return fromPa(pressure * 6894.75729);
        }

        /** Returns the raw pressure value in pascals */
        float toPa() const
        {
            return pressure;
        }

        /** Returns the raw pressure value in bar */
        float toBar() const
        {
            return pressure / 100000;
        }

        /** Returns the raw pressure value in psi */
        float toPSI() const
        {
            return pressure / 6894.75729;
        }
    };
}

#endif

