#include <eeros/control/Signal.hpp>
#include <sstream>

uint32_t Signal::signalCounter = 1;
std::list<Signal*> Signal::signalList;

Signal::Signal(sigdim_t dim) : length(dim) {
	id = signalCounter++;
	signalList.push_back(this);
}

Signal::~Signal() {
	signalList.remove(this);
}

uint32_t Signal::getSignalId() const {
	return id;
}

uint32_t Signal::getDimension() const {
    return length;
}

std::string Signal::getLabel() const {
	return getLabel(0);
}

std::string Signal::getLabel(int index) const {
	std::stringstream label;
	label << '#' << id << '/' << index;
	return label.str();
}

sigtype_t Signal::getType() const {
	return kSignalTypeUnknown;
}

std::list<Signal*>* Signal::getSignalList() {
	return &signalList;
}

Signal* Signal::getSignalById(uint32_t id) {
	std::list<Signal*>::iterator i = signalList.begin();
	while((*i)->id != id && i != signalList.end()) i++;
	return (*i);
}
