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

- ddscxxDurability_VehicleDataPublisher: implements the publisher's main
- ddscxxDurability_VehicleDataSubscriber: implements the subscriber's main

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
- In the first terminal start the subscriber by running ddscxxDurability_VehicleDataPublisher
- In the second terminal start the publisher by running ddscxxDurability_VehicleDataSubscriber
- After some time In the third terminal start the subscriber by running ddscxxDurability_VehicleDataSubscriber (to simulate the late joiner), you can get the data.

