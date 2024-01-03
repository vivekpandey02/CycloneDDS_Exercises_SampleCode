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
#include "VehicleDataReaderListener.hpp"
#include "VehicleData.hpp"
#include "VehicleTrackingInfo.hpp"
#include <algorithm>


VehicleDataReaderListener::VehicleDataReaderListener(){
				isPrintHeader = false;
				stoppedVehicleList.clear();
				VehicleList.clear();
}

VehicleDataReaderListener::~VehicleDataReaderListener() {}

void VehicleDataReaderListener::on_data_available(dds::sub::DataReader<VehicleData::VehiclePosition>& dr)
{
	dds::sub::LoanedSamples<VehicleData::VehiclePosition> samples = dr.take();
	for (dds::sub::LoanedSamples<VehicleData::VehiclePosition>::const_iterator sample_iter = samples.begin();
			sample_iter < samples.end();
			++sample_iter)
	{
		const VehicleData::VehiclePosition& vPosition = sample_iter->data();
        const dds::sub::SampleInfo& info = sample_iter->info();

		if (info.valid())
		{
                        //std::cout << "=== [Subscriber] Message received:" << std::endl;
						if(!isPrintHeader)
						{
                        	std::cout << "    Vehicle     |   Location   |   Speed" << std::endl;
							isPrintHeader = true;
						}
                        std::cout << vPosition.v_Id() <<"   |   ["<< vPosition.v_CurrentPosition().latitude() <<","<<  vPosition.v_CurrentPosition().longitude()<<"]   |   "<< vPosition.v_CurrentSpeed()<<std::endl;

						if(vPosition.v_CurrentSpeed() == 0)
						{
									auto it = std::find(stoppedVehicleList.begin(),
													stoppedVehicleList.end(), vPosition.v_Id());
								    if(it != stoppedVehicleList.end())
									{
											//Already stopped car - nothing to do
									}
									else
									{
										stoppedVehicleList.push_back(vPosition.v_Id());
										VehicleTrackingInfo::getInstance()->stoppedVehicles(1);
										VehicleTrackingInfo::getInstance()->onMoveVehicles(-1);
										VehicleTrackingInfo::getInstance()->dispayVehicleTrackingInfo();
									}



						}
						else
						{
									auto it1= std::find(oonVehicleList.begin(),
													oonVehicleList.end(), vPosition.v_Id());
									if(it1 != oonVehicleList.end())
									{
										oonVehicleList.erase(it1);
										VehicleTrackingInfo::getInstance()->oonVehicles(-1);
										VehicleTrackingInfo::getInstance()->onMoveVehicles(1);
										VehicleTrackingInfo::getInstance()->dispayVehicleTrackingInfo();
									}

									auto it = std::find(stoppedVehicleList.begin(),
													stoppedVehicleList.end(), vPosition.v_Id());
								    	if(it != stoppedVehicleList.end())
									{
										stoppedVehicleList.erase(it);
										VehicleTrackingInfo::getInstance()->stoppedVehicles(-1);
										VehicleTrackingInfo::getInstance()->onMoveVehicles(1);
										VehicleTrackingInfo::getInstance()->dispayVehicleTrackingInfo();
									}
									else
									{
										auto it = std::find(VehicleList.begin(),
													VehicleList.end(), vPosition.v_Id());
										if(it != VehicleList.end())
										{

										}
										else{
											VehicleList.push_back(vPosition.v_Id());
											VehicleTrackingInfo::getInstance()->noVehicles(1);
											VehicleTrackingInfo::getInstance()->onMoveVehicles(1);
											VehicleTrackingInfo::getInstance()->dispayVehicleTrackingInfo();
										}
									}
						}
		}
		else
		{
			std::cout<<"Invalid Sample: "<<vPosition.v_Id()<<std::endl;
			oonVehicleList.push_back(vPosition.v_Id());
		}
	}
}

void VehicleDataReaderListener::on_liveliness_changed (dds::sub::DataReader<VehicleData::VehiclePosition>& dr,const dds::core::status::LivelinessChangedStatus & status)
{
	// Montior the Vehicle Health
	if(status.alive_count_change() == 1){
		//VehicleTrackingInfo::getInstance()->noVehicles(status.alive_count_change());
	}else if(status.alive_count_change() == -1)
	{
		VehicleTrackingInfo::getInstance()->oonVehicles(1);
		VehicleTrackingInfo::getInstance()->onMoveVehicles(-1);
	}
	else
	{
	}
	 	
	 
	VehicleTrackingInfo::getInstance()->dispayVehicleTrackingInfo();

	//std::cout<<"##################################################################################################"<<std::endl;
	//std::cout<<"alive: "<<status.alive_count()<<" alive_count_change: "<<status.alive_count_change()<<"not_alive_count: "<<status.not_alive_count()<<" Total Count: "<<status.total_count()<<std::endl;
	//std::cout<<"##################################################################################################"<<std::endl;
}



VehicleTrackerListener::VehicleTrackerListener(){

}

VehicleTrackerListener::~VehicleTrackerListener() {}

void VehicleTrackerListener::on_data_available(dds::sub::DataReader<VehicleData::VehicleTracking>& dr)
{

	dds::sub::LoanedSamples<VehicleData::VehicleTracking> samples = dr.take();
	for (dds::sub::LoanedSamples<VehicleData::VehicleTracking>::const_iterator sample_iter = samples.begin();
			sample_iter < samples.end();
			++sample_iter)
	{
		const VehicleData::VehicleTracking& vTrackingData = sample_iter->data();
        const dds::sub::SampleInfo& info = sample_iter->info();

		if (info.valid())
		{
                        std::cout << "Vehicle  |  on the move   |   Stopped   |   Out of Network" << std::endl;
						std::cout << vTrackingData.noOfVehicle()<<"   "<<vTrackingData.vOnTheMove() <<"   "<<vTrackingData.vStopped()<<"   "<<vTrackingData.vOutOfNetwork()<< std::endl;
                        std::cout << "----------------------------------------------------------" <<std::endl;
		}
	}
}
