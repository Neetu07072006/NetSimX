import{LineChart,Line,BarChart,Bar,XAxis,YAxis,CartesianGrid,Tooltip,ResponsiveContainer}from"recharts";

export default function Metrics({data}){
const packets=data?.packets||{};
const latency=data?.latency||{};
const queue=data?.queue||{};
const failures=data?.failures||{};
const total=Number(packets.total||0);
const delivered=Number(packets.delivered||0);
const dropped=Number(packets.dropped||0);
const loss=total?((dropped/total)*100).toFixed(1):"0.0";
const delivery=total?((delivered/total)*100).toFixed(1):"0.0";
const performanceData=[
{name:"Packets",value:total},
{name:"Delivered",value:delivered},
{name:"Dropped",value:dropped}
];
const failureData=[
{name:"Link Failures",value:Number(failures.linkFailures||0)},
{name:"Recoveries",value:Number(failures.recoveries||0)}
];
const latencyData=[
{name:"Total",value:Number(latency.total||0)},
{name:"Average",value:Number(latency.average||0)}
];

return <section className="metrics-page">
<div className="page-heading">
<div>
<span className="section-label">NETWORK PERFORMANCE</span>
<h2>Performance Monitor</h2>
<p>Real-time performance, queue and reliability metrics from NetSimX.</p>
</div>
<div className={`performance-status ${Number(loss)>20?"performance-danger":Number(loss)>5?"performance-warning":"performance-good"}`}>
<span></span>
{Number(loss)>20?"Performance Critical":Number(loss)>5?"Performance Warning":"Performance Stable"}
</div>
</div>

<div className="metrics-summary">
<div className="metric-panel">
<span>Throughput</span>
<strong>{Number(latency.throughput||0).toFixed(2)}</strong>
<small>Mbps</small>
</div>
<div className="metric-panel">
<span>Average Latency</span>
<strong>{Number(latency.average||0).toFixed(2)}</strong>
<small>ms</small>
</div>
<div className="metric-panel">
<span>Packet Loss</span>
<strong>{loss}%</strong>
<small>{dropped} dropped</small>
</div>
<div className="metric-panel">
<span>Delivery Rate</span>
<strong>{delivery}%</strong>
<small>{delivered} delivered</small>
</div>
</div>

<div className="metrics-grid">
<div className="metrics-card">
<div className="card-heading">
<div>
<h3>Packet Performance</h3>
<p>Overall packet processing results.</p>
</div>
</div>
<div className="metrics-chart">
<ResponsiveContainer width="100%" height={280}>
<BarChart data={performanceData}>
<CartesianGrid strokeDasharray="3 3"/>
<XAxis dataKey="name"/>
<YAxis allowDecimals={false}/>
<Tooltip/>
<Bar dataKey="value" name="Packets"/>
</BarChart>
</ResponsiveContainer>
</div>
</div>

<div className="metrics-card">
<div className="card-heading">
<div>
<h3>Latency Profile</h3>
<p>Total and average network latency.</p>
</div>
</div>
<div className="metrics-chart">
<ResponsiveContainer width="100%" height={280}>
<LineChart data={latencyData}>
<CartesianGrid strokeDasharray="3 3"/>
<XAxis dataKey="name"/>
<YAxis/>
<Tooltip/>
<Line type="monotone" dataKey="value" name="Latency" strokeWidth={3}/>
</LineChart>
</ResponsiveContainer>
</div>
</div>
</div>

<div className="queue-section">
<div className="metrics-card queue-card">
<div className="card-heading">
<div>
<h3>Queue Performance</h3>
<p>Router queue behavior during simulation.</p>
</div>
</div>
<div className="queue-metrics">
<div><span>Utilization</span><strong>{Number(queue.utilization||0).toFixed(1)}%</strong></div>
<div><span>Maximum Size</span><strong>{Number(queue.maximum||0).toFixed(0)}</strong></div>
<div><span>Queue Drops</span><strong>{Number(queue.drops||0)}</strong></div>
<div><span>Average Delay</span><strong>{Number(queue.averageDelay||0).toFixed(2)} ms</strong></div>
</div>
<div className="queue-bar">
<div style={{width:`${Math.min(Number(queue.utilization||0),100)}%`}}></div>
</div>
</div>

<div className="metrics-card">
<div className="card-heading">
<div>
<h3>Network Reliability</h3>
<p>Failure and recovery events.</p>
</div>
</div>
<div className="metrics-chart">
<ResponsiveContainer width="100%" height={250}>
<BarChart data={failureData}>
<CartesianGrid strokeDasharray="3 3"/>
<XAxis dataKey="name"/>
<YAxis allowDecimals={false}/>
<Tooltip/>
<Bar dataKey="value" name="Events"/>
</BarChart>
</ResponsiveContainer>
</div>
</div>
</div>

<div className="metrics-card detailed-metrics">
<div className="card-heading">
<div>
<h3>Detailed Metrics</h3>
<p>Raw measurements reported by the simulation engine.</p>
</div>
</div>
<div className="metrics-detail-grid">
<div><span>Total Packets</span><strong>{total}</strong></div>
<div><span>Delivered Packets</span><strong>{delivered}</strong></div>
<div><span>Dropped Packets</span><strong>{dropped}</strong></div>
<div><span>Expired Packets</span><strong>{packets.expired||0}</strong></div>
<div><span>Total Bytes</span><strong>{packets.bytes||0}</strong></div>
<div><span>Total Latency</span><strong>{Number(latency.total||0).toFixed(2)} ms</strong></div>
<div><span>Queue Drops</span><strong>{queue.drops||0}</strong></div>
<div><span>Link Failures</span><strong>{failures.linkFailures||0}</strong></div>
<div><span>Recovery Events</span><strong>{failures.recoveries||0}</strong></div>
</div>
</div>
</section>;
}