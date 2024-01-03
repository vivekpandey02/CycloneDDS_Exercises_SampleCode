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
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "VehicleData.hpp"
#include "VehicleDataReaderListener.hpp"

using namespace org::eclipse::cyclonedds;

int main() {
    try {
        std::cout << "=== [Subscriber] Create reader." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To subscribe to something, a topic is needed. */
        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data");

        /* A reader also needs a subscriber. */
        /** A dds::sub::Subscriber is created on the domain participant. */
        int _nPartition=0;
        dds::core::StringSeq partitions;
        std::cout<<"Please Create Partition: "<<std::endl;
        std::cout<<"Enter number of partition: "<<std::endl;
        std::cin>>_nPartition;
        std::string tmpPart="";
        for (int i=1;i<=_nPartition;++i)
        {
            std::cout<<"Enter partition"<<i<<" Name: "<<std::endl;
                std::cin>>tmpPart;
                partitions.push_back(tmpPart);
                tmpPart="";
        }

        //std::string name = "Vehicle-Zone1";
        dds::sub::qos::SubscriberQos subQos
            = participant.default_subscriber_qos()
                << dds::core::policy::Partition(partitions);
        dds::sub::Subscriber subscriber(participant, subQos);

        dds::sub::qos::DataReaderQos readerQos = subscriber.default_datareader_qos()
                                                 << dds::core::policy::Reliability::Reliable();
        VehicleDataReaderListener _dataListener;
        dds::core::status::StatusMask mask;
        mask << dds::core::status::StatusMask::data_available();

        /* Now, the reader can be created to subscribe to a VehicleData message. */
        dds::sub::DataReader<VehicleData::VehiclePosition> reader(subscriber, topic, readerQos,  &_dataListener, mask);

        /* Poll until a message has been read.
         * It isn't really recommended to do this kind wait in a polling loop.
         * It's done here just to illustrate the easiest way to get data.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */
        std::cout << "=== [Subscriber] Wait for message." << std::endl;

        while (1) {
                sleep(1);
        }
    } catch (const dds::core::Exception& e) {
        std::cerr << "=== [Subscriber] DDS exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "=== [Subscriber] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Subscriber] Done." << std::endl;

    return EXIT_SUCCESS;
}
