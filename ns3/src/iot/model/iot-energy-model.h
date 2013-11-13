#ifndef IOT_ENERGY_MODEL_H
#define IOT_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/traced-value.h"
#include "ns3/ptr.h"
#include "ns3/pointer.h"

namespace ns3 {

class IotEnergyModelDeviceListener
{
  
};
  
  
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
private:
  //virtual double DoGetCurrentA (void) const;
  Ptr<EnergySource> m_source;
  
  // IotDevice listener
  IotEnergyModelDeviceListener *m_listener;
  
  // This variable keeps track of the total energy consumed by this model.
  TracedValue<double> m_totalEnergyConsumption;
  
  // Energy depletion callback
  IotEnergyDepletionCallback m_energyDepletionCallback;  
};
} // namespace ns3
#endif /* IOT_ENERGY_MODEL_H */