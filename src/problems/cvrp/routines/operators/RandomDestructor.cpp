#include "RandomDestructor.hpp"
#include "../../../../mtrand.hpp"

void CVRP::RandomDestructor::destruct(routing::models::Solution *solution, routing::DestructionParameters* param)
{
    routing::RandomDestructionParameters* parameters = static_cast<routing::RandomDestructionParameters*>(param);
    unsigned long n = parameters->getDmax();

    if(solution->notserved.size() == solution->getProblem()->clients.size()) return;
    Utilities::MTRand_int32 irand(std::time(nullptr));
    unsigned drem = 1 + irand() % n;
    do {
        unsigned t = irand() % solution->getNbTour();
        while ( static_cast<Tour*>(solution->getTour(t))->getNbClient() < 1) {
            t = irand() % solution->getNbTour();
        }
        unsigned long position = (irand() % static_cast<Tour*>(solution->getTour(t))->getNbClient());
        Client * client = static_cast<Client*>(solution->getTour(t)->getClient(position));
        static_cast<Solution*>(solution)->notserved.push_back(client);
        routing::Duration traveltime = static_cast<Tour*>(solution->getTour(t))->traveltime;
        solution->getTour(t)->removeClient(position);
        static_cast<Solution*>(solution)->traveltime = static_cast<Solution*>(solution)->traveltime
                                                       - traveltime
                                                       + static_cast<Tour*>(solution->getTour(t))->traveltime;
        solution->update();
    }
    while (static_cast<Solution*>(solution)->notserved.size() < drem);


}