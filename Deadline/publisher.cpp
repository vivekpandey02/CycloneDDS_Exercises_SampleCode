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
         * The Reliability::Reliablebpolicy is specified to guarantee delivery and a policy::Deadline() of 1 second is
         * used to trigger the ExampleDataReaderListener::on_request_deadline_missed method
         * after no message  has been received for that duration. */
        dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
                                                    << dds::core::policy::Deadline(dds::core::Duration(3,0));

        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data", topicQos);

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);

        dds::pub::qos::DataWriterQos writerQosQos = topic.qos();

        /* Now, the writer can be created to publish a VehicleData message. */
        dds::pub::DataWriter<VehicleData::VehiclePosition> writer(publisher, topic, writerQosQos);


        std::string _id = "XX-XX-123";
        std::cout<<"Enter Vehicle Id [Registration No]: "<<std::endl;
        std::cin >> _id;

        std::cout<<"Enter Vehicle Starting Position: "<<std::endl;
        int _x = 100;
        int _y = 100;

        std::cin >> _x >>_y ;

        /* Create a message to write. */
        VehicleData::VehiclePosition vPosition;
        
        vPosition.v_Id(_id);
        VehicleData::Position position(_x,_y);
        vPosition.v_CurrentPosition(position);
        vPosition.v_CurrentSpeed(80.6);

        /* Write the message. */
        std::cout << "=== [Publisher] Write sample." << std::endl;
        int count =0;
        while (writer.publication_matched_status().current_count() > 0) {
       	    if(count % 5 == 0)
		std::this_thread::sleep_for(std::chrono::seconds(4));
	    else
	    	std::this_thread::sleep_for(std::chrono::seconds(1));
	    	
	    writer.write(vPosition);
            ++_x;
            ++_y;
            VehicleData::Position position(_x,_y);
            vPosition.v_CurrentPosition(position);
            ++count;
        }
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}
