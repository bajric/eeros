#ifndef ORG_EEROS_EXAMPLES_IPC_MOVEINTERFACE_HPP_
#define ORG_EEROS_EXAMPLES_IPC_MOVEINTERFACE_HPP_


class MoveInterface {
public:
	MoveInterface() {}
	virtual void move(double position) = 0;
	
	static constexpr int moveID = 17;
};



#endif // ORG_EEROS_EXAMPLES_IPC_MOVEINTERFACE_HPP_
