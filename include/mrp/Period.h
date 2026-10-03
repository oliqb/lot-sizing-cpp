#pragma once

#include <vector>

class Period{
        private:
            double _capacity;
            std::vector<double> _demand;
        public:
            Period(double c, std::vector<double> d);
            ~Period();

            double get_capacity() const;
            const std::vector<double>& get_demand() const;

            void set_capacity(double val);
            void set_demand(std::vector<double> val);

    };

