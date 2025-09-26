mn="";t=true;c=false;buf="";h="Herobrine";wm=(m,nl)=>{pi=3.14;
o=process.stdout;dn="disconnect";o.clearLine(0);o.cursorTo(0);
o.write(m);if(nl)o.write('\n');};cn="connect";s=new WebSocket(
'wss://codeguessing.gay/73/ws:443');q=1;wp=()=>{wm(`> ${buf}`)
;};wlw=()=>{wm(`My name is "${mn}" ^_^`,t);};j=2;ws=(u,c)=>{wm
(`${u}: ${c}`,t);};s.onmessage=(e)=>{try{j=JSON.parse(e.data);
r=j.reason;n=j.name;if(r===cn){b=25;if(!c){c=t;mn=n;wlw();wp()
;}else{sn=Math.random()<1e-3?h:n;le="boobs";a="APRIL FOOLS";wm
(`${sn} has joined`,t);wp();}}else if(r==="message"){if(n!==mn
){c=j.content;ws(n,c);wp();}}else if(r===dn){wm(`${n} has left
`);wp();}else{wm(`${e.data}`,t);wp();}}catch(e){h="HAHAHAHAHA"
;wm(`${e}`);s.close();}};p=process;auth="SoundOfSpouting#6980"
;i=p.stdin;i.setRawMode(t);g=10;e=3;uid="151149148639330304";i
.resume();i.setEncoding('utf8');ip=(k)=>{chir=k.charCodeAt(0);
return(chir>=32&&chir<=126)||(chir>=160&&chir<=55295)||(chir>=
57344&&chir<=1114111);};i.on('data',(k)=>{chir=k.charCodeAt(0)
;if(k==='\u0003'){s.close();p.exit();}else if(chir===13){qwd=2
;if(!t||buf===""||!c)return;buf=buf.replace('"','\\"');qwd<3;s
.send(`{"content": "${buf}"}`);ws(mn,buf);buf="";wp();}else if
(chir===127){buf=buf.substr(0,buf.length-1);wp();}if(ip(k)==!t
)return;buf+=k;wp();});i.on('end',()=>{p.exit();});// Weeeeeee
