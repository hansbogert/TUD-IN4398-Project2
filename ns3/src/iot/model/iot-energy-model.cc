#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/energy-source.h"
#include "iot-energy-model.h"

NS_LOG_COMPONENT_DEFINE ("IotEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (IotEnergyModel);

TypeId
IotEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IotEnergyModel")
  .SetParent<DeviceEnergyModel>()
  .AddConstructor<IotEnergyModel>();
  return tid;
}

IotEnergyModel::IotEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  m_source = NULL;
  m_listener = new IotEnergyModelDeviceListener;
}

IotEnergyModel::~IotEnergyModel ()
{
  NS_LOG_FUNCTION (this);
  delete m_listener;
}

void
IotEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = DynamicCast<IotEnergySource>(source);
}

double
IotEnergyModel::GetTotalEnergyConsumption (void) const
{
  NS_LOG_FUNCTION (this);
  return m_totalEnergyConsumption;
}

void
IotEnergyModel::SetEnergyDepletionCallback (
  IotEnergyDepletionCallback callback)
{
  NS_LOG_FUNCTION (this);
  if (callback.IsNull ())
    {
      NS_LOG_DEBUG ("IotEnergyModel:Setting NULL energy depletion callback!");
    }
  m_energyDepletionCallback = callback;
}

void IotEnergyModel::ChangeState(int newState)
{
  NS_LOG_FUNCTION (this << newState << "Not implemented");
}

void
IotEnergyModel::HandleEnergyDepletion (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("IotEnergyModel:Energy is depleted!");
  // invoke energy depletion callback, if set.
  if (!m_energyDepletionCallback.IsNull ())
    {
      m_energyDepletionCallback ();
    }
}

IotEnergyModelDeviceListener *
IotEnergyModel::GetDeviceListener (void)
{
  NS_LOG_FUNCTION (this);
  return m_listener;
}

bool 
IotEnergyModel::IotNetDeviceSendCallback (Ptr<NetDevice> device, Ptr<Packet> p, const Address& dest, uint16_t protocol){
  NS_LOG_FUNCTION (this << p->GetSize());
  NS_LOG_FUNCTION("Upstream is " << m_distanceToUpStream << "m away");
  NS_LOG_DEBUG ( "parameters" << m_b1Constant << m_b2Coefficient << m_aPathLossIndex );
  uint16_t bitsSend = p->GetSize () * 8;
  double consumption = (m_b1Constant * bitsSend)
		     + (m_b2Coefficient
		       * bitsSend
		       * pow (m_distanceToUpStream, m_aPathLossIndex)
		       );

  m_source->SubstractEnergy (consumption);
  return true;
}

bool 
IotEnergyModel::IotNetDeviceReceiveCallback (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,  const Address& dest)
{
  NS_LOG_FUNCTION (this << p->GetSize());
  NS_LOG_FUNCTION ("Upstream is " << m_distanceToUpStream << "m away");
  NS_LOG_DEBUG ( "parameters" << m_b1Constant << m_b2Coefficient << m_aPathLossIndex );
  uint16_t bitsReceived = p->GetSize () * 8;
  double consumption = m_rhoReceiveConstant * bitsReceived;

  m_source->SubstractEnergy(consumption);
  return true;
}



IotNetDevice::SendCallback IotEnergyModel::GetIotNetDeviceSendCallback (void )
{
  Callback<bool, Ptr<NetDevice>, Ptr<Packet>, const Address&, uint16_t> send;
  send = MakeCallback(&IotEnergyModel::IotNetDeviceSendCallback, this);
  return send;
}

IotNetDevice::ReceiveCallback IotEnergyModel::GetIotNetDeviceReceiveCallback (void )
{
  Callback<bool, Ptr<NetDevice>, Ptr<const Packet>, uint16_t, const Address& > receive;
  receive = MakeCallback(&IotEnergyModel::IotNetDeviceReceiveCallback, this);
  return receive;
}

void IotEnergyModel::SetDistanceToUpstream (double distance)
{
  m_distanceToUpStream = distance;
}

void IotEnergyModel::SetB1Constant (double b1)
{
  m_b1Constant = b1;
}

void IotEnergyModel::SetB2Coefficient (double b2)
{
  m_b2Coefficient = b2;
}

void IotEnergyModel::SetAPathLossIndex (double a)
{
  m_aPathLossIndex = a;
}

void IotEnergyModel::SetRhoReceiveConstant (double r)
{
  m_rhoReceiveConstant = r;
}

} //end namespace ns3

