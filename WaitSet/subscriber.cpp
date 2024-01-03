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


class ReadCondHandler
{
public:
    /**
     * @param dataState The dataState on which to filter the samples
     */
    ReadCondHandler(dds::sub::status::DataState& dataState)
    : dataState(dataState) {}

    void operator() (const dds::sub::cond::ReadCondition& cond)
    {
        /** retrieve the DataState from the condition */
        dds::sub::status::DataState dataState = cond.state_filter();

        /** retrieve the associated reader from the condition */
        dds::sub::DataReader<VehicleData::VehiclePosition> dr = cond.data_reader();

        dds::sub::LoanedSamples<VehicleData::VehiclePosition> samples = dr.select().state(dataState).take();

        for (dds::sub::LoanedSamples<VehicleData::VehiclePosition>::const_iterator sample_iter = samples.begin();
            sample_iter < samples.end(); ++sample_iter)
        {
            const VehicleData::VehiclePosition& vPosition = sample_iter->data();
            const dds::sub::SampleInfo& info = sample_iter->info();
            if(info.valid())
            {
                std::cout << "\n    --- New message received with ReadCondition on message field ---" << std::endl;
                        std::cout << "=== [Subscriber] Message received:" << std::endl;
                        std::cout << "    Vehicle Id  : " << vPosition.v_Id() << std::endl;
                        std::cout << "    Vehicle Position =  Latitude : " << vPosition.v_CurrentPosition().latitude() <<" | Longitude : "<<  vPosition.v_CurrentPosition().longitude() << std::endl;
                        std::cout << "    Vehicle Speed: = Speed : "<< vPosition.v_CurrentSpeed()<<std::endl;
            }
	}
    }

private:
    dds::sub::status::DataState& dataState;
};

/**
 * Functor to handle the triggering of a StatusCondition within a WaitSet.
 * When the StatusCondition is triggered it will check if a DataWriter has
 * lost it's liveliness and output a message indicating this or an alternate
 * message if a DataWriter has joined.
 */
class StatusCondHandler
{
public:
    /**
     * @param prevAliveCount The previous number of DataWriters
     * @param writerLeft Indicates if a DataWriter has lost it's liveliness
     * @param guardTriggered Indicates if the GuardCondition should be triggered
     */
    StatusCondHandler(int& prevAliveCount, bool& writerLeft, bool& guardTriggered)
    : prevAliveCount(prevAliveCount), writerLeft(writerLeft), guardTriggered(guardTriggered) {}

    void operator() (const dds::core::cond::StatusCondition& cond)
    {
        const dds::core::status::LivelinessChangedStatus livChangedStatus =
                ((dds::sub::DataReader<VehicleData::VehiclePosition>&)cond.entity()).liveliness_changed_status();

        if(livChangedStatus.alive_count() < prevAliveCount)
        {
            std::cout << "\n!!! a DataWriter lost its liveliness" << std::endl;
            writerLeft = true;
            std::cout << "\n=== Triggering Guard condition " << std::endl;
            guardTriggered = true;
        }
        else if(livChangedStatus.alive_count() > prevAliveCount)
        {
            std::cout << "\n!!! a DataWriter joined" << std::endl;
        }
        prevAliveCount = livChangedStatus.alive_count();
    }

private:
    int& prevAliveCount;
    bool& writerLeft;
    bool& guardTriggered;
};



/**
 * Functor to handle the triggering of a QueryCondition within a WaitSet.
 * When the QueryCondition is triggered it will output the messages that
 * triggered the QueryCondition.
 */
class QueryCondHandler
{
public:
    /**
     */
    QueryCondHandler() {}

    void operator() (const dds::sub::cond::QueryCondition& cond)
    {
        /** retrieve the associated reader from the condition */
        dds::sub::DataReader<VehicleData::VehiclePosition> dr = cond.data_reader();

        dds::sub::LoanedSamples<VehicleData::VehiclePosition> samples = dr.select().content(cond).take();

        for (dds::sub::LoanedSamples<VehicleData::VehiclePosition>::const_iterator sample_iter = samples.begin();
            sample_iter < samples.end(); ++sample_iter)
        {
            const VehicleData::VehiclePosition& vPosition = sample_iter->data();
            const dds::sub::SampleInfo& info = sample_iter->info();
            if(info.valid())
            {
                std::cout << "\n    --- New message received with QueryCondition on message field ---" << std::endl;
                        std::cout << "=== [Subscriber] Message received:" << std::endl;
                        std::cout << "    Vehicle Id  : " << vPosition.v_Id() << std::endl;
                        std::cout << "    Vehicle Position =  Latitude : " << vPosition.v_CurrentPosition().latitude() <<" | Longitude : "<<  vPosition.v_CurrentPosition().longitude() << std::endl;
                        std::cout << "    Vehicle Speed: = Speed : "<< vPosition.v_CurrentSpeed()<<std::endl;
            }
        }
    }
};

/**
 * Functor to handle the triggering of a StatusCondition within a WaitSet.
 * When the StatusCondition is triggered it will check if a DataWriter has
 * lost it's liveliness and output a message indicating this or an alternate
 * message if a DataWriter has joined.
 */


/**
 * Functor to handle the triggering of a GuardCondition within a WaitSet.
 * When the GuardCondition is triggered it will set a boolean to indicate
 * that the example should exit.
 */
class GuardCondHandler
{
public:
    /**
     * @param escaped Indicates that the example should exit
     * @param count Indicates the number of times the WaitSet waited
     */
    GuardCondHandler(bool& escaped, int& count)
    : escaped(escaped), count(count) {}

    void operator() (const dds::core::cond::Condition& cond)
    {
        std::cout << std::endl << "!!! Guard condition triggered - count = " << count << std::endl;
        escaped = true;
    }

private:
    bool& escaped;
    int& count;
};

int main() {

    try
    {
        /** A domain participant and topic are created identically as in
         the ::publisher */
        dds::domain::DomainParticipant dp(domain::default_id());
        dds::topic::qos::TopicQos topicQos = dp.default_topic_qos()
                                                    << dds::core::policy::Durability::TransientLocal()
                                                    << dds::core::policy::Reliability::Reliable()
                                                    << dds::core::policy::History::KeepLast(2);
                                                    << dds::core::policy::Liveliness::Automatic(dds::core::Duration::from_millisecs(100));
        dds::topic::Topic<VehicleData::VehiclePosition> topic(dp, "VehiclePosition_data", topicQos);

        /** A dds::sub::Subscriber is created on the domain participant. */
        std::string name = "Vehicle-Zone1";
        dds::sub::qos::SubscriberQos subQos
            = dp.default_subscriber_qos()
                << dds::core::policy::Partition(name);
        dds::sub::Subscriber sub(dp, subQos);

        /** The dds::sub::qos::DataReaderQos are derived from the topic qos */
        dds::sub::qos::DataReaderQos drqos = topic.qos();

        /** A dds::sub::DataReader is created on the Subscriber & Topic with the DataReaderQos. */
        dds::sub::DataReader<VehicleData::VehiclePosition> dr(sub, topic, drqos);

        //Logic variables
        int prevAliveCount = 0;
        bool closed = false;
        bool escaped = false;
        bool writerLeft = false;
        bool guardTriggered = false;
        int count = 0;

        /** A ReadCondition is created which is triggered when a new message is received */  
        dds::sub::status::DataState newDataState;
        newDataState << dds::sub::status::SampleState::not_read()
                << dds::sub::status::ViewState::new_view()
                << dds::sub::status::InstanceState::any();

        ReadCondHandler readCondHandler(newDataState);
        dds::sub::cond::ReadCondition readCond(dr, newDataState, readCondHandler);

        /** A query condition is created which is triggered when a message with the string "v_CurrentPosition > 110" is received */
       // std::cout << "=== [WaitSetDataSubscriber] Query : message = \"v_CurrentPosition > 110\"" << std::endl;
       // dds::sub::Query query(dr, "v_CurrentPosition > 110");
       // dds::sub::status::DataState anyDataState;
       // QueryCondHandler queryCondHandler;
       // dds::sub::cond::QueryCondition queryCond(query, anyDataState, queryCondHandler);

        /** A StatusCondition is created which is triggered when the DataWriter changes it's liveliness */
        StatusCondHandler statusCondHandler(prevAliveCount, writerLeft, guardTriggered);
        dds::core::cond::StatusCondition statusCond(dr, statusCondHandler);
        dds::core::status::StatusMask statusMask;
        statusMask << dds::core::status::StatusMask::liveliness_changed();
        statusCond.enabled_statuses(statusMask);

        /** A GuardCondition is created which will be used to close the subscriber */
        GuardCondHandler guardCondHandler(escaped, count);
        dds::core::cond::GuardCondition guardCond;
        guardCond.handler(guardCondHandler);

        /** A WaitSet is created and the four conditions created above are attached to it */
        dds::core::cond::WaitSet waitSet;
        waitSet += readCond;
       // waitSet += queryCond;
        waitSet += statusCond;
        waitSet += guardCond;

        dds::core::Duration waitTimeout(10, 0);

        std::cout << "=== [WaitSetDataSubscriber] Ready ..." << std::endl;

        while(!closed && count < 20)
        {
            /** Wait until at least one of the conditions in the WaitSet triggers and dispatch the corresponding functor*/
            try
            {
                waitSet.dispatch(waitTimeout);
                /** The GuardCondition is triggered if the StatusCondition determines that a DataWriter has lost it's liveliness */
                std::cout<<"Waitset block the thread: "<<std::endl;
                guardCond.trigger_value(guardTriggered);
            }
            catch(const dds::core::TimeoutError e)
            {
                std::cout << std::endl << "!!! [INFO] WaitSet timed out - count = " << count << std::endl;
            }
            ++count;
            /** The example will exit if the GuardCondition has been triggered */
            closed = escaped && writerLeft;
        }
        if(count >= 20)
        {
            std::cout << std::endl << "*** Error : Timed out - count = " << count << " ***" << std::endl;
        }
        else
        {
            std::cout << "=== [Subscriber] Closed" << std::endl;
        }
    }
    catch (const dds::core::Exception& e)
    {
        std::cerr << "ERROR: Exception: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
