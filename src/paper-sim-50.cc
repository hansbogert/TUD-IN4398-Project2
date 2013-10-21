/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include <ns3/position-allocator.h>
#include <ns3/mobility-helper.h>
#include <ns3/vector.h>
#include <ns3/waypoint-mobility-model.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/basic-energy-source-helper.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
const int NSIZE = 50;
const int WCV_SPEED = 5; // in meter per second
const int BATTERY_CAPACITY = 10800; // in Joules

  //location/positions of the nodes, in exact meters.
const Vector3D positions[NSIZE] =
    {  Vector3D (815, 276, 0),
       Vector3D (906, 680, 0),
       Vector3D (127, 655, 0),
       Vector3D (913, 163, 0),
       Vector3D (632, 119, 0),
       Vector3D (98,  498, 0),
       Vector3D (278, 960, 0),
       Vector3D (547, 340, 0),
       Vector3D (958, 585, 0),
       Vector3D (965, 224, 0),
       Vector3D (158, 751, 0),
       Vector3D (971, 255, 0),
       Vector3D (957, 506, 0),
       Vector3D (485, 699, 0),
       Vector3D (800, 891, 0),
       Vector3D (142, 959, 0),
       Vector3D (422, 547, 0),
       Vector3D (916, 139, 0),
       Vector3D (792, 149, 0),
       Vector3D (959, 258, 0),
       Vector3D (656, 841, 0),
       Vector3D (36,  254, 0),
       Vector3D (849, 814, 0),
       Vector3D (934, 244, 0),
       Vector3D (679, 292, 0),
       Vector3D (758, 350, 0),
       Vector3D (743, 197, 0),
       Vector3D (392, 251, 0),
       Vector3D (655, 616, 0),
       Vector3D (171, 473, 0),
       Vector3D (706, 352, 0),
       Vector3D (32,  831, 0),
       Vector3D (277, 585, 0),
       Vector3D (46,  550, 0),
       Vector3D (97,  917, 0),
       Vector3D (823, 286, 0),
       Vector3D (695, 757, 0),
       Vector3D (317, 754, 0),
       Vector3D (950, 380, 0),
       Vector3D (34,  568, 0),
       Vector3D (439, 76,  0),
       Vector3D (382, 54,  0),
       Vector3D (766, 531, 0),
       Vector3D (795, 779, 0),
       Vector3D (187, 934, 0),
       Vector3D (490, 130, 0),
       Vector3D (446, 569, 0),
       Vector3D (646, 469, 0),
       Vector3D (709, 12,  0),
       Vector3D (755, 337, 0)
    };

// the rates for the nodes
const int rates[NSIZE] = { 1, 8, 4, 6, 3, 7, 3, 7, 6, 8, 5, 1, 4, 10, 2, 9, 5, 10, 1, 5, 3, 10, 1, 8 ,8 
                           , 9, 1, 4, 4, 9, 5, 10, 1, 3, 2, 2, 8, 6, 6, 2, 8, 7, 4, 6, 6, 1, 3, 2, 2, 3};

// set the order the WCV will walk, corresponds to table II of the
// paper, i.e. counter-clock-wise solution
const int order[50] = { 42, 41, 46, 28, 8, 48, 43, 31, 26, 50, 36, 1, 27, 5, 49, 19, 18, 4, 10, 24, 20, 12, 39, 13, 9
                  , 2, 44, 23, 15, 25, 21, 37, 29, 14, 47, 17, 33, 38, 7, 45, 16, 35, 32, 11, 3, 40, 34, 6, 30, 22 };

const int arrivalTimes[50] = { 110702, 110725, 110777, 111113, 111776, 112461, 114579, 114660, 115627, 115639
                         , 115855, 115997, 116273, 116353, 116412, 116484, 116538, 116581, 116696, 116747
                         , 116997, 117006, 117032, 117534, 117717
                         , 117778, 117848, 117903, 117923, 117960, 118002, 118065, 118519, 119056, 119192
                         , 119246, 119614, 120936, 121142, 121171, 121213, 121239, 121380, 121894, 122090
                         , 123039, 123046, 123073, 123151, 123766};

const int chargingTimes[50] = { 11, 37, 305, 627, 653, 2092, 43, 957, 10, 199, 139, 254, 53, 33, 40, 29, 38, 100, 43, 245, 6, 1, 477, 167, 40
                          , 41, 42, 2, 11, 25, 44, 425, 499, 109, 47, 338, 1287, 164, 10, 31, 14, 120, 483, 176, 924, 2, 12, 63, 563, 1166 };

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  /* Node initialization **/
  /** Represents the sensor nodes  */
  NodeContainer nodes;
  nodes.Create (NSIZE);

  /** Represents the wcv which can charge the fixed sensor nodes */
  NodeContainer wcv;
  wcv.Create (1);

  /** Represents the sink station to which all sensor nodes eventually
     send their data, albeit through other nodes. */
  NodeContainer sink;
  sink.Create (1);

  /* Position handling **/
  /** Nodes positioning */

  Ptr<ListPositionAllocator> nodeListPositionAllocator = CreateObject<ListPositionAllocator> ();

  for (int i=0; i<NSIZE; i++) {
      nodeListPositionAllocator->Add ( positions[i] );
  }

  MobilityHelper mobility;
  mobility.SetPositionAllocator ( nodeListPositionAllocator );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install ( nodes ); 
  
  /** WCV positioning */
  mobility.SetMobilityModel ( "ns3::WaypointMobilityModel" );
  mobility.Install ( wcv );
  Ptr<WaypointMobilityModel> waypointMobility = wcv.Get (0)->GetObject<WaypointMobilityModel> ();
  
  int startTime = 11625;
  Time startWaypointTime(startTime);
  Vector3D origin(0, 0, 0);
  Waypoint firstWaypoint(startWaypointTime, origin);
  waypointMobility->AddWaypoint(firstWaypoint);

  for (int i=0; i<NSIZE; i++){
    int time = arrivalTimes[i];
    Time arrivalWaypointTime (time);
    Waypoint arrivalWaypoint (arrivalWaypointTime, positions[order[i]]);
    //add arrival waypoint
    waypointMobility->AddWaypoint (arrivalWaypoint);
    //add waypoint on same location but after chargingTime
    time += chargingTimes[i];
    Time chargedWaypointTime (time);
    Waypoint chargedWaypoint (chargedWaypointTime, positions[i]);
    waypointMobility->AddWaypoint(chargedWaypoint);
  }

  /** Sink position */
  Vector3D vSink(500, 500, 0);
  Ptr<ListPositionAllocator> sinkListPositionAllocator = CreateObject<ListPositionAllocator> ();
  sinkListPositionAllocator->Add (vSink);
  mobility.SetPositionAllocator (sinkListPositionAllocator);
  mobility.SetMobilityModel ( "ns3::ConstantPositionMobilityModel" );
  mobility.Install (sink);
  
  

  /* Layer 2 network */
  /*

  PointToPointHelper pointToPoint;

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  
  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  //Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("myfirst.tr"));
  */

  /* Energy setup */
  BasicEnergySourceHelper basicSourceHelper;
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (BATTERY_CAPACITY));
  EnergySourceContainer nodeSources = basicSourceHelper.Install (nodes);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
