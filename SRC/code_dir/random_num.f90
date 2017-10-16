PROGRAM random_num

IMPLICIT NONE
real    :: randnum

! ----- variables for portable seed setting -----
INTEGER :: i_seed, i
INTEGER, DIMENSION(:), ALLOCATABLE :: a_seed
INTEGER, DIMENSION(1:8) :: dt_seed
! ----- Set up random seed portably -----
CALL RANDOM_SEED(size=i_seed)
ALLOCATE(a_seed(1:i_seed))
CALL RANDOM_SEED(get=a_seed)
CALL DATE_AND_TIME(values=dt_seed)
a_seed(i_seed)=dt_seed(8); a_seed(1)=dt_seed(8)*dt_seed(7)*dt_seed(6)
CALL RANDOM_SEED(put=a_seed)
DEALLOCATE(a_seed)

CALL RANDOM_NUMBER(randnum)
print *, randnum
STOP
END
