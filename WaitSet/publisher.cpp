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

/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "VehicleData.hpp"

using namespace org::eclipse::cyclonedds;

int main() {
    try {
        std::cout << "=== [Publisher] Create writer." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To publish something, a topic is needed. */
        dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
                                                    << dds::core::policy::Durability::TransientLocal()
                                                    << dds::core::policy::Reliability::Reliable()
                                                    << dds::core::policy::History::KeepLast(2);
                                                    << dds::core::policy::Liveliness::Automatic(dds::core::Duration::from_millisecs(100));
        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data", topicQos);

        /* A writer also needs a publisher. */
                /** A dds::sub::Subscriber is created on the domain participant. */
        std::string name = "Vehicle-Zone1";
        dds::pub::qos::PublisherQos pubQos
            = participant.default_publisher_qos()
                << dds::core::policy::Partition(name);
 
        dds::pub::Publisher publisher(participant, pubQos);

        /* Now, the writer can be created to publish a VehicleData message. */
        dds::pub::qos::DataWriterQos dwqos = topic.qos();
        dds::pub::DataWriter<VehicleData::VehiclePosition> writer(publisher, topic, dwqos);

        /* For this example, we'd like to have a subscriber to actually read
         * our message. This is not always necessary. Also, the way it is
         * done here is just to illustrate the easiest way to do so. It isn't
         * really recommended to do a wait in a polling loop, however.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */
        std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;
        while (writer.publication_matched_status().current_count() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
            std::string vRNumber="XX-XX-123";
        	int _xPos = 100;
	        int _yPos = 100;

            std::cout<<"Please Enter the Vehicle Registration id and current Position (x,y) : "<<std::endl;
            std::cin>>vRNumber>>_xPos>>_yPos;


        /* Create a message to write. */
        VehicleData::VehiclePosition vPosition;
        vPosition.v_Id(vRNumber);
        VehicleData::Position position(_xPos,_yPos);
        vPosition.v_CurrentPosition(position);
        vPosition.v_CurrentSpeed(80.6);
	

	bool cW = true; 
	int choice=1;
	while(cW)
	{
       		/* Write the message. */
        	std::cout << "=== [Publisher] Write sample." << std::endl;
        	writer.write(vPosition);
		
		std::cout<<"Press 1 to write more sample"<<std::endl;
		std::cin>> choice;
		if(choice !=1)
			cW = false;
	}

    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}
