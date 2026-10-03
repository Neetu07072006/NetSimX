export default function MetricCard({label,value,unit,accent}){
return <div className="metric-card"><span>{label}</span><strong className={accent||""}>{value}</strong>{unit&&<small>{unit}</small>}</div>
}