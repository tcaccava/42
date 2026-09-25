*This project has been created as part of the 42 curriculum by fmanno.*

# NetPractice

## Description

NetPractice is a networking project based on a series of exercises where the goal is to fix different network configurations.

Each level contains hosts, switches, routers and routing tables with some missing or incorrect values. The goal is to configure IP addresses, subnet masks and routes so that the devices can communicate correctly.

The project is useful to understand how IPv4 networks work.

Concepts important for the project are:

- TCP/IP addressing
- IPv4 addresses
- subnet masks and CIDR notation
- network and broadcast addresses
- host ranges
- default gateways
- routing tables
- destination networks and next hops
- routers and switches
- communication between different subnets

A particularly important part of the project is learning how to calculate the network an IP belongs to.

## Instructions

Download and extract the files provided with the project.

From the project directory, run:

```bash
./run.sh
```

This starts a local web server and opens the NetPractice interface in the browser.

Then open:

```text
http://localhost:49242
```

on your browser.

Before starting the exercises, you can enter your 42 login in the interface.

For every level:

1. Configure the network until all objectives are completed.
2. Use `Check again` to test the configuration.
3. Click `Get my config` before moving to next level, in order to save and export your configuration solutions.

There are 10 training levels, so the final repository must contain **10 exported configuration files**, one for each level.

All 10 configuration files must be placed at the root of the repository with this README.

During the evaluation, random network configurations must be solved in 15 minutes of time without the use of external tools. Only a simple calculator, such as `bc`, is allowed.

## Useful Concepts

### IP Address and Subnet Mask

An IPv4 address contains 32 bits divided into four octets.

The subnet mask determines which part of the address identifies the network and which part can be used for hosts.

CIDR notation is another way to represent the subnet mask.

For example:

```text
/24 = 255.255.255.0
/25 = 255.255.255.128
/26 = 255.255.255.192
/27 = 255.255.255.224
/28 = 255.255.255.240
/29 = 255.255.255.248
/30 = 255.255.255.252
```

The subnet mask is also used to calculate the network address, broadcast address and usable host range.

### Routers and Switches

A switch connects devices inside the same local network.

A router connects different networks. A router normally has several network interfaces, with each interface belonging to a different subnet.

### Routing Tables

A routing table tells a device where packets should be sent.

A route contains:

- `destination`: the network for which the rule applies
- `next hop`: the next router or interface that should receive the packet

A default route (`0.0.0.0/0`) is used when there is no more specific route available.

For a communication to work correctly, both the forward path and the return path must be valid.

## Resources

Some useful references for the networking concepts used in this project:

- RFC 791 - Internet Protocol
- RFC 950 - Internet Standard Subnetting Procedure
- RFC 4632 - Classless Inter-domain Routing (CIDR)
- Cisco networking documentation
- Linux `ip` command documentation
- 42 NetPractice subject and training interface

The main topics studied while preparing the project were IPv4 addressing, TCP/IP, subnet masks, CIDR notation, network and broadcast addresses, routing tables, default gateways, routers, switches and basic OSI/TCP-IP layer concepts.

### AI Usage

AI was used to study the theorical concepts during the preparation of the project.

It was mainly used to:

- clarify networking concepts
- understand routing tables, destinations and next hops
- organize and produce study notes
- help review this README

