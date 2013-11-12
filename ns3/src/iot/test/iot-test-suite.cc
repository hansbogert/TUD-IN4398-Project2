/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
  bool ReceiveCb(Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &);
private:
  virtual void DoRun (void);
  bool received;
};

// Add some help text to this case to describe what it is intended to test
IotTestCasePacketGoesThroughChannel::IotTestCasePacketGoesThroughChannel ()
  : TestCase ("Iot test case (does nothing)")
{
    received = false;
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
IotTestCasePacketGoesThroughChannel::~IotTestCasePacketGoesThroughChannel ()
{
}

bool IotTestCasePacketGoesThroughChannel::ReceiveCb(
    Ptr< NetDevice > 
  , ns3::Ptr< const Packet > 
  , uint16_t 
  , const Address& )
{
  NS_LOG_FUNCTION("ReceiveCb");
  received = true;
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
  Ptr<Node> node1 = CreateObject<Node>();
  Ptr<Node> node2 = CreateObject<Node>();
  
  Ptr<IotNetDevice> device1 = CreateObject<IotNetDevice>();
  Ptr<IotNetDevice> device2 = CreateObject<IotNetDevice>();
  
  Mac48Address address1("00:00:00:00:00:00");
  Mac48Address address2("00:00:00:00:00:01");
  
  Ptr<IotChannel> channel = CreateObject<IotChannel>();
  
  Ptr<Packet> packet = Create<Packet>();

  /* setup */
  node1->AddDevice(device1);
  node2->AddDevice(device2);
  
  device1->SetAddress(address1);
  device2->SetAddress(address2);
  
  device1->SetChannel(channel);
  device2->SetChannel(channel);
  
  Callback<bool,Ptr<NetDevice>,Ptr<const Packet>,uint16_t,const Address &> receive;
  receive = MakeCallback(&IotTestCasePacketGoesThroughChannel::ReceiveCb, this);
  
  device2->SetReceiveCallback(receive);
  
  
  /* Simulation */
  device1->Send(packet, device2->GetAddress(), 0);
  
  Simulator::Run();
  
  /* asserts */
  
  
  
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, received, "callback was apparently not called");
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

