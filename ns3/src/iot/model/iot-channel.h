/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* Copyright (c) 2013 hansbogert@gmail.com
 *
 * Edits made to original Simple{Channel|NetDevice} for the IoT seminar @TUDelft
 * Following original license holds
 *
 * Author: Hans van den Bogert <hansbogert@gmail.com>
 */

/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef IOT_CHANNEL_H
#define IOT_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/mac48-address.h"
#include <vector>

namespace ns3 {

class IotNetDevice;
class Packet;

/**
 * \ingroup channel
 * \brief A iot channel, for simple things and testing
 */
class IotChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  IotChannel ();

  void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
             Ptr<IotNetDevice> sender);

  void Add (Ptr<IotNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  std::vector<Ptr<IotNetDevice> > m_devices;
};

} // namespace ns3

#endif /* IOT_CHANNEL_H */
