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
        std::cout << "=== [Subscriber] Create reader." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To subscribe to something, a topic is needed. 
         * Create a topic with Reliability Reliable and History KeepLast, depth = 2
         */
        dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
                                                    << dds::core::policy::Reliability::Reliable()
                                                    << dds::core::policy::History::KeepLast(2);
                                                    
        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data", topicQos);

        /* A reader also needs a subscriber. */
        dds::sub::Subscriber subscriber(participant);

        /* Now, the reader can be created to subscribe to a VehicleData message. */
        dds::sub::qos::DataReaderQos readerQos = topic.qos();
        dds::sub::DataReader<VehicleData::VehiclePosition> reader(subscriber, topic, readerQos);
        int choice;
    
        while (1) {
            /* For this example, the reader will return a set of messages (aka
             * Samples). There are other ways of getting samples from reader.
             * See the various read() and take() functions that are present. */
            dds::sub::LoanedSamples<VehicleData::VehiclePosition> samples;
            std:: cout<<"Press 1 for read, any other key for take : ";
            std::cin>> choice;

            if(choice ==1 )
            	samples = reader.read();
            else
            	samples = reader.take();

            /* Are samples read? */
            if (samples.length() > 0) {
                /* Use an iterator to run over the set of samples. */
                dds::sub::LoanedSamples<VehicleData::VehiclePosition>::const_iterator sample_iter;
                for (sample_iter = samples.begin();
                     sample_iter < samples.end();
                     ++sample_iter) {
                    /* Get the message and sample information. */
                    const VehicleData::VehiclePosition& vPosition = sample_iter->data();
                    const dds::sub::SampleInfo& info = sample_iter->info();

                    /* Sometimes a sample is read, only to indicate a data
                     * state change (which can be found in the info). If
                     * that's the case, only the key value of the sample
                     * is set. The other data parts are not.
                     * Check if this sample has valid data. */
                    if (info.valid()) {
                        std::cout << "=== [Subscriber] Message received: " << std::endl;
                        std::cout << "Vehicle Id  : " << vPosition.v_Id() ;
                        std::cout << " , Vehicle Position =  Latitude : " << vPosition.v_CurrentPosition().latitude() <<" | Longitude : "<<  vPosition.v_CurrentPosition().longitude();
                        std::cout << " , Vehicle Speed: = Speed : "<< vPosition.v_CurrentSpeed()<<std::endl;
                    }
                }
            } else {
            	std::cout<<"No sample in reader queue to process. "<<std::endl;
            }
            std::cout<<"======================================="<<std::endl;
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
