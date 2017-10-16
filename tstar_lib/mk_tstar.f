c	grand's futterman operator code
c	emailed to ed in april 2004
c	tweaked, etc., by ed

	character name*80
	dimension fqp(9148)
	read(*,"(a80)")name
	read(*,*)npts
	read(*,*)dt
	read(*,*)tstar

      	call futt(fqp,npts,dt,tstar)

	beg=0.0
c	if SAC is needed: uncomment:
c         call wsac1(name,fqp,npts,beg,dt,nerr)

c	hopefully: in Q ascii format:
	write(*,"('DELTA: ',e12.6)")dt
	write(*,"('LENGTH: ',i7)")npts
	do 10 i=1,npts
10	write(*,"(e13.6)")fqp(i)
	end

	  subroutine futt(x,n,dt,ts)
	   complex q(10000)
	   complex dummy
	   dimension x(9148)
c           ****  8 sec -.65t* constant delay time (if t* is in sec)**
	   dummy=(0.0, 0.0)
	   pi=3.1415926
	   call binfd(n,lx1,np)
	   lhalf=lx1
	   lx1=lx1*2
	   np=np+1
	   lcent=lhalf+1
	   tt=float(lx1)*dt
	   df=1.0/tt
	   fn=df*float(lhalf)
	   q(1)=(1.0, 0.0)
	   do 50 i=2, lcent
	      f=float(i-1)*df
c	      c=2.*f*ts*(alog(f/1.)-pi*3./ts) put 8. below
              c=2.*f*ts*(alog(f/1.)-pi*8./ts+pi*.65)
	      c2=exp(-pi*f*ts)
	      r=c2*cos(c)
	      ai=c2*sin(c)
	      q(i)=cmplx(r,ai)
50        continue
	  call conj(q, lcent)
	  call cool(np,q, 1.)
	  fudge=exp(pi*ts)
	  do 51 i=1, n
	     x(i)=fudge*real(q(i))
51        continue
	  return
	  end

      SUBROUTINE BINFD(LX,LX1,N)
c      lx should be eq. to less than 2**20 (1048576)
C     2**N.LE.LX.LT.2**(N+1)
      DO 3 I=1,20
      IND = 2**I
      IF(LX-IND) 4, 5, 3
    3 CONTINUE
      N = 100
      write(*,*)  'Warning  lx too large!'
      RETURN
    4 CONTINUE
      N = I-1
      GO TO 6
    5 CONTINUE
      N = I
    6 CONTINUE
      LX1 = 2**N
      RETURN
      END

      SUBROUTINE CONJ(C,LX1)
C     LX1 = 2**N/2 + 1
      COMPLEX C(10000)
      NM1 = LX1 - 1
      DO 30 I=1, NM1
      NT = LX1 + I
      NU = LX1 - I
      C(NT) = CONJG (C(NU))
   30 CONTINUE
      C(LX1) = CMPLX (REAL(C(LX1)),0.0)
      RETURN
      END

      SUBROUTINE COOL(NN,DATAI,SIGNI)
      REAL DATAI(10000)
      N=2**(NN+1)
      J=1
      DO 5 I=1,N,2
      IF(I-J)1,2,2
    1 TEMPR=DATAI(J)
      TEMPI=DATAI(J+1)
      DATAI(J)=DATAI(I)
      DATAI(J+1)=DATAI(I+1)
      DATAI(I)=TEMPR
      DATAI(I+1)=TEMPI
    2 M=N/2
    3 IF(J-M)5,5,4
    4 J=J-M
      M=M/2
      IF(M-2)5,3,3
    5 J=J+M
      MMAX=2
    6 IF(MMAX-N)7,10,10
    7 ISTEP=2*MMAX
      THETA=SIGNI*6.28318531/FLOAT(MMAX)
      SINTH=SIN(THETA/2.)
      WSTPR=-2.0  *SINTH*SINTH
      WSTPI= SIN(THETA)
      WR=1.
      WI=0.
      DO 9 M=1,MMAX,2
      DO 8 I=M,N,ISTEP
      J=I+MMAX
      TEMPR=WR*DATAI(J)-WI*DATAI(J+1)
      TEMPI=WR*DATAI(J+1)+WI*DATAI(J)
      DATAI(J)=DATAI(I)-TEMPR
      DATAI(J+1)=DATAI(I+1)-TEMPI
      DATAI(I)=DATAI(I)+TEMPR
    8 DATAI(I+1)=DATAI(I+1)+TEMPI
      TEMPR=WR
      WR=WR*WSTPR-WI*WSTPI+WR
    9 WI=WI*WSTPR+TEMPR*WSTPI+WI
      MMAX=ISTEP
      GO TO 6
   10 RETURN
      END
