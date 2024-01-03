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
        
       dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
       						    << dds::core::policy::Reliability::Reliable()
                                                    << dds::core::policy::Durability::TransientLocal();

        /* To publish something, a topic is needed. */
        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data",topicQos);

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);
        
        dds::pub::qos::DataWriterQos writerQos = topic.qos();
 
        /* Now, the writer can be created to publish a VehicleData message. */
        dds::pub::DataWriter<VehicleData::VehiclePosition> writer(publisher, topic, writerQos);


       std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;
       while (writer.publication_matched_status().current_count() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        /* Create a message to write. */
        VehicleData::VehiclePosition vPosition;
        vPosition.v_Id("XX-XX-123");
        VehicleData::Position position(100,100);
        vPosition.v_CurrentPosition(position);
        vPosition.v_CurrentSpeed(80.6);

        /* Write the message. */
        std::cout << "=== [Publisher] Write sample." << std::endl;
        writer.write(vPosition);
      	while(1)
      	{
      	  	std::this_thread::sleep_for(std::chrono::seconds(10));
      	}
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}
