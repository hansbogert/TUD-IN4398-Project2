#ifndef IOT_ENERGY_MODEL_H
#define IOT_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/packet.h"
#include "ns3/iot-energy-source.h"
#include "ns3/traced-value.h"
#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/iot-net-device.h"

namespace ns3 {

class IotEnergyModelDeviceListener
{
  
};
  
class Packet;  
class IotEnergyModel : public DeviceEnergyModel
{
public:
  typedef Callback<void> IotEnergyDepletionCallback;
public:
  static TypeId GetTypeId (void);
  IotEnergyModel ();
  virtual ~IotEnergyModel();
  virtual void 	ChangeState (int newState);
  virtual double GetTotalEnergyConsumption (void) const;
  void SetEnergyDepletionCallback (IotEnergyDepletionCallback callback);
  virtual void 	HandleEnergyDepletion (void);
  virtual void 	SetEnergySource (Ptr< EnergySource > source);
  IotEnergyModelDeviceListener * GetDeviceListener (void);
  IotNetDevice::SendCallback GetIotNetDeviceSendCallback (void);
  IotNetDevice::ReceiveCallback GetIotNetDeviceReceiveCallback (void);

  bool IotNetDeviceSendCallback (Ptr<NetDevice>, Ptr<Packet> p, const Address& dest, uint16_t protocol);
  bool IotNetDeviceReceiveCallback (Ptr<NetDevice>, Ptr<const Packet> p,  uint16_t protocol, const Address& dest);
  virtual void SetDistanceToUpstream (double distance); // in meters
  virtual void SetB1Constant (double b1); // in J
  virtual void SetB2Coefficient (double b2); // in J
  virtual void SetAPathLossIndex (double a); // Loss index, see paper
  virtual void SetRhoReceiveConstant (double a); // cost for receiving bits, see paper


private:
  //virtual double DoGetCurrentA (void) const;
  Ptr<IotEnergySource> m_source;

  // IotDevice listener
  IotEnergyModelDeviceListener *m_listener;

  // This variable keeps track of the total energy consumed by this model.
  TracedValue<double> m_totalEnergyConsumption;

  // Energy depletion callback
  IotEnergyDepletionCallback m_energyDepletionCallback;

  double m_distanceToUpStream;
  double m_b1Constant; //in J/b
  double m_b2Coefficient; //in J/(b/m^4)
  double m_aPathLossIndex;
  double m_rhoReceiveConstant;
};
} // namespace ns3
#endif /* IOT_ENERGY_MODEL_H */