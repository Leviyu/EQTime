! calc the P, SH, and SV radiation pattern, using
! aki and richards formulation.  
! code modified by EJG, from an original
! code written by CHUNPENG ZHAO
! summer 2009
!
! input: strike, dip, rake ("fi,delta,lamda")
!        station theta (take-off angle) and az, 
!        and desired radiation pattern
!
! output: amplitude

         character*2 radpat
         real fi,lamda,delta,amp_theo
         real itheta,jaz,azi,takeoff

! fi = strike, delta = dip, lamda = rake
! read strike dip rake
         READ(*,*)fi,delta,lamda
! read desired radpat
         READ(*,*)radpat
! read take off angle and azimuth to compute
! radiation for:
         READ(*,*)itheta,jaz

         deg2rad=3.1415926/180
         rad2deg=1/deg2rad
         fi=fi*deg2rad
         delta=delta*deg2rad
         lamda=lamda*deg2rad
         takeoff=itheta*deg2rad
         azi=jaz*deg2rad
    
         if ( radpat == "P " ) then
!        P radiation Pattern
              amp_theo=cos(lamda)*sin(delta)*(sin(takeoff)**2.)
     c        *sin(2.*(azi-fi))-cos(lamda)*cos(delta)*sin(2*takeoff)
     c        *cos(azi-fi)+sin(lamda)*sin(2.*delta)*(cos(takeoff)**2.
     c        -sin(takeoff)**2.*sin(azi-fi)**2.)+sin(lamda)
     c        *cos(2.*delta)*sin(2.*takeoff)*sin(azi-fi)
              proj=takeoff*rad2deg 
              azi=azi*rad2deg
              if(abs(amp_theo) .lt. 0.0001) amp_theo=0.0
              write(*,*)amp_theo
         else if ( radpat == "SV" ) then
!        SV radiation Pattern
              amp_theo=sin(lamda)*cos(2.*delta)*cos(takeoff*2.)
     c        *sin(azi-fi)-cos(lamda)*cos(delta)*cos(2.*takeoff)
     c        *cos(azi-fi)+1./2.*cos(lamda)*sin(delta)
     c        *sin(takeoff*2)*sin(2.*(azi-fi))-1./2.*sin(lamda)
     c        *sin(2.*delta)*sin(2.*takeoff)*(1+sin(azi-fi)**2.)
              proj=takeoff*rad2deg
              azi=azi*rad2deg
              if(abs(amp_theo) .lt. 0.0001) amp_theo=0.0
              write(*,*)amp_theo
         else if ( radpat == "SH" ) then
!        SH radiation Pattern
              amp_theo=cos(lamda)*cos(delta)*cos(takeoff)
     c        *sin(azi-fi)+cos(lamda)*sin(delta)*sin(takeoff)
     c        *cos(2.*(azi-fi))+sin(lamda)*cos(2.*delta)*cos(takeoff)
     c        *cos(azi-fi)-1./2.*sin(lamda)*sin(2.*delta)
     c        *sin(takeoff)*sin(2.*(azi-fi))
              proj=takeoff*rad2deg
              azi=azi*rad2deg
              if(abs(amp_theo) .lt. 0.0001) amp_theo=0.0
              write(*,*)amp_theo
        end if
        end

