/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* Copyright (c) 2013 hansbogert@gmail.com
 * Author: Hans van den Bogert <hansbogert@gmail.com>
 * 
 */
// Include a header file from your module to test.
#include "ns3/iot.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/iot-channel.h"
#include "ns3/iot-net-device.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"



// An essential include is test.h
#include "ns3/test.h"

NS_LOG_COMPONENT_DEFINE ("IotTestSuite");

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

class IotTestCasePacketGoesThroughChannel : public TestCase
{
public:
  IotTestCasePacketGoesThroughChannel ();
  virtual ~IotTestCasePacketGoesThroughChannel ();
  bool ReceiveCb2(Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &);
  bool ReceiveCbBase(Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &);
private:
  virtual void DoRun (void);
  bool received2;
  bool receivedBase;

};

// Add some help text to this case to describe what it is intended to test
IotTestCasePacketGoesThroughChannel::IotTestCasePacketGoesThroughChannel ()
  : TestCase ("Iot test case for iot devices and channel")
{
    received2 = false;
    receivedBase = false;

}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
IotTestCasePacketGoesThroughChannel::~IotTestCasePacketGoesThroughChannel ()
{
}

bool IotTestCasePacketGoesThroughChannel::ReceiveCb2(
    Ptr< NetDevice >  device
  , ns3::Ptr< const Packet > p 
  , uint16_t protocol
  , const Address& dest)
{
  NS_LOG_FUNCTION("ReceiveCb2");
  NS_LOG_INFO("Received by " << device->GetNode()->GetId());
  received2 = true;
  return true;
}

bool IotTestCasePacketGoesThroughChannel::ReceiveCbBase(
    Ptr< NetDevice >  device
  , ns3::Ptr< const Packet > p 
  , uint16_t protocol
  , const Address& dest)
{
  NS_LOG_FUNCTION("ReceiveCbBase");
  NS_LOG_INFO("Received by " << device->GetNode()->GetId());
  receivedBase = true;
  return true;
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
IotTestCasePacketGoesThroughChannel::DoRun (void)
{
  NS_LOG_UNCOND ("Starting test");
  /*Inits*/
  Ptr<Node> node1    = CreateObject<Node>();
  Ptr<Node> node2    = CreateObject<Node>();
  Ptr<Node> nodeBase = CreateObject<Node>();
  
  Ptr<IotNetDevice> device12    = CreateObject<IotNetDevice>();
  Ptr<IotNetDevice> device21    = CreateObject<IotNetDevice>();
  Ptr<IotNetDevice> device2Base = CreateObject<IotNetDevice>();
  Ptr<IotNetDevice> deviceBase2 = CreateObject<IotNetDevice>();
  
  Mac48Address address12   ("00:00:00:00:00:01");
  Mac48Address address21   ("00:00:00:00:00:02");
  Mac48Address address2Base("00:00:00:00:00:03");
  Mac48Address addressBase2("00:00:00:00:00:0f");
  
  Ptr<IotChannel> channel12    = CreateObject<IotChannel>();
  Ptr<IotChannel> channel2Base = CreateObject<IotChannel>();
  
  Ptr<Packet> packet = Create<Packet>();

  /* setup */
  node1->   AddDevice(device12);
  node2->   AddDevice(device2Base);
  node2->   AddDevice(device21);
  nodeBase->AddDevice(deviceBase2);
  
  device12->   SetAddress(address12);
  device21->   SetAddress(address21);
  device2Base->SetAddress(address2Base);
  deviceBase2->SetAddress(addressBase2);
  
  device12->SetChannel(channel12);
  device21->SetChannel(channel12);

  device2Base->SetChannel(channel2Base);
  deviceBase2->SetChannel(channel2Base);
  
  Callback<bool,Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &> receive2;
  receive2 = MakeCallback(&IotTestCasePacketGoesThroughChannel::ReceiveCb2, this);

  Callback<bool,Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &> receiveBase;
  receiveBase = MakeCallback(&IotTestCasePacketGoesThroughChannel::ReceiveCbBase, this);
  
  device21->   SetReceiveCallback(receive2);
  deviceBase2->SetReceiveCallback(receiveBase);

  
  /* Simulation */
  device12->Send(packet, Mac48Address("00:00:00:00:00:0f"), 0);
  
  Simulator::Run();
  
  /* asserts */
  
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, received2, "packet did not went through device2");
  NS_TEST_ASSERT_MSG_EQ (true, receivedBase, "packet did not make it to the base");

  /*
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
  */
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class IotTestSuite : public TestSuite
{
public:
  IotTestSuite ();
};

IotTestSuite::IotTestSuite ()
  : TestSuite ("iot", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new IotTestCasePacketGoesThroughChannel, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static IotTestSuite iotTestSuite;

