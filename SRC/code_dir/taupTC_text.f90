program taupTC_text

IMPLICIT NONE
INTEGER                       :: status_read, ipts, npts
REAL(kind=8)                  :: DD, TT, ref_distance, ref_time1, ref_time2, timeAT_ref_distance  
REAL(kind=8),ALLOCATABLE      :: distance(:), time(:)
CHARACTER*80                  :: TC_file, temptext


   READ(*,*) TC_file
   READ(*,*) ref_distance
   READ(*,*) ref_time1, ref_time2

   status_read = 0
   ipts = 0
   open(11, file = TC_file)
   read(11,*) temptext
   do while ( status_read .eq. 0 )
      read(11,*, iostat= status_read ) DD, TT
      ipts = ipts+1
   end do
   npts = ipts - 1 
   rewind(11)
   ALLOCATE ( distance(npts), time(npts)) 
   read(11,*) temptext
   do ipts = 1, npts
     read(11,*) distance(ipts), time(ipts)
   end do 

   do ipts = 1, npts-1
       if (  (distance(ipts) <= ref_distance .and.  ref_distance <= distance(ipts+1))  .or. &
             (distance(ipts+1) <= ref_distance .and.  ref_distance <= distance(ipts)) ) then
            call LinerINTP( distance(ipts), time(ipts) , distance(ipts+1), time(ipts+1), ref_distance, timeAT_ref_distance )
            if ( timeAT_ref_distance >= ref_time1 .and. timeAT_ref_distance <= ref_time2 ) then
               print *, ref_distance, timeAT_ref_distance
            end if
       end if
        
   end do
   

STOP
END

SUBROUTINE LinerINTP(x1,y1,x2,y2,xx,yy)

real(kind=8)  :: x1,y1,x2,y2,xx,yy
real(kind=8)  :: slope

slope = (y2-y1)/(x2-x1)

yy=slope*xx+y1-slope*x1

return
end
 
