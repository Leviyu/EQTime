#include<stdio.h>
#include<math.h>
//#include<ASU_tools.h>

// 3 functions
// rad_pat
// rad_pat_loc
// rad_pat_rayp

/***********************************************************
 * This C function calculate the radiation pattern ( amplitude )
 * for given fault orientation and ray path orientation on
 * the lower hamisphere.
 *
 * (for rad_pat_loc:)
 * The ray path orientation is calculated using given event/
 * station location and chosen phase. (use TauP here.)
 *
 * double strike    ----  Falut strike. (in deg, 0~360)
 * double dip       ----  Falut dip.  (in deg, 0~90)
 * double rake      ----  Falut rake.  (in deg, -180~180)
 * double az        ----  Ray path azimuth. (in deg)
 * double take_off  ----  Ray path take-off angle. (in deg, 0~180)
 * int    P         ----  Chosen component. (P--0, SV--1, SH--2).
 *
 * (for rad_pat_loc:)
 * double evlo    ----  Event location longitude.
 * double evla    ----  Event location latitude.
 * double evdp    ----  Event depth. ( in km )
 * double stlo    ----  Station location longitude.
 * double stla    ----  Station location latitude.
 * char   *phase  ----  Chosen phase.
 *
 * Shule Yu
 * Jun 13 2014
 *
 * Key words: radiation pattern.
 *
 * Reference:
 *     Aki 2002, Page 108~109.
***********************************************************/

double rad_pat(double strike,double dip,double rake,double az,double take_off,int P){

    if (P<0 || P>2){
        printf("In %s: Component error ...\n",__func__);
        printf("P--0, SV--1, SH--2\n");
        return -1;
    }

    double AMP,s,d,r,a,t;

    // Convert parameters into rad unit.
    s=strike*M_PI/180;
    d=dip*M_PI/180;
    r=rake*M_PI/180;
    a=az*M_PI/180;
    t=take_off*M_PI/180;

    // Calculate.
    if (P==0){

        AMP=cos(r)*sin(d)*pow(sin(t),2)*sin(2*(a-s))-
            cos(r)*cos(d)*sin(2*t)*cos(a-s)+
            sin(r)*sin(2*d)*(pow(cos(t),2)-pow(sin(t),2)*pow(sin(a-s),2))+
            sin(r)*cos(2*d)*sin(2*t)*sin(a-s);

        return AMP;
    }

    if (P==1){

        AMP=sin(r)*cos(2*d)*cos(2*t)*sin(a-s)-
            cos(r)*cos(d)*cos(2*t)*cos(a-s)+
            0.5*cos(r)*sin(d)*sin(2*t)*sin(2*(a-s))-
            0.5*sin(r)*sin(2*d)*sin(2*t)*(1+pow(sin(a-s),2));

        return AMP;
    }

    if (P==2){

        AMP=cos(r)*cos(d)*cos(t)*sin(a-s)+
            cos(r)*sin(d)*sin(t)*cos(2*(a-s))+
            sin(r)*cos(2*d)*cos(t)*cos(a-s)-
            0.5*sin(r)*sin(2*d)*sin(t)*sin(2*(a-s));

        return AMP;
    }

    return -1.0;
}

