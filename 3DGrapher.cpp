#include<simplecpp>

struct Vector {
    double x, y, z;
    Vector() {}
    Vector(double X, double Y, double Z) {
        x=X;
        y=Y;
        z=Z;
    }
    Vector(double r, double theta) {
        x=r*cosine(theta);
        y=r*sine(theta);
        z=0;
    }
    Vector(double r, double theta, double phi, bool spPolar) {
        x=r*sine(phi)*cosine(theta);
        y=r*sine(phi)*sine(theta);
        z=r*cosine(phi);
    }
    Vector operator+(Vector b) {
        Vector c(x+b.x, y+b.y, z+b.z);
        return c;
    }
    Vector operator-(Vector b) {
        Vector c(x-b.x, y-b.y, z-b.z);
        return c;
    }
    double operator*(Vector b) {
        return (x*b.x+y*b.y+z*b.z);
    }
    Vector cross(Vector b) {
        return Vector(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
    }
    Vector operator*(double scale) {
        Vector c(x*scale, y*scale, z*scale);
        return c;
    }
    double mag() {
        return sqrt(x*x+y*y+z*z);
    }
    double angle(Vector b) {
        Vector a=*this;
        return arccosine((a*b)/a.mag()/b.mag());
    }
    double angle(Vector b, Vector v) {  //signed angle of a wrt to b, rotation vector v(right hand thumb rule)
        Vector a=*this;
        Vector c=v.cross(b);
        return atan2((a*c)*b.mag(), (a*b)*c.mag())*180/PI;
    }
    void print() {
        cout<<"("<<x<<", "<<y<<", "<<z<<")"<<endl;
    }
    void draw(Vector o, Color c) {
        imprintLine(o.x, o.y, o.x+x, o.y+y, c, 5);
    }
};

void draw(Vector a, Vector b, Color c) {
    imprintLine(a.x, a.y, b.x, b.y, c);
}

Vector mobius(double a, double r, double t) {
    return Vector((a+r*cosine(t/2))*cosine(t), (a+r*cosine(t/2))*sine(t), r*sine(t/2));
}

Vector shell(double a, double r, double t) {
    return Vector(a, t, r, true);
}

Vector paraboloid(double a, double r, double t) {
    return Vector(r*cosine(t), r*sine(t), a*r*exp(-0.0001*r*r));
}

Vector porbital(double a, double r, double t) {
    return Vector(a*(cosine(r))*cosine(r), r>90?t+180:t, r, true);
}

Vector torus(double a, double r, double t) {
    return Vector((a+20*cosine(r))*cosine(t), (a+20*cosine(r))*sine(t), 20*sine(r));
}

main_program{
    initCanvas("3D-Grapher", 1500, 800);
    double scale=3;
    Vector o(750, 400, 0);

    Vector i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);
    double theta=120, phi=70;
    Vector v(1, theta, phi, true);
    Vector u=v;

    XEvent e;
    int lastx=0, lasty=0;
    bool wash=false;

while (true) {
    nextEvent(e);
    if (mouseButtonPressEvent(e)){
        lastx=e.xbutton.x;
        lasty=e.xbutton.y;
    }
    else if (mouseDragEvent(e)){
        wash=true;
        if (lastx==e.xmotion.x && lasty==e.xmotion.y) continue;
        double dtheta=-0.2*(e.xmotion.x-lastx);
        double dphi=-0.2*(e.xmotion.y-lasty);
        theta=fmod(theta+dtheta+360, 360);

        if (phi+dphi>0 && phi+dphi<180)
            phi+=dphi;
        v=Vector(1, theta, phi, true);
        lastx=e.xmotion.x;
        lasty=e.xmotion.y;
    }
    else
        wash=false;

    Vector X=i-v*(v*i), Y=j-v*(v*j), Z=k-v*(v*k);
    double alpha=X.angle(Z, v), beta=Y.angle(Z, v);
    Vector x(X.mag()*scale, 270-alpha), y(Y.mag()*scale, 270-beta), z(Z.mag()*scale, 270);

    if (wash)
        Circle c(0, 0, 1);

    (x*100).draw(o, COLOR("green"));        drawText(int((o+(x*120)).x), int((o+(x*120)).y), "X", COLOR("black"));
    (y*100).draw(o, COLOR(255, 170, 0));    drawText(int((o+(y*120)).x), int((o+(y*120)).y), "Y", COLOR("black"));
    (z*100).draw(o, COLOR("black"));        drawText(int((o+(z*120)).x), int((o+(z*120)).y), "Z", COLOR("black"));

    // Mobius starts
    if (true){
    int a=100, b=20;
    int R=128, G=0, B=128;
    int darkLim=sqrt(2*(a+b)*(a+b)+b*b)/2;
    int dR=-(0-R)/darkLim, dG=-(0-G)/darkLim, dB=-(255-B)/darkLim;
    for (int t=0; t<360; t+=10) {
        Vector p1, p2;
        int r=-b, frame;
        for (; r<b; r+=4) {
            p1=mobius(a, r, t);
            p2=mobius(a, r+4, t);
            frame=p1*u;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
            p1=mobius(a, r, t);
            p2=mobius(a, r, t+10);
            frame=p1*u;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
        }
        p1=mobius(a, r, t);
        p2=mobius(a, r, t+10);
        frame=p1*v;
        draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(r+b)*dR, G+(r+b)*dG, B+(r+b)*dB));
        //wait(0.01);
    }
    }
    //Circle starts
    if (false){
    int a=60, b=180;
    int R=128, G=0, B=128;
    int darkLim=a;
    int dR=-(0-R)/darkLim, dG=-(0-G)/darkLim, dB=-(255-B)/darkLim;
    for (int t=0; t<360; t+=3) {
        Vector p1, p2;
        int r=0, frame;
        for (; r<b; r+=5) {
            p1=shell(a, r, t);
            p2=shell(a, r+5, t);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
            p1=shell(a, r, t);
            p2=shell(a, r, t+3);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
        }
        /*p1=mobius(a, r, t);
        p2=mobius(a, r, t+3);
        frame=p1*v;
        draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(r+b)*dR, G+(r+b)*dG, B+(r+b)*dB));*/
        wait(0.01);
    }
    }
    //Paraboloid starts
    if (false){
    double a=2, b=200;
    int R=128, G=0, B=128;
    double darkLim=0.642*b;
    int dR=-(0-R)/darkLim, dG=-(0-G)/darkLim, dB=-(255-B)/darkLim;
    for (int t=0; t<360; t+=5) {
        Vector p1, p2;
        int r=0, frame;
        for (; r<b; r+=5) {
            p1=paraboloid(a, r, t);
            p2=paraboloid(a, r+5, t);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
            p1=paraboloid(a, r, t);
            p2=paraboloid(a, r, t+5);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
        }
        /*p1=mobius(a, r, t);
        p2=mobius(a, r, t+3);
        frame=p1*v;
        draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(r+b)*dR, G+(r+b)*dG, B+(r+b)*dB));*/
        //wait(0.01);
    }
    }
    //p-orbital starts
    if (false){
    int a=100, b=180;
    int R=128, G=0, B=128;
    int darkLim=a;
    int dR=-(0-R)/darkLim, dG=-(0-G)/darkLim, dB=-(255-B)/darkLim;
    for (int t=0; t<360; t+=5) {
        Vector p1, p2;
        int r=0, frame;
        for (; r<b; r+=2) {
            p1=porbital(a, r, t);
            p2=porbital(a, r+2, t);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
            p1=porbital(a, r, t);
            p2=porbital(a, r, t+5);
            frame=p1*v;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
        }
        /*p1=mobius(a, r, t);
        p2=mobius(a, r, t+3);
        frame=p1*v;
        draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(r+b)*dR, G+(r+b)*dG, B+(r+b)*dB));*/
    }
    }
    //Torus starts
    if (false){
    int a=100, b=360;
    int R=128, G=0, B=128;
    int darkLim=a+20;
    int dR=-(0-R)/darkLim, dG=-(0-G)/darkLim, dB=-(255-B)/darkLim;
    for (int t=0; t<360; t+=15) {
        Vector p1, p2;
        int r=0, frame;
        for (; r<b; r+=15) {
            p1=torus(a, r, t);
            p2=torus(a, r+15, t);
            frame=p1*u;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
            p1=torus(a, r, t);
            p2=torus(a, r, t+15);
            frame=p1*u;
            draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(frame)*dR, G+(frame)*dG, B+(frame)*dB));
        }
        /*p1=mobius(a, r, t);
        p2=mobius(a, r, t+3);
        frame=p1*v;
        draw(o+x*(p1.x)+y*(p1.y)+z*(p1.z), o+x*(p2.x)+y*(p2.y)+z*(p2.z), COLOR(R+(r+b)*dR, G+(r+b)*dG, B+(r+b)*dB));*/
        //wait(0.01);
    }
    }
}
    wait(100);
}
