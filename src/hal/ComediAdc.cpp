#include <eeros/hal/ComediAdc.hpp>

using namespace eeros::hal;

ComediAdc::ComediAdc(std::string id,
					 ComediDevice* device,
					 uint32_t subDeviceNumber,
					 uint32_t channel,
					 double scale,
					 double offset) : ScalablePeripheralInput<double>(id, scale, offset) {
	this->deviceHandle = device->getDeviceHandle();
	this->subDeviceNumber = subDeviceNumber;
	this->channel = channel;
}

double ComediAdc::get() {
	lsampl_t data = 0;
	comedi_data_read(deviceHandle, subDeviceNumber, channel, 0, AREF_GROUND, &data);
	return static_cast<int>(data) * scale + offset;
}
