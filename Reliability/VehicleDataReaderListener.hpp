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
#include "dds/dds.hpp"
#include <iostream>
#include <vector>
#include "VehicleData.hpp"


class VehicleDataReaderListener :  public virtual dds::sub::DataReaderListener<VehicleData::VehiclePosition>, public virtual dds::sub::NoOpDataReaderListener<VehicleData::VehiclePosition>
{
public:
	VehicleDataReaderListener();
	virtual ~VehicleDataReaderListener();

	virtual void on_data_available(dds::sub::DataReader<VehicleData::VehiclePosition>& the_reader);
   
};
