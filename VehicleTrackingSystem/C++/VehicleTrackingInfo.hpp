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
#include <vector>
#pragma once

class VehicleTrackingInfo
{
    int _noVehicles;
    int _onMoveVehicles;
    int _stoppedVehicles;
    int _oonVehicles;
	static VehicleTrackingInfo* _vehicleTrackingInfo;
     VehicleTrackingInfo();
	public:
		static VehicleTrackingInfo* getInstance();
		//Increase count of total number of Vehicle by 1
		void noVehicles(int);
		
		//Increase count of total number of vehicle in motion by 1
		void onMoveVehicles(int);
		
		//Increase count of  total number of Stoppped vehicle by 1
		void stoppedVehicles(int);
		
		//Increase Out of Network Vehicle
		void oonVehicles(int);
		
		// Total number of vehicle
		int noVehicles();
		
		// Total number of movable vehicle
		int onMoveVehicles();
		
		// Total number of stopped vehicle
		int stoppedVehicles();
		
		// Total number of out of network vehicle
		int oonVehicles();

		// Desplay the Vehicle Tracing Information
		void dispayVehicleTrackingInfo();
};
