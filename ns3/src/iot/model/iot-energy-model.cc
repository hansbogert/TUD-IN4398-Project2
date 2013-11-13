#include "ns3/log.h"
#include "ns3/double.h"
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
  m_source = source;
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
}