#include <iostream>
#include <vector>

using namespace std;

struct vec2 {
	double x,y;
	double distance_eu(vec2 p) {
		return sqrt(pow(x - p.x,2) + pow(y - p.y,2));
	}
	double distance_manh(vec2 p) {
		return abs(p.x - x)+abs(p.y - y);
	}
	double distance(vec2 p,bool e=true) {
		return e ? distance_eu(p) : distance_manh(p);
	}
	bool operator==(vec2& obj) {
		return ((x == obj.x) && (y == obj.y));
	}
};

struct set {
	vec2 p;
	double v; // value
	bool operator==(set& obj) {
		return (obj.p == p);
	}
};

void push_repeated(vector<set*>* dataset,set* t,int f) {
	vector<set*> r;
	for(int i = 0; i < f; i++) {
		r.push_back(t);
	}
	dataset->insert(dataset->begin(),r.begin(),r.end());
}

double average_set(vector<set*> d) {
	long double sum = 0;
	for(int i = 0; i < d.size(); i++) {
		if(d[i] != nullptr) {
			sum += d[i]->v;
		}
	}
	return sum / d.size();
}

double average(vector<double> d) {
	long double sum = 0;
	for(int i = 0; i < d.size(); i++) {
		sum += d[i];
	}
	return sum / d.size();
}

class knn {
public:
	vector<set> dataset;
	void add(vec2 p,double v) {
		dataset.push_back({p,v});
	}
private:
	set* closer_set(vec2 p,vector<set*> ignore = {},bool e=true) {
		set* c = nullptr;
		double min_dist = (+INFINITY);
		for(int i = 0; i < dataset.size(); i++) {
			double c_dist = 0;
			for(int j = 0; j < ignore.size(); j++) {
				if(dataset[i] == (*ignore[j])) {
					goto skip_element;
				}
			}
			c_dist = dataset[i].p.distance(p,e);
			if(c_dist < min_dist) {
				c = &dataset[i];
				min_dist = c_dist;
			}
		skip_element:continue;
		}
		return c;
	}
public:
	double predict(vec2 p,int k=3,bool euclidian = true) {
		vector<set*> near_v;
		for(int i = 0; i < k; i++) {
			push_repeated(&near_v,closer_set(p,near_v,euclidian),k - i);
		}
		return average_set(near_v);
	}
};

double f(double X,double Y) {
	return pow(X,1/Y);
}

int main() {
	knn clf;

	for(double x = 1; x <= 10; x+=0.5) {
		for(double y = 1; y <= 10; y+=0.5) {
			clf.add({x,y}, f(x,y) );
		}
	}

	vector<double> accuracy = {};

	for(double x = 1; x <= 10; x += 0.05) {
		for(double y = 1; y <= 10; y += 0.05) {
			double ob = f(x,y);
			double pr = clf.predict({x,y},3,false);
			accuracy.push_back( min(ob,pr)/max(ob,pr) );
		}
	}

	cout.precision(3);
	cout << average(accuracy)*100 << "%";
}