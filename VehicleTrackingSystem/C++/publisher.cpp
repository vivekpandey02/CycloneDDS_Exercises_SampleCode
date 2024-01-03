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


std::atomic<int> choice{0};
std::atomic<int> readyT{0};
//static int choice = 0;

int randomSpeed(){
    int lb = 80, ub = 100;
    return ((rand() % (ub - lb + 1)) + lb) ;
}

int main() {
    try {
        auto f = [&choice]()
        {
                    int ch = 0;
                    while(!readyT)
                    {
                        sleep(1);
                    }
                    while(1)
                    {
                        if(choice == 0 )
                        {
                            sleep(10);
                            std::cout<<"Press 1 to stop the car: "<<std::endl;
                            std::cin>>ch;
                            choice = ch;
                        }
                        else if(choice == 1)
                        {
                            std::cout<<"Press 0 to  move the car: "<<std::endl;
                            std::cin>>ch;  
                            choice = ch;                          
                        }
                    }
        };

        auto f1 = [&choice]()
        {
                    std::cout << "=== [Publisher] Create writer." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To publish something, a topic is needed. */
        dds::topic::qos::TopicQos topicQos = participant.default_topic_qos()
                                                    << dds::core::policy::Reliability::Reliable()
                                                    <<dds::core::policy::Liveliness::Automatic(dds::core::Duration::from_millisecs(100));

        dds::topic::Topic<VehicleData::VehiclePosition> topic(participant, "VehiclePosition_data", topicQos);

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);

        dds::pub::qos::DataWriterQos writerQosQos = topic.qos();

        std::string _id = "XX-XX-123";
        std::cout<<"Enter Vehicle Id [Registration No]: "<<std::endl;
        std::cin >> _id;

        std::cout<<"Enter Vehicle Starting Position: "<<std::endl;
        int _x = 100;
        int _y = 100;

        std::cin >> _x >>_y ;

        /* Now, the writer can be created to publish a VehicleData message. */
        dds::pub::DataWriter<VehicleData::VehiclePosition> writer(publisher, topic, writerQosQos);
                   /* Create a message to write. */
            VehicleData::VehiclePosition vPosition;
            
            vPosition.v_Id(_id);
            VehicleData::Position position(_x,_y);
            vPosition.v_CurrentPosition(position);
            vPosition.v_CurrentSpeed(randomSpeed());

            /* Write the message. */
            int count = 0;
            int pChoice;
            readyT = 1;
            while (writer.publication_matched_status().current_count() > 0) {    
                //std::cout << "=== [Publisher] Write sample." << std::endl;
                        writer.write(vPosition);
                        std::this_thread::sleep_for(std::chrono::seconds(1)); 
                        pChoice = choice;
                        //std::cout<<"choice: "<<pChoice<<std::endl;
                        if(choice == 0)
                        {
                            ++_x;
                            ++_y;
                            VehicleData::Position position(_x,_y);
                            vPosition.v_CurrentPosition(position);
                            vPosition.v_CurrentSpeed(randomSpeed());
                        }
                        else if(choice == 1)
                        {
                            //VehicleData::Position position(_x,_y);
                           // vPosition.v_CurrentPosition(position);
                            vPosition.v_CurrentSpeed(0);
                        }
        }
    };


        std::thread th1(f);
        std::thread th2(f1);
        th1.join();
        th2.join();


 
        
    }
    catch (const dds::core::Exception& e) {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}
