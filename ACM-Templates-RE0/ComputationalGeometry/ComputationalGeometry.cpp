struct Point{
	double x,y;ll id; 
	Point(double _x = 0,double _y = 0):x(_x),y(_y){}
};
bool operator<(const Point& a,const Point& b){
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}
int dcmp(double x){if (fabs(x)<eps)return 0;else return x<0?-1:1;}
Point operator + (Point a,Point b){return Point(a.x+b.x,a.y+b.y);}
Point operator - (Point a,Point b){return Point(a.x-b.x,a.y-b.y);}
Point operator * (Point a,double b){return Point(a.x*b,a.y*b);}
Point operator / (Point a,double b){return Point(a.x/b,a.y/b);}
double operator * (Point a,Point b){return a.x*b.x+a.y*b.y;}
bool operator == (Point a,Point b){return dcmp(a.x-b.x) == 0 && dcmp(a.y-b.y) == 0;}
double Dot(Point a,Point b){return a.x*b.x+a.y*b.y;}  //点积
double Length(Point a){return sqrt(Dot(a,a));}
double Cross(Point a,Point b){return a.x*b.y-a.y*b.x;} //叉积

inline double Length(Point a){
	return sqrt(a.x*a.x + a.y*a.y);
}
inline double Length2(Point a){
	return a.x*a.x + a.y*a.y;
}
//单位化向量 ，若是零向量直接返回 
Point unit(Point a){
	double l = Length(a); 
	if(l < eps) return a;
	return Point(a.x/l,a.y/l);
}
//求向量A的左转法向量 
Point normal(Point a){
	return Point(-a.y,a.x);
}
//求单位左转法向量，调用前请保证A不是零向量 
Point unitNormal(Point a){
	double l = Length(a);
	return Point(-a.y/l,a.x/l);
}
inline double Angle(Point a,Point b){
	return acos(a * b / Length(a) / Length(b));
}
//有向面积 
double Area2(Point a,Point b,Point c){
	return Cross(b-a,c-a);
}
Point rotate(Point a,double rad){
	return Point(a.x*cos(rad)-a.y*sin(rad), a.x*sin(rad)+a.y*cos(rad));
}
//不损失精度判断线段规范相交(不含端点)
//若要判断线段是否有点在多边形内部，最好缩多边形，判任一公共点，
//或者把线段端点往里缩一下，同时取中点，check一下这三个点是不是在多边形内部 
bool isSegmentsIntersection(Point A,Point B,Point C,Point D){
	//跨立试验 
	if(dcmp(Cross(C-A,D-A) * Cross(C-B,D-B)) >= 0) return false;
	if(dcmp(Cross(A-C,B-C) * Cross(A-D,B-D)) >= 0) return false;
	//快速排斥试验 
	if(dcmp(min(max(A.x,B.x),max(C.x,D.x)) - max(min(A.x,B.x),min(C.x,D.x))) < 0) return false;
	if(dcmp(min(max(A.y,B.y),max(C.y,D.y)) - max(min(A.y,B.y),min(C.y,D.y))) < 0) return false;
	return true;
}
//点在线段上(//不含端点)
bool isPointOnSegment(Point P,Point a,Point b){
	if(P == a || P == b) return true;
	//if(p == a || p == b) return false;
	return dcmp(Cross(a-P,b-P)) == 0 && dcmp((a-P)*(b-P)) < 0;
} 
//判断两条线段是否有公共点 
bool isSegmentsCrash(Point A,Point B,Point C,Point D){
	if( isPointOnSegment(A,C,D) || isPointOnSegment(B,C,D) ||
		isPointOnSegment(C,A,B) || isPointOnSegment(D,A,B)) return true;
	if(dcmp(Cross(B-A,D-C)) == 0) return false;//共线 
	return isSegmentsIntersection(A,B,C,D);//判断线段规范相交 
}
Point midPoint(Point a,Point b){
	return Point((a.x+b.x)*0.5,(a.y+b.y)*0.5);
} 
//------------------线段相关内容-------------------- 
//有向直线 
struct Line{
	Point p1,p2;//直线上两点，从p1到p2，左边是半平面 
	double ang;//极角，从x正半轴转到v所需的角(弧度) 
	Line(){}
	Line(Point p1, Point p2):p1(p1),p2(p2){
		ang = atan2(p2.y-p1.y, p2.x-p1.x);
	}
	bool operator < (const Line& L) const{ //半平面交需要的排序函数 
		return ang < L.ang;
	}
};
//直线相交，使用前保证有唯一交点，cross(v,w)非0
Point getLineIntersection(Point A, Point B, Point C, Point D){
	Point u = A - C, v = B - A, w = D - C;
	double t = Cross(w, u) / Cross(v, w);
	return A + v * t;
}
Point getLineIntersection(Line L1, Line L2){
	Point u = L1.p1 - L2.p1, v = L1.p2 - L1.p1, w = L2.p2 - L2.p1;
	double t = Cross(w, u) / Cross(v, w);
	return L1.p1 + v * t;
}
//点到直线距离
double distanceToLine(Point P,Line L){
	Point v1 = L.p2 - L.p1, v2 = P - L.p1;
	return fabs(Cross(v1,v2)) / Length(v1);//不取绝对值就是有向距离 
}
//----------------多边形相关内容----------------------
typedef vector<Point> polygon;
//调用时最好zoom(poly, eps*1000) 或者 zoom(poly, sqrt(eps))
void zoom(polygon& poly, double rate){
	int n = poly.size();
	vector<Point> tmp;
	for(int i = 0;i < n;i++) tmp.push_back(unitNormal(poly[i] - poly[(i-1+n)%n]));
	for(int i = 0;i < n;i++){
		poly[i] = poly[i] + ((tmp[i] + tmp[(i+1)%n]) * rate);
	}
}
//-----------------圆相关内容-----------------------
struct Circle{
	Point o;
	double r;
	Circle(Point o,double r):o(o),r(r){}
	Point point(double rad){
		return Point(o.x+cos(rad)*r,o.y+sin(rad)*r);
	}
};
//给定两点作为直径获取圆
Circle getCircle(Point a,Point b){
	return Circle((a+b)*0.5,Length(a-b)*0.5);
} 
//给予三个点，求外接圆 
Circle Getcir(Point A,Point B,Point C){
    double a = 2*(B.x - A.x);
    double b = 2*(B.y - A.y);
    double c = (B.x*B.x+B.y*B.y) - (A.x*A.x+A.y*A.y);
    double d = 2*(C.x-B.x);
    double e = 2*(C.y-B.y);
    double f = (C.x*C.x + C.y*C.y) - (B.x*B.x + B.y*B.y);
    double x = (b*f-e*c)/(b*d-e*a);
    double y = (d*c-a*f)/(b*d-e*a);
    double r = sqrt((x-A.x)*(x-A.x) + (y-A.y)*(y-A.y));
    Point ans(x,y);
    return Circle(ans,r);
}
//包含三个点的面积最小的圆(注意，不是外接圆)
Circle getMinCircle(Point a,Point b,Point c){
	if(dcmp(Cross(b-a,c-a)) == 0){
		//三点共线
		if (dcmp(Length(a-b)+Length(b-c)-Length(a-c))==0) return getCircle(a,c);
        if (dcmp(Length(b-a)+Length(a-c)-Length(b-c))==0) return getCircle(b,c);
        if (dcmp(Length(a-c)+Length(c-b)-Length(a-b))==0) return getCircle(a,b);
	} else{
		if((b-a)*(c-a) <= 0) return getCircle(b,c);
		if((a-b)*(c-b) <= 0) return getCircle(a,c);
		if((a-c)*(b-c) <= 0) return getCircle(a,b);
		Point m1 = midPoint(a,b), m2 = midPoint(a,c);
		Line L1 = Line(m1,m1 + normal(b-a));
		Line L2 = Line(m2,m2 + normal(c-a));
		Point o = getLineIntersection(L1,L2);
		return Circle(o,Length(a-o));
	}
}
//点在圆内(不含边界是<)
bool pointInCircle(Point a,Circle c){
	return dcmp(Length2(a-c.o)-c.r*c.r) <= 0;
}
//两圆的面积交
double Ac(Point c1, double r1, Point c2, double r2)
{
    double d = dist(c1,c2);
    if(r1 + r2 < d + eps) return 0;
    if(d < fabs(r1 - r2) + eps)
    {
        double r = min(r1,r2);
        return PI*r*r;
    }
    double x = (d*d + r1*r1 - r2*r2)/(2*d);
    double t1 = acos(x / r1);
    double t2 = acos((d - x)/r2);
    return r1*r1*t1 + r2*r2*t2 - d*r1*sin(t1);
}
