# Netsight: 5G Network Emulator

ITGSS Extended Reality Lab in Azerbaijan is focused on Immersive Media Technologies, with a special focus on what we denominated Distributed Reality (DR). The goal of (DR) is to be able to merge different local realities into a single real-time remote immersive experience. One key feature of DR is to allow the users to see his/her own hands and body within a fully immersive VR experience. However, this goal requires high-end hardware to run in real-time. Consequently, we are currently working on offloading architectures and implementations which allow us to run our demanding algorithms (AI/ML) on light VR/AR devices in real-time. 

5G technologies should be theoretically able to fulfil the network requirements of real-time task offloading. However, 5G technologies still have a long path ahead to reach their full potential. Consequently, we decided to implement our own 5G networks and beyond emulator to test and optimize our own solutions, determining which configuration (not only 5G, but also beyond) satisfy the requirements.

##  Goals

There are several emulators publicly available online, such as NS-3, SimuLTE or Simul5G. However, their complexity is extremely high, especially for non-telecommunications-specialists. We present FikoRE, our real-time 5G Radio Access Network (RAN) emulator carefully designed for application-level experimentation and prototyping. Its modularity and straightforward implementation allow multidisciplinary user to rapidly use or even modify it to test their own applications. Contrarily to the mentioned simulators/emulators, the goal of FikoRE is not mainly to understand and test the network, but study how the network and its different possible configurations behave for particular applications, use-cases and verticals.  

As we want to test our solutions in with actual networked applications we designed FikoRE to:  

* Work in Real-Time.
* Handle actual IP traffic efficiently. 
* Handle multiple emulated users with real or simulated traffic. 
* Model the real behavior of the network with sufficient accuracy. 

Besides, we need the emulator to be simple to use and, more importantly, easy to modify for possible particular needs. Consequently, FikoRE:  

* Has a high level of modularity to allow easy modifications.  
* Follows a straightforward implementation.  
* Is simple to use both as an emulator (real traffic) and simulator (simulated traffic).  

Furthermore, as the goal is to test actual applications on different possible scenarios and understand the most optimal network configurations, we understand is crucial to allow to easily modify the resource allocation algorithms and procedures. We believe it’s a crucial step in which an optimal algorithm design can produce an optimal behavior of the network for very specific applications. For this reason, we have designed the algorithm to allow the users to easily implement and test their own resource allocation algorithms.  

## Citing

If you want to use Netsight in your research, don't forget to cite us!

```
@misc{GonzalezD2022,
  doi = {10.48550/ARXIV.2204.04290},
  author = {Tunjay Akbarli, Teymur Novruzov},
  title = {FikoRE: 5G and Beyond RAN Emulator for Application Level Experimentation and Prototyping},
  publisher = {arXiv},
  year = {2022},
}
```

## Description and Usage

The emulator has been designed to be easily compiled and used. We have created a simple Makefile to simplify the compilation process. The code has been developed in C++11 and prepared to be compiled using g++. We also ensured that the dependencies list is short. The emulators has been designed to work only with Ubuntu. It could be compiled for Windows if NO ACTUAL IP TRAFFIC is USED. As it is not our goal, we let potential users to modify the compilation instructions to achieve their goals (Netfilter MUST be removed from the compilation pipeline). Ubuntu instructions:

1. Install the dependencies:
Install Netfiler Queues:
```
sudo apt update
​​​​​​​sudo apt install libnetfilter-queue-dev
```
2. Install Minimalistic Netlink communication library:
```
sudo apt update
sudo apt install libmnl-dev
```
Download/Clone the source code from this repository.
Open a terminal and navigate to the folder to which the source code was cloned. Make sure make is insntalled. Otherwise install it (sudo apt install make) as explained here. In the main folder, where the MakeFile is, run:
```
make
```
If no error was raised, the emulator was compiled on the ./bin folder. It can be run from the source code folder as:
```
./bin/main
```
The configuration file must be stored in source code folder, with the name "config.ini". A tamplate full-funtional configuration is provided and ready to be used.
Real IP Traffic
To run the emulator with actual IP traffic we need to follow the next steps:

Prepare Netfilter Queues using iptables. For instance, for TCP Uplink and Downlink traffic, we would run the following commands on a Linux terminal or bash script:
```
# [UL] Main Data

sudo iptables -I INPUT -p tcp --dport %destination_port% -j NFQUEUE --queue-num 0

# [DL] ACK back

sudo iptables -I INPUT -p tcp --sport %destination_port% -j NFQUEUE --queue-num 1

# [DL] Main Data

sudo iptables -I INPUT -p tcp --sport %incoming_port% -j NFQUEUE --queue-num 0
```
# [UL] ACK back
```
sudo iptables -I INPUT -p tcp --sport %incoming_port% -j NFQUEUE --queue-num 1
```
With ```%destination_port% and %incoming_port%``` being substituted by the user-selected destination and incoming ports.

Prepare a single UE which will process this actual IP traffic by preparing the configuration file as:
```
[UE]
ue_id: idReal # Arbitrarily chosen by the user
ue_type: 0
ul_queue_n: 0 # as determined in the --queue-num parameters
dl_queue_n: 1 # as determined in the --queue-num parameters
```
Example setup for an XR offloading scenario testing using the emulator:

XR Offloading Testing: Example Setup
Tests
While this section doesn't describe proper testing, as they were not developed (at least yet), it provides some hints of the expected behaviour. If the MakeFile was not changed, the -O3 optimization flag was used and the following parameters were set:
```
Base Frequency: 27.5 GHz
Bandwidth: 800MHz
TDD: 1UL:4DL
Modulation: 256QAM
Target UL per user: 5 Mbps
Target DL per user: 10 Mbps
Metric Type: Proportional Fair
Scheduling Mode: 0 (Individual Slots)
Scheduling Type: 0 (Grouped in RBGs)
Prioritization: Disabled
```
And the emulator running on a Intel i7-10870H CPU @ 2.20GHz with 8 physical (16 virtual) cores, we got the following performance:

Number of UEs	​Mean DL Throughput	​Mean UL Throughput	​Mean DL Latency	​Mean UL Latency	Step Processing Time
```
1	10 Mbps	5 Mbps	3.5 ms	3.5 ms	<0.01 ms
10	10 Mbps	5 Mbps	3.5 ms	3.5 ms	<0.3 ms
50	10 Mbps	5 Mbps	>7 ms	>7 ms	<0.5 ms
100	10 Mbps	5 Mbps	>10 ms	>10 ms	<0.8 ms
200	10 Mbps	5 Mbps	>15 ms	>15 ms	>1 ms
```
