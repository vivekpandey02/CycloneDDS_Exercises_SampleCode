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

VehicleDataReaderListener::VehicleDataReaderListener(){

}

VehicleDataReaderListener::~VehicleDataReaderListener() {}

void VehicleDataReaderListener::on_data_available(dds::sub::DataReader<VehicleData::VehiclePosition>& dr)
{

	std::cout<<"Data Available Listener Triggered. "<<std::endl;
	dds::sub::LoanedSamples<VehicleData::VehiclePosition> samples = dr.take();
	for (dds::sub::LoanedSamples<VehicleData::VehiclePosition>::const_iterator sample_iter = samples.begin();
			sample_iter < samples.end();
			++sample_iter)
	{
		const VehicleData::VehiclePosition& vPosition = sample_iter->data();
        const dds::sub::SampleInfo& info = sample_iter->info();

		if (info.valid())
		{
                        std::cout << "=== [Subscriber] Message received:" << std::endl;
                        std::cout << "    Vehicle Id  : " << vPosition.v_Id() << std::endl;
                        std::cout << "    Vehicle Position =  Latitude : " << vPosition.v_CurrentPosition().latitude() <<" | Longitude : "<<  vPosition.v_CurrentPosition().longitude() << std::endl;
                        std::cout << "    Vehicle Speed: = Speed : "<< vPosition.v_CurrentSpeed()<<std::endl;
		}
	}
}
