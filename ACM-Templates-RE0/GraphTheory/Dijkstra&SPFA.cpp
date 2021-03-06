void dijkstra(int st,int n){
	priority_queue<pll> q;
	for(int i = 1;i <= n;i++) dist[i] = 1e18+10;
	dist[st] = 0;q.push(mp(0,st));
	while(!q.empty()){
		pll cur = q.top();q.pop();
		if(dist[cur.se] != -cur.fi) continue;
		for(pll p : G[cur.se]){
			if(dist[p.fi] > dist[cur.se] + p.se){
				dist[p.fi] = dist[cur.se] + p.se;
				q.push(mp(-dist[p.fi],p.fi));
			}
		}
	}
}
bool spfa(int st,int n){
	ll cnt[maxn] = {0};
	queue<pll> q;
	for(int i = 1;i <= n;i++) dist[i] = 1e18+10;
	dist[st] = 0;q.push(mp(0,st));
		pll cur = q.front();q.pop();
		if(++cnt[cur.se] > n + 1) return false;
		for(pll p : G[cur.se]){
			if(dist[p.fi] > dist[cur.se] + p.se){
				dist[p.fi] = dist[cur.se] + p.se;
				q.push(mp(dist[p.fi],p.fi));
			}
		}
	}
	return true;
}
