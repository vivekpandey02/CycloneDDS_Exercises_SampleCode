/*
 * Copyright(c) 2023 ZettaScale Technology and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include <iostream>
#include "VehicleTrackingInfo.hpp"


VehicleTrackingInfo::VehicleTrackingInfo() {
  _noVehicles = 0;
  _onMoveVehicles = 0;
  _stoppedVehicles = 0;
  _oonVehicles = 0;
}



VehicleTrackingInfo* VehicleTrackingInfo::getInstance() {
  if (_vehicleTrackingInfo == NULL) {
    _vehicleTrackingInfo = new VehicleTrackingInfo();
    return _vehicleTrackingInfo;
  } else
    return _vehicleTrackingInfo;
}

VehicleTrackingInfo* VehicleTrackingInfo::_vehicleTrackingInfo = NULL;

void VehicleTrackingInfo::noVehicles(int cnt) {
	_noVehicles = _noVehicles + cnt;
}
void VehicleTrackingInfo::onMoveVehicles(int cnt) {
	_onMoveVehicles = _onMoveVehicles + cnt;
}

void VehicleTrackingInfo::stoppedVehicles(int cnt) {
	_stoppedVehicles = _stoppedVehicles + cnt;
}

void VehicleTrackingInfo::oonVehicles(int cnt) {
	_oonVehicles = _oonVehicles + cnt;
}

int VehicleTrackingInfo::noVehicles() {
		return _noVehicles;
}

int VehicleTrackingInfo::onMoveVehicles() {
		return _onMoveVehicles;
}

int VehicleTrackingInfo::stoppedVehicles() {
		return _stoppedVehicles;
}

int VehicleTrackingInfo::oonVehicles() {
		return _oonVehicles;
}

void VehicleTrackingInfo::dispayVehicleTrackingInfo()
{
	std::cout<<"##################################################################################################"<<std::endl;
	std::cout<<"Vehicles  |  On Move Vehicle  |  stopped Vehicles  |  Out of Network"<<std::endl;
		std::cout<<"   "<<_noVehicles<<"      |         "<<_onMoveVehicles<<"         |        "<<_stoppedVehicles<<"           |       "<<_oonVehicles<<std::endl;
	std::cout<<"##################################################################################################"<<std::endl;
}
