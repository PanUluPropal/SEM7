#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>
using namespace std;

namespace Zadacha1 {
    class Trajectory {
    public:
        vector<double> t, x;
        bool loadFromFile(const string& f) {
            ifstream file(f);
            if (!file.is_open()) return false;
            string h; getline(file, h);
            double a, b; char c;
            while (file >> a >> c >> b) {
                t.push_back(a);
                x.push_back(b);
            }
            return true;
        }
        vector<double> velocity() {
            vector<double> v;
            auto f = [](double x1, double x2, double t1, double t2) { return (x2 - x1) / (t2 - t1); };
            for (size_t i = 0; i < t.size() - 1; i++) v.push_back(f(x[i], x[i + 1], t[i], t[i + 1]));
            v.push_back(v.back());
            return v;
        }
        void plot() {
            ofstream o("traj_plot.csv");
            o << "t,x\n";
            for (size_t i = 0; i < t.size(); i++) o << t[i] << "," << x[i] << endl;
            o.close();
            FILE* g = popen("gnuplot -persistent", "w");
            fprintf(g, "plot 'traj_plot.csv' using 1:2 with linespoints\n");
            pclose(g);
        }
    };
    void run() {
        Trajectory a;
        if (a.loadFromFile("traj.csv")) {
            a.velocity();
            a.plot();
        }
    }
}

namespace Zadacha2 {
    class SensorData {
    public:
        vector<double> t, h1, h2;
        bool load(const string& f) {
            ifstream file(f);
            if (!file.is_open()) return false;
            string h; getline(file, h);
            double a, b, c; char d;
            while (file >> a >> d >> b >> d >> c) {
                t.push_back(a);
                h1.push_back(b);
                h2.push_back(c);
            }
            return true;
        }
        void diff() {
            ofstream o("diff.csv");
            o << "t,dh\n";
            auto f = [](double a, double b) { return fabs(a - b); };
            for (size_t i = 0; i < t.size(); i++)
                o << t[i] << "," << f(h1[i], h2[i]) << endl;
            o.close();
        }
        void plot() {
            FILE* g = popen("gnuplot -persistent", "w");
            fprintf(g, "plot 'sensors.csv' using 1:2 with lines,'sensors.csv' using 1:3 with lines,'diff.csv' using 1:2 with linespoints\n");
            pclose(g);
        }
    };
    void run() {
        SensorData s;
        if (s.load("sensors.csv")) {
            s.diff();
            s.plot();
        }
    }
}

namespace Zadacha3 {
    void run() {
        ifstream file("altitude.csv");
        if (!file.is_open()) return;
        string h; getline(file, h);
        vector<pair<double, double>> d, f;
        double a, b; char c;
        while (file >> a >> c >> b) d.emplace_back(a, b);
        auto cond = [](auto& p) { return p.second > 900 && p.second < 1100; };
        copy_if(d.begin(), d.end(), back_inserter(f), cond);
        ofstream o1("original.csv"), o2("filtered.csv");
        o1 << "t,H\n"; o2 << "t,H\n";
        for (auto& p : d) o1 << p.first << "," << p.second << endl;
        for (auto& p : f) o2 << p.first << "," << p.second << endl;
        o1.close(); o2.close();
        FILE* g = popen("gnuplot -persistent", "w");
        fprintf(g, "plot 'original.csv' using 1:2 with points,'filtered.csv' using 1:2 with linespoints\n");
        pclose(g);
    }
}

namespace Zadacha4 {
    class Navigator {
    public:
        vector<double> t, x, y;
        bool load(const string& f) {
            ifstream file(f);
            if (!file.is_open()) return false;
            string h; getline(file, h);
            double a, b, c; char d;
            while (file >> a >> d >> b >> d >> c) {
                t.push_back(a); x.push_back(b); y.push_back(c);
            }
            return true;
        }
        vector<double> speed() {
            vector<double> v;
            auto f = [](double a, double b, double t1, double t2) { return (b - a) / (t2 - t1); };
            for (size_t i = 0; i < t.size() - 1; i++) {
                double vx = f(x[i], x[i + 1], t[i], t[i + 1]);
                double vy = f(y[i], y[i + 1], t[i], t[i + 1]);
                v.push_back(sqrt(vx * vx + vy * vy));
            }
            v.push_back(v.back());
            return v;
        }
        void save() {
            auto v = speed();
            ofstream o("speed.csv");
            o << "t,v\n";
            for (size_t i = 0; i < t.size(); i++)
                o << t[i] << "," << v[i] << endl;
            o.close();
            FILE* g = popen("gnuplot -persistent", "w");
            fprintf(g, "plot 'speed.csv' using 1:2 with linespoints\n");
            pclose(g);
        }
    };
    void run() {
        Navigator n;
        if (n.load("navigation.csv")) n.save();
    }
}

namespace Zadacha5 {
    class MotionAnalyzer {
    public:
        vector<double> t, x, v, a;
        bool load(const string& f) {
            ifstream file(f);
            if (!file.is_open()) return false;
            string h; getline(file, h);
            double ti, xi; char c;
            while (file >> ti >> c >> xi) {
                t.push_back(ti);
                x.push_back(xi);
            }
            return true;
        }
        void vel() {
            auto f = [](double x1, double x2, double t1, double t2) { return (x2 - x1) / (t2 - t1); };
            for (size_t i = 0; i < t.size() - 1; i++)
                v.push_back(f(x[i], x[i + 1], t[i], t[i + 1]));
            v.push_back(v.back());
        }
        void acc() {
            auto f = [](double v1, double v2, double t1, double t2) { return (v2 - v1) / (t2 - t1); };
            for (size_t i = 0; i < t.size() - 1; i++)
                a.push_back(f(v[i], v[i + 1], t[i], t[i + 1]));
            a.push_back(a.back());
        }
        void save() {
            ofstream o("motion_processed.csv");
            o << "t,x,v,a\n";
            for (size_t i = 0; i < t.size(); i++)
                o << t[i] << "," << x[i] << "," << v[i] << "," << a[i] << endl;
            o.close();
            FILE* g = popen("gnuplot -persistent", "w");
            fprintf(g, "plot 'motion_processed.csv' using 1:3 with linespoints,'motion_processed.csv' using 1:4 with linespoints\n");
            pclose(g);
        }
    };
    void run() {
        MotionAnalyzer m;
        if (m.load("motion.csv")) {
            m.vel();
            m.acc();
            m.save();
        }
    }
}

int main() {
    Zadacha1::run();
    Zadacha2::run();
    Zadacha3::run();
    Zadacha4::run();
    Zadacha5::run();
    return 0;
}
