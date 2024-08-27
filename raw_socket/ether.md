
ether
destination MAC (6 bytes)
- unicast : the lowest bit of the first byte is 0 
-multicast : the lowest bit of the first byte is 1 
-broadcast : FFFFFF
source MAC (6 bytes)
length/type (2 bytes) : when the value less the 0x0600 , it represent length , else it represent type
- ip : 0x0800
- arp : 0x0806
- ipv6 : 0x86DD
- PPP : 0x880B
