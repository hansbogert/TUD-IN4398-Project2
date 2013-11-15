/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* Copyright (c) 2013 hansbogert@gmail.com
 *
 * Edits made to original BasicEnergySource for the IoT seminar @TUDelft
 * Following original license holds
 *
 * Author: Hans van den Bogert <hansbogert@gmail.com>
 */
/*
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
 *
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
 *
 * Authors: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#ifndef IOT_ENERGY_SOURCE_H
#define IOT_ENERGY_SOURCE_H

#include "ns3/traced-value.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/energy-source.h"

namespace ns3 {

/**
 * \ingroup energy
 * IotEnergySource decreases/increases remaining energy stored in itself in
 * linearly.
 *
 */
class IotEnergySource : public EnergySource
{
public:
  static TypeId GetTypeId (void);
  IotEnergySource ();
  virtual ~IotEnergySource ();

  /**
   * \return Initial energy stored in energy source, in Joules.
   *
   * Implements GetInitialEnergy.
   */
  virtual double GetInitialEnergy (void) const;

  /**
   * \returns Supply voltage at the energy source.
   *
   * Implements GetSupplyVoltage.
   */
  virtual double GetSupplyVoltage (void) const;

  /**
   * \return Remaining energy in energy source, in Joules
   *
   * Implements GetRemainingEnergy.
   */
  virtual double GetRemainingEnergy (void);

  /**
   * \returns Energy fraction.
   *
   * Implements GetEnergyFraction.
   */
  virtual double GetEnergyFraction (void);

  /**
   * Implements UpdateEnergySource.
   */
  virtual void UpdateEnergySource (void);

  /**
   * \param initialEnergyJ Initial energy, in Joules
   *
   * Sets initial energy stored in the energy source. Note that initial energy
   * is assumed to be set before simulation starts and is set only once per
   * simulation.
   */
  void SetInitialEnergy (double initialEnergyJ);

  /**
   * \param supplyVoltageV Supply voltage at the energy source, in Volts.
   *
   * Sets supply voltage of the energy source.
   */
  void SetSupplyVoltage (double supplyVoltageV);

  /**
   * \param interval Energy update interval.
   *
   * This function sets the interval between each energy update.
   */
  void SetEnergyUpdateInterval (Time interval);

  /**
   * \returns The interval between each energy update.
   */
  Time GetEnergyUpdateInterval (void) const;

  /**
   * This alternate version derived from the original BasicEnergySource has this added
   * function to substract batches of energy in contrast to continuous models 
   * heavily unfluenced the original energy-framework, this is in no way real-life, 
   * but the paper simulation also does expresses energy in terms of sent bytes. (i.e. 
   * no electrical current information, nor link speeds, so nothing can be inferred)
   * \param substract substract amount of energy (J) from remaining.
   */
  void SubstractEnergy (double substract);

private:
  /// Defined in ns3::Object
  void DoInitialize (void);

  /// Defined in ns3::Object
  void DoDispose (void);

  /**
   * Handles the remaining energy going to zero event. This function notifies
   * all the energy models aggregated to the node about the energy being
   * depleted. Each energy model is then responsible for its own handler.
   */
  void HandleEnergyDrainedEvent (void);

  /**
   * Calculates remaining energy. This function uses the total current from all
   * device models to calculate the amount of energy to decrease. The energy to
   * decrease is given by:
   *    energy to decrease = total current * supply voltage * time duration
   * This function subtracts the calculated energy to decrease from remaining
   * energy.
   */
  void CalculateRemainingEnergy (void);
  


  

private:
  double m_initialEnergyJ;                // initial energy, in Joules
  double m_supplyVoltageV;                // supply voltage, in Volts
  TracedValue<double> m_remainingEnergyJ; // remaining energy, in Joules
  EventId m_energyUpdateEvent;            // energy update event
  Time m_lastUpdateTime;                  // last update time
  Time m_energyUpdateInterval;            // energy update interval

};

} // namespace ns3

#endif /* IOT_ENERGY_SOURCE_H */