#include "Solution.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <ilcplex/ilocplexi.h>
#pragma GCC diagnostic pop
void CVRP::Solution::pushTour(routing::models::Tour *tour)
{
    addTour(tour, getNbTour());
}

void CVRP::Solution::addTour(routing::models::Tour  *tour, unsigned long position)
{

    tours.insert(tours.begin() + position, static_cast<Tour*>(tour));
    traveltime += static_cast<Tour*>( tour )->traveltime;
}

void CVRP::Solution::getVarsVals(IloNumVarArray &vars, IloNumArray &vals)
{
    std::vector<std::vector<IloBool> > values(problem->arcs.size());
    std::vector<std::vector<IloBool> > affectation(problem->affectation.size());
    std::vector<IloInt> consumption(static_cast<Problem*>(problem)->consumption.size(), 0);
    for (unsigned j = 0; j < problem->arcs.size(); ++j){
        values[j] = std::vector<IloBool>(problem->arcs.size(), IloFalse);
        affectation[j] = std::vector<IloBool>(problem->affectation[j].size(), IloFalse);
    }
    for (unsigned k = 0; k < this->getNbTour(); ++k) {
        unsigned last = 0;
        for (unsigned i = 0; i < static_cast<Tour*>(this->getTour(k))->getNbClient(); ++i) {
            values[last][static_cast<Tour*>(this->getTour(k))->getClient(i)->getID()] = IloTrue;
            consumption[static_cast<Tour*>(this->getTour(k))->getClient(i)->getID()] = consumption[last]
                                                                                     + static_cast<Client*>(this->getTour(k)->getClient(i))->getDemand();
            last = static_cast<Tour*>(this->getTour(k))->getClient(i)->getID();
            affectation[last][k] = IloTrue;

        }
        values[last][0] = IloTrue;
    }
    for (unsigned i = 0; i < static_cast<Problem*>(problem)->consumption.size(); ++i) {
        vars.add(static_cast<Problem*>(problem)->consumption[i]);
        vals.add(consumption[i]);
    }
    for (unsigned i = 0; i < problem->arcs.size(); ++i) {
        for (unsigned j = 0; j < problem->arcs.size(); ++j) {
            if (i == j) continue;
            vars.add(problem->arcs[i][j]);
            vals.add(values[i][j]);
        }
        for (unsigned k = 0; k < problem->affectation[i].size(); ++k) {
            vars.add(static_cast<Problem*>(problem)->affectation[i][k]);
            vals.add(affectation[i][k]);
        }
    }
}

void CVRP::Solution::print(std::ostream & out)
{
    out << "solution cost " << getCost() << std::endl;

    for (unsigned t = 0;t < getNbTour();t++) {
        out << "tour " << t << " : ";
        for (unsigned i = 0; i < static_cast<Tour*>(this->getTour(t))->getNbClient(); ++i) {
            out << static_cast<Tour*>(this->getTour(t))->getClient(i)->getID() << " ";
        }
        out << std::endl;
    }
}

routing::models::Solution *CVRP::Solution::clone() const{
    return new Solution(*this);
}

void CVRP::Solution::update(routing::models::Solution* copy )
{
    for(int i = 0 ; i < this->notserved.size(); i++){
        this->notserved[i] = copy->notserved[i];
    }
    this->traveltime = static_cast<CVRP::Solution*>(copy)->traveltime;

    for (int j = 0; j < this->tours.size(); ++j) {
        this->tours[j] = static_cast<CVRP::Tour*>(copy->getTour(j));
    }

}

