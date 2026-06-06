*This project has been created as part of the 42 curriculum by girizzi.*

# NetPractice

- [Description](#description)
    - [TCP/IP model](#tcpip-model)
    - [Internet addresses](#internet-addresses)
    - [Subnet masks](#subnet-masks)
    - [Default gateways](#default-gateways)
    - [Routers](#routers)
    - [Switches](#switches)
    - [OSI model](#osi-model)
- [Instructions](#instructions)
    - [Run the training interface](#run-the-training-interface)
    - [Export configurations](#export-configurations)
    - [Submission requirements](#submission-requirements)
- [Resources](#resources)

## Description

### TCP/IP model

TCP/IP (Transmission Control Protocol/Internet Protocol) is a set of standardized rules (or protocols) that allow computers to communicate on a network such as the internet. It was created by DARPA (an agency of the US Department of Defense) in the 1970s. TCP/IP provides end-to-end connectivity specifying how data should be formatted, addresses, transmitted, routed and received at the destination.

TCP/IP has four abstraction layers:

#### Application layer

The request/data to be sent is created locally on the device, with rules depending on the type of application. For example, an HTTP request will be created by the browser.

#### Transport layer

This layer describes how the data will be sent and received. The two main transfer protocols are TCP and UDP.

TCP (Transmission Control Protocol): The sender and receiver must communicate and establish a connection first before the data is sent. Once the destination receives the data, it must send an acknowledgement back to the source. This three-way handshake adds reliability but increases latency.

UDP (User Datagram Protocol): Messages are sent without negotiating a connection, and the data being sent is not kept track of. It is used for time-sensitive transmissions, such as video playback or DNS lookups.

#### Network (or internet) layer

The primary protocol in this layer is IP (Internet Protocol), which is used to deliver packets of information between two devices. The data is bundled with an IP packet header that includes the source and destination addresses. It is sent to the routers, which will use the IP header to handle and route the packet.

#### Link layer

This layer defines the networking methods used within the scope of the local network on which hosts and routers communicate.

Devices connected through ethernet use the MAC (Media access control) protocol instead of IP. The packet is bundled with the source's MAC address to create a frame, and sent to the ethernet switch which forwards it to the router. The router deconstructs the frame, takes out the packet and forwards it to its destination.

### Internet addresses

The two parts of an Internet address are the network address portion and the host address portion. This allows a remote host to specify both the remote network and the host on the remote network when sending information. By convention, a host number of 0 is used to refer to the network itself.

TCP/IP supports three classes of Internet addresses: Class A, Class B, and Class C. The different classes of Internet addresses are designated by how the 32 bits of the address are allocated. The particular address class a network is assigned depends on the size of the network.

- Class A addresses: consist of an 8-bit network address and a 24-bit local or host address.
- Class B addresses: consist of a 16-bit network address and a 16-bit local or host address.
- Class C addresses: consist of a 24-bit network address and an 8-bit local host address.
- Internet addresses using zeros: When a C class Internet address contains a 0 as the host address portion, (for example, 192.9.200.0), TCP/IP sends a wildcard address on the network.

### Subnet masks

When a host sends a message to a destination, the system must determine whether the destination is on the same network as the source or if the destination can be reached directly through one of the local interfaces. The system compares the destination address to the host address using the subnet mask.

The subnet mask is a set of 4 bytes, just like the Internet address. The subnet mask consists of high bits (1's) corresponding to the bit positions of the network and subnetwork address, and low bits (0's) corresponding to the bit positions of the host address. The destination address and the local network address are compared by performing the logical AND and exclusive OR on the subnet mask of the source host.

### Default gateways

If a TCP/IP computer needs to communicate with a host on another network, it will usually communicate through a device called a router. A default gateway is the router that links a host's subnet to other networks.

If the result of the comparison between the IP address and subnet mask determines the destination to be a remote host, then the computer will forward the packet to the default gateway defined in its TCP/IP properties. It's then the responsibility of the router to forward the packet to the correct subnet.

### Routers

Routers are devices that connect one network to another. They create local networks, control access to them, and route TCP/IP traffic on them. They also enable local network TCP/IP traffic to move to and from the Internet using Network Address Translation (NAT). Routers also use a firewall to restrict public Internet access to the local network.

### Switches

A switch is a device that enables the connection of multiple devices to the same network. Most routers for homes and small business have a built-in switch. As switches don't have the ability to use IP addresses, they instead use MAC addresses. A switch uses a routing table to associate the switch’s port number with the MAC address connected at the other end of the wire.

### OSI model

The Open Systems Interconnection (OSI) model is an alternative model to TCP/IP. They both describe how information is transmitted between devices across a network. However, the OSI model has 7 layers as opposed to TCP/IP's 4 layers.

- Application layer: Connects directly to end-user applications, providing various services (browsing, email, etc.).
- Presentation layer: Translates data between the formats used by an application when transmitting data over the network. It also manages encryption and data compression.
- Session layer: Responsible for session management, synchronization, and organizing traffic.
- Transport layer: Responsible for ensuring that data is completely and reliably transferred between networks.
- Network layer: Routes, forwards, and addresses packets between networks.
- Data link layer: Manages the transfer of data between devices on the network and corrects errors from the physical layer.
- Physical layer: Includes the physical components connecting devices (cables, routers, etc.).

## Instructions

### Run the training interface

Download

```shell
wget "https://cdn.intra.42.fr/document/document/48830/net_practice.1.9.tgz"
```

Extract

```shell
tar -xvf "net_practice.1.9.tgz" && rm -rf "net_practice.1.9.tgz"
```

Run

```shell
cd net_practice && ./run.sh
```

### Export configurations

On the web interface, simply click on "Get my config" to export and save it to file.

### Submission requirements

- 10 configuration files, one for each level, exported using the `Get my config` button.
- README.md

## Resources

- [Understand TCP/IP addressing and subnetting basics (Microsoft Learn)](https://learn.microsoft.com/en-us/troubleshoot/windows-client/networking/tcpip-addressing-and-subnetting)
- [RFC 1878: Variable Length Subnet Table For IPv4](https://www.rfc-editor.org/info/rfc1878/)
- [IP Addressing and Subnetting Explained (Medium)](https://medium.com/@noderin1/ip-addressing-and-subnetting-explained-0ff5b5970dbf)
- [Internet protocol suite (Wikipedia)](https://en.wikipedia.org/wiki/Internet_protocol_suite)
- [ELI5: What exactly is TCP/IP?  (Reddit)](https://www.reddit.com/r/explainlikeimfive/comments/swwcyw/eli5_what_exactly_is_tcpip/)
- [Transmission Control Protocol (Wikipedia)](https://en.wikipedia.org/wiki/Transmission_Control_Protocol)
- [User Datagram Protocol (Wikipedia)](https://en.wikipedia.org/wiki/User_Datagram_Protocol)
- [Introduction to TCP/IP (Part 1) - Routers, Switches and Addressing](https://developerhelp.microchip.com/xwiki/bin/view/applications/tcp-ip/routers-switches-addressing/#HSwitchesinLocalNetworks)
- [OSI model (Wikipedia)](https://en.wikipedia.org/wiki/OSI_model)
