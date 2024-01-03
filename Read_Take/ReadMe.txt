..
   Copyright(c) 2006 to 2022 ZettaScale Technology and others

   This program and the accompanying materials are made available under the
   terms of the Eclipse Public License v. 2.0 which is available at
   http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
   v. 1.0 which is available at
   http://www.eclipse.org/org/documents/edl-v10.php.

   SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause

VehicleData
==========

Description
***********

The basic ddscxx VehicleData example is used to illustrate the necessary steps to setup DCPS entities.

Design
******

It consists of 2 units:

- ddscxxSimplePubSub_VehicleDataPublisher: implements the publisher's main
- ddscxxSimplePubSub_VehicleDataSubscriber: implements the subscriber's main

Scenario
********

The publisher sends a single Vehicle Data sample. The sample contains three fields:

- a Vehicle Id (v_id) field : (type string)
- a Vehicle Current Position (v_CurrentPosition) field : it contains latitude (type double) and longitude (type double)
- a Vehicle Current Speed (v_CurrentSpeed): (type double)

When it receives the sample, the subscriber displays the Vehicle Id, Vehicle Current Position and the Vehicle Current Speed.

Running the example
*******************

It is recommended that you run the subscriber and publisher in separate terminals to avoid mixing the output.

- Open 2 terminals.
- In the first terminal start the subscriber by running ddscxxSimplePubSub_VehicleDataPublisher
- In the second terminal start the publisher by running ddscxxSimplePubSub_VehicleDataSubscriber
- InddscxxSimplePubSub_VehicleDataSubscriber you have to option to process the data from the reader queue
	- read -> if you choose read, then you can re-read the sample again
	- take -> if you choose take, then you get the data and data will be deleted from the reader queue. when you try to re-read or re-take then you will not get the data.

































==============================================================================
Step:1
	>mkdir build && cd build

Step:2
	>cmake ..

Step:3
	>cmake --build .

Step:4
-Start Subsctriber
 	>ddscxxVehicleDataSubscriber

Step:5
-Start Publisher
	>ddscxxVehicleDataPublisher
