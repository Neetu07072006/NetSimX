import{BarChart,Bar,XAxis,YAxis,CartesianGrid,Tooltip,ResponsiveContainer,Cell}from"recharts";

export default function Security({data}){
const security=data?.security||{};
const alerts=data?.alerts||[];
const chartData=[
{name:"Critical",value:Number(security.critical||0)},
{name:"High",value:Number(security.high||0)},
{name:"Medium",value:Number(security.medium||0)},
{name:"Low",value:Number(security.low||0)}
];
const totalFirewall=Number(security.firewallAllowed||0)+Number(security.firewallDenied||0);
const allowRate=totalFirewall?((Number(security.firewallAllowed||0)/totalFirewall)*100).toFixed(1):"0.0";

return <section className="security-page">
<div className="page-heading">
<div>
<span className="section-label">NETWORK SECURITY</span>
<h2>Security Center</h2>
<p>Firewall activity and intrusion detection events from NetSimX.</p>
</div>
<div className={`security-status ${Number(security.critical||0)>0?"security-critical":Number(security.high||0)>0?"security-warning":"security-safe"}`}>
<span></span>
{Number(security.critical||0)>0?"Critical Threats":Number(security.high||0)>0?"Threats Detected":"Network Secure"}
</div>
</div>

<div className="security-summary">
<div className="security-card">
<span>Firewall Allowed</span>
<strong>{security.firewallAllowed||0}</strong>
</div>
<div className="security-card">
<span>Firewall Denied</span>
<strong>{security.firewallDenied||0}</strong>
</div>
<div className="security-card">
<span>IDS Alerts</span>
<strong>{security.idsAlerts||0}</strong>
</div>
<div className="security-card">
<span>Allow Rate</span>
<strong>{allowRate}%</strong>
</div>
</div>

<div className="security-grid">
<div className="security-panel">
<div className="card-heading">
<div>
<h3>IDS Alert Severity</h3>
<p>Distribution of detected security events.</p>
</div>
</div>
<div className="security-chart">
<ResponsiveContainer width="100%" height={320}>
<BarChart data={chartData}>
<CartesianGrid strokeDasharray="3 3"/>
<XAxis dataKey="name"/>
<YAxis allowDecimals={false}/>
<Tooltip/>
<Bar dataKey="value" name="Alerts">
{chartData.map((item,index)=><Cell key={item.name} className={`severity-${index}`}/>)}
</Bar>
</BarChart>
</ResponsiveContainer>
</div>
</div>

<div className="security-panel">
<div className="card-heading">
<div>
<h3>Security Breakdown</h3>
<p>Current IDS severity counters.</p>
</div>
</div>
<div className="severity-list">
<div className="severity-row"><span className="severity-dot critical"></span><span>Critical</span><strong>{security.critical||0}</strong></div>
<div className="severity-row"><span className="severity-dot high"></span><span>High</span><strong>{security.high||0}</strong></div>
<div className="severity-row"><span className="severity-dot medium"></span><span>Medium</span><strong>{security.medium||0}</strong></div>
<div className="severity-row"><span className="severity-dot low"></span><span>Low</span><strong>{security.low||0}</strong></div>
</div>
</div>
</div>

<div className="security-panel alert-panel">
<div className="card-heading">
<div>
<h3>IDS Alert Log</h3>
<p>Security events detected during the latest simulation.</p>
</div>
<span className="alert-count">{alerts.length} Events</span>
</div>
<div className="table-wrapper">
<table>
<thead>
<tr>
<th>ID</th>
<th>Type</th>
<th>Severity</th>
<th>Source</th>
<th>Protocol</th>
<th>Port</th>
</tr>
</thead>
<tbody>
{alerts.length===0?<tr><td colSpan="6" className="empty-security">No security alerts detected.</td></tr>:alerts.map(alert=><tr key={alert.id}>
<td>#{alert.id}</td>
<td><strong>{alert.type}</strong></td>
<td><span className={`alert-badge ${String(alert.severity||"").toLowerCase()}`}>{alert.severity}</span></td>
<td>{alert.source}</td>
<td>{alert.protocol}</td>
<td>{alert.port<0?"-":alert.port}</td>
</tr>)}
</tbody>
</table>
</div>
</div>
</section>;
}