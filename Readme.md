Omega challenge: Switch kill switch
--
Ousema BOUANENI

Done as part of a cybersecurity course at École polytechnique (CSC_43M05_EP)

# Program description
`l2flood` is a command line utility that launches a [MAC flooding attack](https://en.wikipedia.org/wiki/MAC_flooding) on a network interface of the user's choice. 

Once you're in the root directory of the project, it can be compiled with a simple:
```console
$make
```

The usage syntax is the following:
```console
#./l2flood <interface>
```

# Setup description
## The network
In order to demonstrate the attack, I have set up 4 different VMs: a switch, an attacker and two victims. Each of the three machines shares a network (using the virtual box internal network adapter) with the switch, as is illustrated by the following diagram:
```
               IP: 192.168.1.1                 
            ┌────────────────────┐             
            │ machine1 (attacker)│             
            └─────────┬──────────┘             
                      │intnet1                 
                      │                        
                  ┌───┴────┐                   
        ┌─────────┤ switch ├─────────┐         
        │intnet2  └────────┘ intnet3 │         
        │                            │         
┌───────┴──────────┐        ┌────────┴────────┐
│ machine2 (victim)│        │machine3 (victim)│
└──────────────────┘        └─────────────────┘
   IP: 192.168.1.2            IP: 192.168.1.3  
```

On the switch, an open vswitch bridge has been set up to link the three network interfaces:
```console
#ovs-vsctl add-br br0
#ovs-vsctl add-port br0 <interface1>
#ovs-vsctl add-port br0 <interface2>
#ovs-vsctl add-port br0 <interface3>
#ip link set br0 up
```
## The attack
After setting up the machines with the appropriate ip addresses, and getting the l2flood executable program into machine1 (for instance using `scp`), the effects of the attack can be illustrated by making one of the two victims ping each other, and launching `l2flood` on the attacker, which interrupts the ping command, as can be seen in `demonstration.mp4`.