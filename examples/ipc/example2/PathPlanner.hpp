#ifndef ORG_EEROS_EXAMPLES_IPC_PATHPLANNER_HPP_
#define ORG_EEROS_EXAMPLES_IPC_PATHPLANNER_HPP_

#include <eeros/ipc/Server.hpp>
#include "MoveInterface.hpp"
#include <iostream>


class PathPlanner : public MoveInterface {
public:
	PathPlanner(eeros::ipc::Server &server) {
		server.add_method(moveID, "move", this, &PathPlanner::move);
	}
	
	virtual ~PathPlanner() { }
	
	virtual void move(double position) {
		std::cout << ">>>>>> move to position " << position << " <<<<<<" << std::endl;
	}
};



#endif // ORG_EEROS_EXAMPLES_IPC_PATHPLANNER_HPP_
