import{useState}from"react";
import{useSimulator}from"./hooks/useSimulator";
import SimulationControls from"./components/SimulationControls";
import SimulationLab from"./components/SimulationLab";
import Overview from"./components/Overview";
import Topology from"./components/Topology";
import Routing from"./components/Routing";
import Traffic from"./components/Traffic";
import Security from"./components/Security";
import Metrics from"./components/Metrics";
const tabs=[
["overview","Overview"],
["topology","Topology"],
["routing","Routing"],
["traffic","Traffic"],
["security","Security"],
["metrics","Metrics"],
["lab","Simulation Lab"]
];

export default function App(){
const{data,loading,error,refresh}=useSimulator();
const[tab,setTab]=useState("overview");

if(loading){
return <div className="loading">Connecting to NetSimX C++ Engine...</div>;
}

if(error||!data){
return <div className="loading"><h2>NetSimX API Connection Error</h2><p>{error||"No simulation data received."}</p><button onClick={refresh}>Retry</button></div>;
}

const pages={
overview:<><SimulationControls onComplete={refresh}/><Overview data={data}/></>,
topology:<Topology data={data}/>,
routing:<Routing data={data}/>,
traffic:<Traffic data={data}/>,
security:<Security data={data}/>,
metrics:<Metrics data={data}/>,
lab:<SimulationLab onComplete={refresh}/>
};

return <div className="app">
<aside>
<div className="brand">
<div className="brand-mark">N</div>
<div><strong>NetSimX</strong><small>Network Simulator</small></div>
</div>
<nav>
{tabs.map(([id,label])=><button key={id} className={tab===id?"active":""} onClick={()=>setTab(id)}>{label}</button>)}
</nav>
<div className="side-status"><span></span>C++ Engine Connected</div>
</aside>
<main>
<header>
<div><span className="status-dot"></span>Live Simulation Environment</div>
<div className="header-right"><button className="refresh-btn" onClick={refresh}>Refresh</button><span>C++ Engine · Phase 21</span></div>
</header>
{pages[tab]}
</main>
</div>;
}