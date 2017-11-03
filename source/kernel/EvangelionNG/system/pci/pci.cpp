/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
*         This software is derived from the Ghost operating system project,           *
*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
*         https://ghostkernel.org/                                                    *
*         https://github.com/maxdev1/ghost                                            *
*                                                                                     *
* This program is free software; you can redistribute it and/or                       *
* modify it under the terms of the GNU General Public License                         *
* as published by the Free Software Foundation; either version 2                      *
* of the License, or (char *argumentat your option) any later version.                *
*                                                                                     *
* This program is distributed in the hope that it will be useful,                     *
* but WITHout ANY WARRANTY; without even the implied warranty of                      *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
* GNU General Public License for more details.                                        *
*                                                                                     *
* You should have received a copy of the GNU General Public License                   *
* along with this program; if not, write to the Free Software                         *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "pci.hpp"
#include "logger/logger.hpp"
#include "system/IOPorts.hpp"
#include "utils/ListEntry.hpp"

/**
 * global list of pci devices
 */
llist<PciHeader*> *Pci::pciHeaders = 0;

/**
 * initialize the pci scanning
 */
void Pci::initialize()
{
	// create the list
	pciHeaders = new llist<PciHeader*>();

	// scan the busses
	scanBusesByBruteforce();
	logInfo("%! found %i devices", "pci", pciHeaders->count());
}

/**
 * read data from device and return the pci device code
 *
 * @param bus:			bus where we write
 * @param slot:			pci slot where we write
 * @param function:		function of the device
 * @param reg:			device register
 * @return the device id
 */
uint32_t Pci::readConfiguration(uint8_t bus, uint8_t slot, uint8_t function, uint8_t reg)
{
	uint32_t address = (((uint32_t) bus << 16) | ((uint32_t) slot << 11) | ((uint32_t) function << 8) | (reg & 0xFC) | ENABLE_BIT);

	// write address to address field
	IOports::writeInt(CONFIG_ADDRESS, address);

	// read data from data field
	return IOports::readInt(CONFIG_DATA);
}

/**
 * Simple brute-force scanning implementation that checks each slot on each bus if there
 * is a valid device available.
 */
void Pci::scanBusesByBruteforce()
{
	logInfo("%! scanning bus with brute force", "pci");

	for (uint32_t bus = 0; bus < 256; bus++)
	{
		for (uint32_t slot = 0; slot < 32; slot++)
		{
			// check for a valid device in function 0
			if (isValidDevice(bus, slot, 0))
			{
				// check if it's a multifunction device
				if (isMultifunctionDevice(bus, slot))
				{
					// device may have up to 8 functions, check each
					for (int function = 0; function < 8; function++)
						if (isValidDevice(bus, slot, function)) createDeviceHeader(bus, slot, function);
				}

				// create a device header without functions
				else createDeviceHeader(bus, slot, 0);
			}
		}
	}
}

/**
 * Creates the device header that we remember in our global device list.
 *
 * @param bus:			bus where we write
 * @param slot:			pci slot where we write
 * @param function:		function of the device
 */
void Pci::createDeviceHeader(uint8_t bus, uint8_t slot, uint8_t function)
{
	// read header type
	PciHeader *header = createHeader();
	header->bus = bus;
	header->slot = slot;
	header->function = function;

	uint8_t headerTypeWithMultifuncBit = (readConfiguration(bus, slot, function, 0x0C) & 0xFF0000) >> 16;
	header->headerType = headerTypeWithMultifuncBit & 0x7F;
	header->multiFunction = (headerTypeWithMultifuncBit & 0x80) >> 7;

	// read information common to all headers
	uint32_t entry00 = readConfiguration(bus, slot, function, 0x00);
	header->deviceId = entry00 >> 16;
	header->vendorId = entry00 & 0xFFFF;

	uint32_t entry08 = readConfiguration(bus, slot, function, 0x08);
	header->classCode = (entry08 & 0xFF000000) >> 24;
	header->subclassCode = (entry08 & 0xFF0000) >> 16;
	header->progIf = (entry08 & 0xFF00) >> 8;
	header->revisionId = (entry08 & 0xFF);
}

/**
 * Searches for a device of the requested type
 *
 * @param classCode:		pci device class
 * @param subclassCode:		pci device subclassCode
 * @param index:			the index of the device
 * @return a pci header instance with pci device infos
 */
PciHeader *Pci::findDeviceOfType(uint8_t classCode, uint8_t subclassCode, int index)
{
	PciHeader *header = pciHeaders->get(index);
	if (classCode == header->classCode && subclassCode == header->subclassCode) return header;

	return 0;
}

/**
 * Searches for a device of the requested type
 *
 * @param bus:			bus where we write
 * @param slot:			pci slot where we write
 * @param function:		function of the device
 * @return a pci header instance with pci device infos
 */
PciHeader *Pci::findDeviceAt(uint8_t bus, uint8_t slot, uint8_t function)
{
	// parse the list
	for (llist<PciHeader*>::iterator it = pciHeaders->begin(); it != pciHeaders->end(); ++it)
		if ((*it)->bus == bus && (*it)->slot == slot && (*it)->function == function) return *it;

	return 0;
}

/**
 * @return the number of pci device
 */
uint32_t Pci::getDeviceCount()
{
	return pciHeaders->count();
}

/**
 * get a device descriptor from position
 *
 * @param position:		list position
 * @return the instance of the pci header
 */
PciHeader *Pci::getDeviceAt(uint32_t position)
{
	return pciHeaders->get(position);
}

/**
 * check device validity
 *
 * @param bus:			bus where we write
 * @param slot:			pci slot where we write
 * @param function:		function of the device
 * @return if the device is valid
 */
bool Pci::isValidDevice(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t vendor = readConfiguration(bus, slot, function, 0) & 0xFFFF;

	// if invalid vendor, this is not a valid device
	if (vendor == 0xFFFF) return false;

	return true;
}

/**
 * check device multifunction
 *
 * @param bus:		bus where we write
 * @param slot:		pci slot where we write
 * @return if the device is valid
 */
bool Pci::isMultifunctionDevice(uint8_t bus, uint8_t slot)
{
	uint8_t headerType = (readConfiguration(bus, slot, 0, 0x0C) & 0xFF0000) >> 16;
	return (headerType & 0x80);
}

/**
 * create a pci device descriptor
 *
 * @return the created device header
 */
PciHeader *Pci::createHeader()
{
	// add to list a new header instance
	pciHeaders->add(new PciHeader());
	return pciHeaders->back();
}
