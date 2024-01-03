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
                /* To publish something, a topic is needed. 
                 * Create a topic with Reliability Reliable and History KeepLast, depth = 2
                 */
        dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
                                                    << dds::core::policy::Reliability::Reliable()
                                                    << dds::core::policy::History::KeepLast(2);
                                                   

        /* To publish something, a topic is needed. */
        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data", topicQos);

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);

        /* Now, the writer can be created to publish a VehicleData message. */
       dds::pub::qos::DataWriterQos writerQosQos = topic.qos();
        dds::pub::DataWriter<VehicleData::VehiclePosition> writer(publisher, topic, writerQosQos);
        
       std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;
        while (writer.publication_matched_status().current_count() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        
        std::string vId="";
        std::cout<<"Enter Vehicle Id: "<<vId;
        std::cin>>vId;

        /* Create a message to write. */
        VehicleData::VehiclePosition vPosition;
        vPosition.v_Id(vId);
        std::cout<<"Enter Vehicle Current Position (x,y): ";
        int x,y,z;
        std::cin>>x>>y;
        VehicleData::Position position(x,y);
        vPosition.v_CurrentPosition(position);
        std::cout<<"Enter Vehicle Speed: ";
        std::cin>>z;
        vPosition.v_CurrentSpeed(z);

        /* Write the message. */
        writer.write(vPosition);
        std::cout << "=== [Publisher] Vehicle position ("<<x<<","<<y<<") published successfully." << std::endl;
        
        bool lFlag =true;
        int choice = 0;
	while(lFlag)
	{
		std::cout <<"Press 1 to Publish vehicle updated position, any other key to exit.";
		std::cin >> choice;
		if(choice == 1)
		{		
			VehicleData::Position position(++x,++y);
			vPosition.v_CurrentPosition(position);
			writer.write(vPosition);
			std::cout << "=== [Publisher] Vehicle updated position ("<<x<<","<<y<<") published successfully." << std::endl;
		}
		else
		{
			lFlag = false;
		}
	}
	
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}
