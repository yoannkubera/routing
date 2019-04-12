//
// Created by ali on 4/11/19.
//


#pragma once



#include "../models/Solution.hpp"
#include "../routines/operators/Constructor.hpp"
#include "../routines/operators/Destructor.hpp"
#include "../../../solvers/LSSolver.hpp"
#include "../../../routines/neighborhoods/IDCH.hpp"

namespace CVRPTW{
    template <class Reader>
    class LSSolver: public routing::LSSolver<Reader> {
        public:
            LSSolver(const std::string & p_inputFile) : routing::LSSolver<Reader>(p_inputFile,this->getGenerator(),this->getNeighbors())
            {

            }

            virtual void initSolution() override
            {
                this->solution = new Solution(static_cast<CVRPTW::Problem*>(this->problem));
            }

            virtual std::vector<routing::Neighborhood*> getNeighbors() {
                std::vector<routing::Neighborhood*> neighbors{
                        new routing::IDCH(new Constructor, new Destructor)
                };
                return neighbors;
            }

            virtual routing::Generator * getGenerator() {
                return new routing::Generator(new Constructor, new Destructor);
            }
    };
}