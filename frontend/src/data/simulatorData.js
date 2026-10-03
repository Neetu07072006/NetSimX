export const simulatorData={
health:"HEALTHY",
packets:{total:36,delivered:33,dropped:3,expired:0,bytes:18432},
latency:{total:172,average:5.21,throughput:0.86},
security:{firewallAllowed:31,firewallDenied:5,idsAlerts:4,critical:1,high:2,medium:1,low:0},
queue:{drops:2,utilization:64.5,maximum:8,averageDelay:1.8},
failures:{linkFailures:1,recoveries:1},
nodes:[
{id:"pc1",name:"PC1",type:"HOST",ip:"192.168.1.10",x:90,y:210},
{id:"r1",name:"R1",type:"ROUTER",ip:"10.0.1.1",x:270,y:110},
{id:"r2",name:"R2",type:"ROUTER",ip:"10.0.2.1",x:500,y:80},
{id:"r3",name:"R3",type:"ROUTER",ip:"10.0.3.1",x:500,y:240},
{id:"r4",name:"R4",type:"ROUTER",ip:"10.0.4.1",x:730,y:160},
{id:"pc4",name:"PC4",type:"HOST",ip:"192.168.4.10",x:910,y:160}
],
links:[
{a:"pc1",b:"r1",latency:1,status:"ACTIVE"},
{a:"r1",b:"r2",latency:5,status:"ACTIVE"},
{a:"r1",b:"r3",latency:2,status:"ACTIVE"},
{a:"r2",b:"r4",latency:2,status:"ACTIVE"},
{a:"r3",b:"r4",latency:1,status:"ACTIVE"},
{a:"r4",b:"pc4",latency:1,status:"ACTIVE"}
],
routes:[
{source:"PC1",destination:"PC4",path:"PC1 → R1 → R3 → R4 → PC4",cost:"5 ms"},
{source:"PC1",destination:"R4",path:"PC1 → R1 → R3 → R4",cost:"4 ms"},
{source:"R1",destination:"PC4",path:"R1 → R3 → R4 → PC4",cost:"4 ms"}
],
traffic:[
{protocol:"TCP",packets:14,delivered:13,dropped:1},
{protocol:"UDP",packets:12,delivered:11,dropped:1},
{protocol:"ICMP",packets:8,delivered:8,dropped:0},
{protocol:"TELNET",packets:2,delivered:1,dropped:1}
],
alerts:[
{id:1,type:"MULTIPLE ATTACKS",severity:"CRITICAL",source:"192.168.1.10",protocol:"TCP",port:8080},
{id:2,type:"PORT SCAN",severity:"HIGH",source:"192.168.1.10",protocol:"TCP",port:80},
{id:3,type:"REPEATED DENIAL",severity:"HIGH",source:"192.168.1.10",protocol:"TCP",port:8080},
{id:4,type:"SUSPICIOUS PROTOCOL",severity:"MEDIUM",source:"192.168.1.10",protocol:"TELNET",port:23}
]
};