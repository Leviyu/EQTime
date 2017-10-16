PROGRAM radiation

! This program calculates the radiation pattern coefficients for a given
! set of fault parameters: strike, rake, and dip (in degrees).  The program
! then loops through all takeoff angles and azimuths and produces an output
! file with the format:
! Azimuth/Takeoff_Angle/Amplitude


IMPLICIT NONE

! Declare parameters

REAL, PARAMETER :: d2r = 0.01745329252          !Degrees => Radians

! Declare variables

REAL, DIMENSION (:,:), ALLOCATABLE :: amps_SH   !Array storing SH amplitude
REAL, DIMENSION (:,:), ALLOCATABLE :: amps_SV   !Array storing SV amplitudes
REAL, DIMENSION (:,:), ALLOCATABLE :: amps_P    !Array storing P amplitudes

REAL :: ft_strk					!Fault strike
REAL :: psi					!Angle between strike and azimuth
REAL :: rake					!Fault rake
REAL :: dip					!Fault dip
REAL :: takeoff, azi, lamda, delta, fi
REAL :: MAX_P,temp, tanvalue

CHARACTER (len=30) file1, file2, file3		!Name of files to dump amps into

INTEGER :: i_h					!Loop index: Take off angle
INTEGER :: az_s					!Loop index: Station azimuth

INTEGER :: r, c                                 !Write out loop indexes
INTEGER :: naz1,naz2                            !Begin and end azimuths
INTEGER :: nih1,nih2                            !Begin and end take off angles

! Number of values in each array
naz1 = 0
naz2 = 360
nih1 = 0 
nih2 = 90

! Initialize arrays => for finer spacing, increase the number of values and don't
! forget to divide azimuths and take off angles by this increment
ALLOCATE( amps_SH(naz1:naz2,nih1:nih2) )
ALLOCATE( amps_SV(naz1:naz2,nih1:nih2) )
ALLOCATE(  amps_P(naz1:naz2,nih1:nih2) )

! Read in fault parameters
WRITE (*,*) "Enter the strike, dip, and rake (degrees): "
READ (*,*) ft_strk, dip, rake

! Make file
!WRITE (*,*) "Enter the filenames (SH,SV,P): "
!READ (*,*) file1, file2, file3
file1 = 'SH_radiation'
file2 = 'SV_radiation'
file3 = 'P_radiation'
OPEN (UNIT=1, FILE=file1, STATUS='REPLACE', ACTION='WRITE')
OPEN (UNIT=2, FILE=file2, STATUS='REPLACE', ACTION='WRITE')
OPEN (UNIT=3, FILE=file3, STATUS='REPLACE', ACTION='WRITE')
MAX_P=0.0

lamda = rake*d2r
delta = dip*d2r
fi =  ft_strk * d2r
! Find amplitudes and place in array
DO i_h = nih1, nih2

    takeoff = i_h*d2r

  DO az_s = naz1, naz2
      

!      psi = ( ft_strk - REAL(az_s) )*d2r
      azi = az_s * d2r 
      psi =  azi - fi
      !SH Energy

       amps_SH(az_s,i_h) = cos(lamda)*cos(delta)*cos(takeoff)*sin(psi)&
                   + cos(lamda)*sin(delta)*sin(takeoff)*cos(2*psi)&
                   + sin(lamda)*cos(2*delta)*cos(takeoff)*cos(psi)&
                   - 0.5*sin(lamda)*sin(2*delta)*sin(takeoff)*sin(2*psi)


      !SV Energy
      amps_SV(az_s,i_h) = sin(lamda)*cos(2*delta)*cos(2*takeoff)*sin(psi)&
                   - cos(lamda)*cos(delta)*cos(2*takeoff)*cos(psi)&
                   + 0.5*cos(lamda)*sin(delta)*sin(2*takeoff)*sin(2*psi)&
                   - 0.5*sin(lamda)*sin(2*delta)*sin(2*takeoff)*(1+sin(psi)**2)

      !P Energy
      amps_P(az_s,i_h) = cos(lamda)*sin(delta)*(sin(takeoff)**2)*sin(2*psi)&
                   - cos(lamda)*cos(delta)*sin(2*takeoff)*cos(psi)&
                   + sin(lamda)*sin(2*delta)*(cos(takeoff)**2&
                   - sin(takeoff)**2*sin(psi)**2 )&
                   + sin(lamda)*cos(2*delta)*sin(2*takeoff)*sin(psi)

      temp = amps_SV(az_s,i_h)        
      if (ABS(temp) > MAX_P ) MAX_P = ABS(temp)
  END DO
END DO

! Dump data into the files           
Do c = nih1,nih2
   Do r = naz1, naz2

    ! Eastern hemisphere 0 => 180
!      WRITE (1,100) REAL(r), tanvalue, amps_SH(r,c)
!      WRITE (2,100) REAL(r), tanvalue, amps_SV(r,c)
!      WRITE (3,100) REAL(r), tanvalue, amps_P(r,c)

      ! Amps SH
      WRITE (1,'(2F18.8, F12.8)') REAL(r), REAL(c), amps_SH(r,c)
      WRITE (2,'(2F18.8, F12.8)') REAL(r), REAL(c), amps_SV(r,c)
      WRITE (3,'(2F18.8, F12.8)') REAL(r), REAL(c), amps_P(r,c)


  END DO
END DO

END PROGRAM
