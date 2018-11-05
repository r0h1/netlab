#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

int
main(int argc,char* argv[])
{
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);	
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	
	//creating 4 node mesh topology
	NodeContainer mesh;
	mesh.Create(4);
	
	InternetStackHelper stack;
	stack.Install(mesh);

	NodeContainer csmaNodes1;
	csmaNodes1.Add(mesh.Get(0));
	csmaNodes1.Add(mesh.Get(1));
	csmaNodes1.Add(mesh.Get(2));
	csmaNodes1.Add(mesh.Get(3));

	NodeContainer csmaNodes2;
	csmaNodes2.Add(mesh.Get(1));
	csmaNodes2.Add(mesh.Get(2));
	csmaNodes2.Add(mesh.Get(3));

	NodeContainer p2pNodes;
	p2pNodes.Add(mesh.Get(1));
	p2pNodes.Add(mesh.Get(3));
	
	CsmaHelper csma;
	csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
	csma.SetChannelAttribute("Delay", TimeValue (NanoSeconds(6560)));

	NetDeviceContainer csma1Devices;
	csma1Devices = csma.Install (csmaNodes1);
	
	NetDeviceContainer csma2Devices;
	csma2Devices = csma.Install(csmaNodes2);
	
	PointToPointHelper pointToPoint;
  	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
	
	NetDeviceContainer p2pDevices;
	p2pDevices = pointToPoint.Install(p2pNodes);
	
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
  	Ipv4InterfaceContainer csma1Interfaces;
  	csma1Interfaces = address.Assign (csma1Devices);

	
	address.SetBase ("10.1.2.0", "255.255.255.0");
  	Ipv4InterfaceContainer csma2Interfaces;
  	csma2Interfaces = address.Assign (csma2Devices);
		
	address.SetBase ("10.1.3.0", "255.255.255.0");
	Ipv4InterfaceContainer p2pInterfaces;
	p2pInterfaces = address.Assign(p2pDevices);

	UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (csmaNodes1.Get (0));
  	serverApps.Start (Seconds (1.0));
  	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient (csma1Interfaces.GetAddress (0),9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue(1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
	
	ApplicationContainer clientApps = echoClient.Install (csmaNodes2.Get (0));
	clientApps.Start (Seconds (2.0));
 	clientApps.Stop (Seconds (10.0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
	
	pointToPoint.EnablePcapAll ("second");
	csma.EnablePcap ("second", csma1Devices.Get (1), true);
	csma.EnablePcap ("second", csma2Devices.Get (1), true);

	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}
